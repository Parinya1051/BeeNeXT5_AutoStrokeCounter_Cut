// #27-09-2024 parinya ปรับแต่ง Mode sim

// #include <BlynkGO_HTTPClient32.h>
#include "02_project.h"

BlynkGO_HTTPClient32 http;
IOT_response_cb_t _fn = NULL;
void IOT::request(String url, IOT_response_cb_t func)
{
  _fn = func;
  Serial.println("[IOT][Request] ....1");
  Serial.println("[IOT][URL] " + url);
  if (!data.is_simulation_mode)
  {
    Serial.println("[IOT][HTTPClient] GET DATA...................ONLINE_MODE");
    http.begin(url) // สำหรับ URL  http:// หรือ https:// แบบ insecure
        .onConnected([]()
                     { Serial.println("[IOT][HTTPClient] connected"); })
        .onResponse([](int httpCode, uint8_t *data, size_t len)
                    {
          Serial.printf("[IOT][HTTPClient] HTTP Code : %d\n", httpCode);
          _fn(httpCode, (const char*) data);
          http.end(); })
        // .onHeader([](String header, String value){
        //   Serial.printf("[HTTPClient][HEADER] %s : %s\n", header.c_str(), value.c_str());
        // })
        .onDisconnected([]()
                        {
                          Serial.println("[IOT][HTTPClient] disconnected");
                          // Serial.println("-----------------close--------------------");
                        });
    http.GET();
  }
  else
  {

    // จำลอง รับค่า URL แล้วตอบกลับคืนค่า json tracking no dataa
    // จำลอง หาก WebAPI คือ แบบ GET DATA

    if (-1 != (url.indexOf("GetItemStorage")))
    {
      Serial.println("[IOT][HTTPClient] GET DATA........GetItemStorage_SIM_MODE");
      String tracking_no = url.substring(url.indexOf("TrackingNo=") + 11);
      tracking_no.toUpperCase();
      Serial.print("tracking_no:");
      Serial.println(tracking_no);

      bool found_in_server = tracking_no.startsWith("TEST");
      // bool found_in_server = true;
      int tracking_cnt = found_in_server ? random(8000, 20000) : 0;
      int tracking_warning_zone = found_in_server ? random(18, 22) * 1000 : 0;
      int tracking_max = found_in_server ? random(30, 35) * 1000 : 0;
      String error_msg = found_in_server ? "-" : "error";

      Serial.println("I LOVE YOU:");
      Serial.println(tracking_cnt);
      Serial.println(tracking_warning_zone);
      Serial.println(tracking_max);
      Serial.println(error_msg);
      String json_response = StringX::printf(R"(
                {
                  "TrackingNo": "%s",
                  "CurrentStroke": %d,
                  "SafytyStroke": %d,
                  "PMStroke": %d,
                  "ErrorMsg": "%s"
                })",
                                             tracking_no.c_str(), tracking_cnt, tracking_warning_zone, tracking_max, error_msg.c_str());
      Serial.print("json_response");
      Serial.println(json_response);

      json_response.trim(); // จัดระเบียบ

      _fn(200, json_response.c_str()); // จำลอง response กลับ
    }
    else if (-1 != (url.indexOf("StrokeRecord")))
    {
      Serial.println("[IOT][HTTPClient] UPDATE DATA...StrokeRecord_SIM_MODE");

      int idx_tracking_no = url.indexOf("TrackingNo=") + 11;
      int idx_tracking_cnt = url.indexOf("Stroke=", idx_tracking_no + 1) + 7;
      int idx_program_version = url.indexOf("ProgramVersion=", idx_tracking_cnt + 1) + 15;
      int idx_machine_name = url.indexOf("MachineName=", idx_program_version + 1) + 12;

      String _tracking_no = url.substring(idx_tracking_no, idx_tracking_cnt - 8);
      _tracking_no.toUpperCase();
      int _tracking_cnt = url.substring(idx_tracking_cnt, idx_program_version - 16).toInt();
      String _program_version = url.substring(idx_program_version, idx_machine_name - 13);
      String _machine_name = url.substring(idx_machine_name);
#if USE_DEBUG
      Serial.println(_tracking_no);
      Serial.println(_tracking_cnt);
      Serial.println(_program_version);
      Serial.println(_machine_name);
#endif
      bool found_in_server = _tracking_no.startsWith("TEST");

      String &tracking_no = _tracking_no;
      int tracking_cnt = found_in_server ? _tracking_cnt : 0;
      int tracking_warning_zone = found_in_server ? random(18, 22) * 1000 : 0;
      int tracking_max = found_in_server ? random(30, 35) * 1000 : 0;
      String error_msg = found_in_server ? "-" : "error";

      String json_response = StringX::printf(R"(
                {
                  "TrackingNo": "%s",
                  "CurrentStroke": %d,
                  "SafytyStroke": %d,
                  "PMStroke": %d,
                  "ErrorMsg": "%s"
                })",
                                             tracking_no.c_str(), tracking_cnt, tracking_warning_zone, tracking_max, error_msg.c_str());

      json_response.trim();

      _fn(200, json_response.c_str()); // จำลอง response กลับ
    }
    else if (-1 != (url.indexOf("CheckmachineInfo"))) // จำลอง รับค่า URL แล้วตอบกลับคืนค่า json CheckmachineInfo  data
    {
      Serial.println("[IOT][HTTPClient] UPDATE DATA...CheckmachineInfo_SIM_MODE");
      int urllen = url.length();
      int idx_TrackingNo = url.indexOf("CheckmachineInfo?") + 17;
      String _TrackingNo = url.substring(idx_TrackingNo, urllen);
      _TrackingNo.toUpperCase();
#if USE_DEBUG
      Serial.println("I LOVE GOO:");
      Serial.println(_TrackingNo);
#endif
      bool found_in_server = _TrackingNo.startsWith("TEST") || _TrackingNo.startsWith("S000");

      String tracking_no = found_in_server ? _TrackingNo : "-";
      String version = found_in_server ? "2.29" : "-";
      int error_msg = found_in_server ? 0 : 1;

      String json_response = StringX::printf(R"(
                {
                  "TrackingNo"  :"%s",                  
                  "Version"     :"%s",
                  "ErrorCode"   :%d
                })",
                                             tracking_no, version, error_msg);

      json_response.trim();
      _fn(200, json_response.c_str()); // จำลอง response กลับ
    }
  }
}
