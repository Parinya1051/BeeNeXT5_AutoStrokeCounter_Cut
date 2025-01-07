#include <BlynkGOv3.h>
#include "02_project.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <AutoConnect.h>

using WiFiWebServer = WebServer;
using WiFiclient = WiFiClient;
WiFiclient interclient;
WiFiWebServer httpServer;


FONT_DECLARE(prasanmit_70);
GRect rect_updatefirmware;
GLabel lb_updatefirmware(rect_updatefirmware);

AutoConnect portal(httpServer);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace UPDATE_VERSION
{
  void update_started();
  void update_finished();
  void update_progress(int cur, int total);
  void update_error1(int err);
  void updateVersion();
  void getInformation(String URL_get_vevsrsion);
  void screenUpdatefirmware(String message,bool enable);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UPDATE_VERSION::update_started()
{
  Serial.println("CALLBACK:  HTTP update process started");

}

void UPDATE_VERSION::update_finished()
{
  Serial.println("CALLBACK:  HTTP update process finished");  
  
}

void UPDATE_VERSION::update_progress(int cur, int total)
{
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

void UPDATE_VERSION::update_error1(int err)
{
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UPDATE_VERSION::updateVersion()
{

  // Add optional calback notifies
  Serial.println("[UPDATE_VERSION]updateVersionFormServer:");
  httpUpdate.onStart(update_started);
  httpUpdate.onEnd(update_finished);
  httpUpdate.onProgress(update_progress);
  httpUpdate.onError(update_error1);
  String newVer = (data.Version_new);
  // Serial.print(interclient);
  Serial.print(DEFAULT_WEB_FIRMWARE_UPDATE);
  Serial.print(DEFAULT_NODERED_PORT);
  Serial.print(URL_UPDATE_ESP32);
  Serial.println(newVer);
  // String web_update_firmware ="http://smfwebservicesvr.mektec.co.th:1884";
  // Serial.print("URL_UPDATE_ESP32=");
  // Serial.println(web_update_firmware);
    // t_httpUpdate_return ret = httpUpdate.update(interclient, web_update_firmware, URL_UPDATE_ESP32 + ver);   //use ip address  DEFAULT_WEB_SERVERSERVICE  //DEFAULT_WEB_FIRMWARE_UPDATE
    t_httpUpdate_return ret = httpUpdate.update(interclient, DEFAULT_WEB_FIRMWARE_UPDATE, DEFAULT_NODERED_PORT , URL_UPDATE_ESP32 + newVer);   //use ip address  DEFAULT_WEB_SERVERSERVICE or DEFAULT_WEB_FIRMWARE_UPDATE
    // t_httpUpdate_return ret = httpUpdate.update(interclient, "192.168.231.202", 3000,"/firmware/AutoStrokeCounter_ESP32_2.02.bin");      //demo sim

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
    UPDATE_VERSION::screenUpdatefirmware(ALERT_UPDATE_FIRMWARE_ERR + httpUpdate.getLastErrorString()+"\n", true);

            static SoftTimer timer1;
            timer1.delay(5000, []()
                                {  
                                  UPDATE_VERSION::screenUpdatefirmware(ALERT_UPDATE_FIRMWARE_ERR,false);
                                  GRAPHIC::BARCODE_SCANNER::design(1);  });                                           //ถ้าอัปโหลดไม่ได้ให้กลับไปที่หน้าเดิม
                  
    data.state = P_STATE_SCANNER_STEP1;
    return;
   

    break;

  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("HTTP_UPDATE_NO_UPDATES");
    break;

  case HTTP_UPDATE_OK:
    Serial.println("HTTP_UPDATE_OK");
     
    break;
  }
}

void UPDATE_VERSION::handle()
{
  portal.handleClient();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UPDATE_VERSION::getInformation(String URL_getInformation)
{
#if USE_IOT
  Serial.print("[UPDATE_VERSION]getInformation:");
  Serial.println(URL_getInformation);
  
  if (data.is_simulation_mode)
  {
    //// SKIP When On WorkMode
  }
  else
  {
    if (!WiFi.isConnected())
      return;
  }
  IOT::request(URL_getInformation, [](int httpCode, const char *response)
               {              
                memset(&data.Version_new, 0, sizeof(data.Version_new));
                memset(&GRAPHIC::BARCODE_SCANNER::_data.Version_new, 0, sizeof(GRAPHIC::BARCODE_SCANNER::_data.Version_new));  // clear data ใน data.Version ก่อนรับค่าใหม่
                if(httpCode==200){                  
                  // Serial.printf("[UPDATE_VERSION][getInformation Tracking No] resp : %d\n", httpCode ); // ตอบกลับมา  
                  Serial.printf("[UPDATE_VERSION][getInformation Machine No] resp : %s\n", response ); // ตอบกลับมา 
                          StaticJsonDocument<500> json;
                          auto error = deserializeJson(json, response);
                          if(!error){
                                if( json.containsKey("ErrorCode")) {                                                                                                                         
                                    if( json.containsKey("ErrorCode") )       GRAPHIC::BARCODE_SCANNER::_data.error_msg_machine          = json["ErrorCode"]      .as<int>();
                                    int ErrorMsg = (GRAPHIC::BARCODE_SCANNER::_data.error_msg_machine);
                                    // Extract values//                                     
                                    GRAPHIC::BARCODE_SCANNER::_data.Version_new = "";
                                        if(ErrorMsg == 0) {  
                                          if( json.containsKey("TrackingNo")    ) snprintf(GRAPHIC::BARCODE_SCANNER::_data.machine_name, sizeof( GRAPHIC::BARCODE_SCANNER::_data.machine_name), "%s", json["TrackingNo"].as<const char*>());
                                          if( json.containsKey("ID")    )         snprintf(GRAPHIC::BARCODE_SCANNER::_data.machine_id,  sizeof( GRAPHIC::BARCODE_SCANNER::_data.machine_id),  "%s", json["ID"]        .as<const char*>());
                                          if( json.containsKey("Version")    )    (GRAPHIC::BARCODE_SCANNER::_data.Version_new                =  json["Version"]        .as<String>());
                                          if( json.containsKey("ErrorCode") )      GRAPHIC::BARCODE_SCANNER::_data.error_msg_machine          = json["ErrorCode"]       .as<int>();
                                          
                                          #if USE_DEBUG                               
                                          Serial.println("------------Machine_Resp-------------");
                                          Serial.printf("  [Tracking Machine Name.]    %s\n",GRAPHIC::BARCODE_SCANNER::_data.machine_name);
                                          Serial.printf("  [Tracking Machine ID.  ]    %s\n",GRAPHIC::BARCODE_SCANNER::_data.machine_id);
                                          Serial.printf("  [Tracking Version.     ]    %s\n",GRAPHIC::BARCODE_SCANNER::_data.Version_new);
                                          Serial.printf("  [Tracking Message-ERR. ]    %d\n",GRAPHIC::BARCODE_SCANNER::_data.error_msg_machine);                                    
                                          Serial.println("---------------memcpy----------------");                
                                          #endif                                           
                                          memcpy(&data, &GRAPHIC::BARCODE_SCANNER::_data, sizeof(GRAPHIC::BARCODE_SCANNER::_data));                     
                                        }
                                        else{
                                          Serial.println("[UPDATE_VERSION][JSON] error : The value of the ErrorCode code was found.  ");
                                        }
                                          
                                  }else{
                                      Serial.println("[UPDATE_VERSION][JSON] error : Could not find key name ErrorCode ");
                                    }
                        }else{
                          Serial.println("[UPDATE_VERSION][JSON] parse error");
                        }
              }else{
                Serial.printf("[UPDATE_VERSION][HTTPClient] httpCode : %d ....error ไม่พบข้อมูลที่ร้องขอ\n", httpCode);
              } });

#endif // USE_IOT
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UPDATE_VERSION::screenUpdatefirmware(String message,bool enable){  

     rect_updatefirmware.show(enable);
     rect_updatefirmware.size(800,480);
     rect_updatefirmware.color(TFT_SILVER);
     rect_updatefirmware.border(3,TFT_RED);
     rect_updatefirmware.align(ALIGN_CENTER);
     rect_updatefirmware.toForeground();
     lb_updatefirmware = message ;
     lb_updatefirmware.align(rect_updatefirmware,ALIGN_CENTER);
     lb_updatefirmware.font(prasanmit_70,TFT_RED);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////