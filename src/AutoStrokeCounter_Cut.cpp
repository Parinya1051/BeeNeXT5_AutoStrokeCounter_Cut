/*

25-07-2024_Parinya N.  #Add Animation Splash Screen
30-07-2024_Parinya N.  #Add MQTT_Server setting
01-08-2024_Parinya N.  #Add Serialport Setting RS232 and RS485
29-08-2024_Parinya N.  #Add wificonnect_state
17-09-2024_Parinya N   #Add //เคลียข้อมูลข้างเก่า _data.tracking_cnt ออกก่อน ไปรับค่ามาใหม่
17-09-2024_Parinya N   #Add void getInformation(); void checkProgramName();
18-09-2024_Parinya N   #Add void gettoolingdata();  แทน
18-09-2024_Parinya N   #Add แก้ ขั้นตอนการยิงบาร์โค้ด จาก อ่าน toolingname ก่อนเป็น อ่าน machinename ก่อน  พร้อมหน่วงเวลาให้แสดงค่าที่อ่านได้ในช่อง TextArear
25-09-2024_Parinya N   #Add OTA Update firme ware control version
07-11-2024_Parinya N   #Add Edit MQTT.setServer in start_gauge();
18-11-2024_Parinya N   #Check state INPUT PIN All Time 
18-11-2024_Parinya N   #Add subscuibe mqtt lastStroke form Server  topic:"AutoStrokeCounterDown/Toolling Model"

*/

#include <BlynkGOv3.h>
#include "02_project.h"
#include "soc/rtc_cntl_reg.h"

#define COPY_STRING(Ardu_String, my_c_str) \  
snprintf(my_c_str, sizeof(my_c_str), "%s", Ardu_String.c_str())

project_data_t data;
project_data_mqtt_t mqtt;

// Declare_Function
void checkProgramName();
void getDatatooling(String URL_get_data);

//====================================================================================ANIMATION=======================================================================================//


 //====================================================================================ANIMATION=======================================================================================//

void setup()
{
  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable เช็คพลังงานไม่พอ

  Serial.begin(115200);
  Serial.println();
  BlynkGO.begin();

  Serial.println("mqtt_setup_Server");
  MQTT.setServer(DEFAULT_MQTT_SERVER_NAME, DEFAULT_MQTT_SERVER_PORT);

  // BlynkGO.begin("110A6D3ED2A4A8C0");
  // BlynkGO.begin("40627A973BD19B23");
  // BlynkGO.begin("11FAE05F4522AAB8");
  // BlynkGO.begin("11D4DA5D913DE9C0");
  // BlynkGO.begin("11B56C9B0A7A01FE");
  // BlynkGO.begin("11127F19C6A5ACE9");//SN:8440-74648B4D-BDB876-70B8
  // BlynkGO.begin("11AEFD54F5993DAF");//SN:9610-749A8C4D-BDC476-88C4
  // BlynkGO.begin("11F0C038B041775E");//SN:AF90-7476894D-BDD476-A8D4
  // BlynkGO.begin("1169BE70333091EA");//SN:1690-F007039E-9E4CF2-984C//No.36

 // ====================================================================================MAIN_PROGRAM=======================================================================================//                       
                       PROJECT_DATA::init(data); // โหลดจาก flashMem Normal open
                      //  PROJECT_DATA::loadDefault(data);   // บังคับโหลดจาก default Normal close
                       PROJECT_DATA::save(data); // บังคับบันทึกลง flashMem Normal open                                     
                       Serial.printf("[MAIN][PASSCODE] %d\n", data.passcode);
                       checkProgramName();       // ตรวจสอบ _version
#if USE_MRT_EXI0404_I2C // I2C IO Expander  0-3 OUTPUT, 4-7 INPUT
                       MRT_EXI0404::init();
#endif

#if USE_BARCODE
                       HID_BARCODE ::init([](String barcode)
                                          {
                                            barcode.toUpperCase(); // ปรับ barcode ให้เป็นตัวพิมพ์ใหญ่ทั้งหมด
                                            Serial.printf("[MAIN][BARCODE] %s\n", barcode.c_str());

#if USE_GRAPHIC
                                            switch (data.state)
                                            {
                                            case P_STATE_ADMIN:
                                              Serial.println("[MAIN] P_STATE_ADMIN");
                                              GRAPHIC::PAGE_SETTING_BARCODE_SCANNER::barcode_sample(barcode);
                                              break;
                                            case P_STATE_SCANNER_STEP1:
                                            {
                                              Serial.println("[MAIN] P_STATE_SCANNER_STEP1");
                                              COPY_STRING(barcode, GRAPHIC::BARCODE_SCANNER::_data.machine_name); // copy barcode อ่านได้ไปไว้ที่ data.machine_name
                                              GRAPHIC::BARCODE_SCANNER::design(1);
                                              String URL_get_version = URL_GET_VERSION_TEMPLATE;
                                              URL_get_version.replace("[machine_name]", GRAPHIC::BARCODE_SCANNER::_data.machine_name);
                                              UPDATE_VERSION::getInformation(URL_get_version); // get information machine name

                                              if ((data.Version_new == "") || (data.Version_new == "-"))
                                              {
                                                GRAPHIC::BARCODE_SCANNER::alarm_machinename();
                                                Serial.println("[MAIN] This Machine Name No Data");
                                                data.state = P_STATE_SCANNER_STEP1;
                                                return;
                                              }
                                              else
                                              {
                                                float oldVer = (data.Version).toFloat();
                                                float newVer = (data.Version_new).toFloat();
                                                #if USE_DEBUG
                                                Serial.println("-----------Compare_Version-----------");
                                                Serial.print("  OldVer:");
                                                Serial.println(oldVer, 2);
                                                Serial.print("  NewVer:");
                                                Serial.println(newVer, 2);
                                                #endif
  
                                                if ((oldVer != newVer) && (!data.is_simulation_mode))
                                                {
                                                  Serial.println("[MAIN] UPDATE VERSION");
                                                  // UPDATE_VERSION::screenUpdatefirmware(ALERT_UPDATE_FIRMWARE, true);
                                                  static SoftTimer timer1;
                                                  timer1.delay(2000, []()
                                                               { 
                                                                // UPDATE_VERSION::updateVersion(); 
                                                                });
                                                }
                                                else
                                                {
                                                  Serial.println("[MAIN] NOT UPDATE VERSION");
                                                };
                                                Serial.println("-------------------------------------");
                                                static SoftTimer timer2;
                                                timer2.delay(500, []()
                                                             { GRAPHIC::BARCODE_SCANNER::design(2); });

                                                data.state = P_STATE_SCANNER_STEP2;
                                              }
                                              break;
                                            }

                                            case P_STATE_SCANNER_STEP2:
                                            {
                                              Serial.println("[MAIN] P_STATE_SCANNER_STEP2");
                                              COPY_STRING(barcode, GRAPHIC::BARCODE_SCANNER::_data.tracking_no); // copy barcode อ่านได้ไปไว้ที่ data.tracking_no
                                              GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt = 0;                  // เคลียข้อมูลข้างเก่า ออกก่อน ไปรับค่ามาใหม่
                                              GRAPHIC::BARCODE_SCANNER::_data.tracking_warning_zone = 0;         // เคลียข้อมูลข้างเก่า ออกก่อน ไปรับค่ามาใหม่
                                              GRAPHIC::BARCODE_SCANNER::_data.tracking_max = 0;                  // เคลียข้อมูลข้างเก่า ออกก่อน ไปรับค่ามาใหม่
                                              GRAPHIC::BARCODE_SCANNER::design(2);
                                              static SoftTimer timer3;
                                              timer3.delay(500, []()
                                                           { 
                                                  String URL_get_data = URL_GET_DATA_TEMPLATE;
                                                  URL_get_data.replace("[tracking_no]", GRAPHIC::BARCODE_SCANNER::_data.tracking_no);
                                                  getDatatooling(URL_get_data); });

                                              static SoftTimer timer4;
                                              timer4.delay(2000, []()
                                                           {                                                                                                                     
                                                            GRAPHIC::BARCODE_SCANNER::start_gauge();});                       
                                                           
                                              break;
                                            }
                                            }
#endif // USE_GRAPHIC
                                          });
#endif // USE_BARCODE

#if USE_GRAPHIC
                       /***********************
                        *  ออกแบบ กราฟิก
                        **********************/
                       GRAPHIC::design();

                       /***********************
                        *  ตัวเช็ค state
                        **********************/
                       GRAPHIC::state_checker(); // เช็คทั้ง pin ที่รับสัญญาณมาเพิ่ม tracking cnt และ ทั้งหน้ากราฟิก

                      //  GRAPHIC::mqtt_check(); //
                       
                       GRAPHIC::MQTT_F::mqtt_onMessage(); // check data onMessage 
                       /***********************
                        *  RS-485
                        **********************/
                      //  RS_485::init();
                      // RS_485::readholderRegiter_interval_updater(1);
// MQTT
#if USE_MQTT
  // mqtt_widget(); // MQTT_OnWitget
  GRAPHIC::MQTT_F::mqtt_onMessage(); 

  // static GTimer timer_mqttsub;
  // timer_mqttsub.setInterval(1000, []()
  //                           {                                                      
  //                             if (mqtt.mqtt_sub_enable)
  //                             {
  //                               Serial.println("mqtt_setup_Server");
  //                               MQTT.setServer(DEFAULT_MQTT_SERVER_NAME, DEFAULT_MQTT_SERVER_PORT);
  //                               MQTT.connect();
  //                               GRAPHIC::MQTT_F::mqtt_sub();         // จะจำไว้ และเมื่อ mqtt เชื่อมต่อสำเร็จ จะ subscribe ให้อัตโนมัติ
  //                               GRAPHIC::MQTT_F::mqtt_subDown();       // จะจำไว้ และเมื่อ mqtt เชื่อมต่อสำเร็จ จะ subscribe ให้อัตโนมัติ  
  //                               mqtt.mqtt_sub_enable = false;    
  //                             } 
  //                           });
#endif // USE_MQTT




#endif // USE_GRAPHIC

  //=================================================================================END MAIN_PROGRAM===================================================================================//
}
 //====================================================================================MAIN_LOOP=======================================================================================//
void loop()
{
  BlynkGO.update();

}
 //====================================================================================END LOOP=======================================================================================//

 // ==================================================================================IOT EVENT=======================================================================================//
WIFI_DISCONNECTED()
{
  mqtt.wificonnect_state = false;
}

WIFI_CONNECTED()
{
  mqtt.wificonnect_state = true;

}
//
/* ใส่หรือไม่ก็ได้ */

#if USE_MQTT

MQTT_CONNECTED()
{

  GRAPHIC::MQTT_F::mqtt_connected(); // show status mqtt 

}
///* ใส่หรือไม่ก็ได้ */
MQTT_DISCONNECTED()
{

  GRAPHIC::MQTT_F::mqtt_disconnected();
}
#endif // USE_MQTT


 // ==================================================================================SUB1_PROGRAM=====================================================================================//
void checkProgramName()
{
  Serial.println("");
  String Programname = __FILE__;
  Serial.print("ProgramName:");
  Serial.println(Programname);
  String pro = Programname.substring(4, Programname.length() - 4);

  int lastIndex = pro.lastIndexOf('_');
  String version = pro.substring(lastIndex + 1, pro.length());
  Serial.print("Version:");
  Serial.println(version); 
  data.Version = version; 

}
 // ==================================================================================SUB2_PROGRAM======================================================================================//
void getDatatooling(String URL_get_data)
{

#if USE_BARCODE
#if USE_IOT

  //  String URL_get_data = URL_GET_DATA_TEMPLATE;
  //  URL_get_data.replace("[tracking_no]", GRAPHIC::BARCODE_SCANNER::_data.tracking_no);
  //  Serial.print("URL_get_data:");Serial.println(URL_get_data);
  //  URL_get_data:http://mmct-ws.mektec.co.th:80/API/TMS/GetItemStorage?TrackingNo=TEST01

  if (data.is_simulation_mode)
  {
    //// SKIP When On WorkMode
  }
  else
  {
    if (!WiFi.isConnected())
      return;
  }
  // URL_get_data:http://mmct-ws.mektec.co.th:80M/API/TSM/GetItemStorage?TrackingNo=TEST01

  IOT::request(URL_get_data, [](int httpCode, const char *response)
               {
                            if(httpCode==200){
                              // RESP    : {"TrackingNo":"TEST-SM-001","CurrentStroke":27939870,"SafytyStroke":20000,"PMStroke":30000,"ErrorMsg":"-"}
                              Serial.printf("[MAIN][WebAPI req. Tracking No] resp : %s\n", response ); // ตอบกลับมา
                              StaticJsonDocument<250> json;
                              auto error = deserializeJson(json, response);
                            
                              if(!error){
                                Serial.println("-------------------------------------");
                                // response ตัวอย่าง
                                // {"TrackingNo":"TEST-SM-001","CurrentStroke":27939870,"SafytyStroke":20000,"PMStroke":30000,"ErrorMsg":"-"}
                                Serial.println("[MAIN][JSON]");
                                if( json.containsKey("ErrorMsg")) {
                                  snprintf(GRAPHIC::BARCODE_SCANNER::_data.error_msg, sizeof( GRAPHIC::BARCODE_SCANNER::_data.error_msg), "%s", json["ErrorMsg"].as<const char*>());
                                  String ErrorMsg = String(GRAPHIC::BARCODE_SCANNER::_data.error_msg); ErrorMsg.trim();
                                  if(ErrorMsg == "-") {
                                    if( json.containsKey("TrackingNo")    )  snprintf(GRAPHIC::BARCODE_SCANNER::_data.tracking_no, sizeof( GRAPHIC::BARCODE_SCANNER::_data.tracking_no), "%s", json["TrackingNo"].as<const char*>());
                                    if( json.containsKey("CurrentStroke") )  GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt          = json["CurrentStroke"] .as<int>();
                                    if( json.containsKey("SafytyStroke") )   GRAPHIC::BARCODE_SCANNER::_data.tracking_warning_zone = json["SafytyStroke"]  .as<int>();
                                    if( json.containsKey("PMStroke") )       GRAPHIC::BARCODE_SCANNER::_data.tracking_max          = json["PMStroke"]      .as<int>();
                                   
                                    #if USE_DEBUG                               
                                    Serial.println("-------------Tooling_Resp------------");
                                    Serial.printf("  [Tracking NO.     ]       %s\n",GRAPHIC::BARCODE_SCANNER::_data.tracking_no);
                                    Serial.printf("  [Tracking Count.  ]       %d\n",GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt);
                                    Serial.printf("  [Tracking Warning.]       %d\n",GRAPHIC::BARCODE_SCANNER::_data.tracking_warning_zone);
                                    Serial.printf("  [Tracking Max.    ]       %d\n",GRAPHIC::BARCODE_SCANNER::_data.tracking_max);  
                                    Serial.println("-------------------------------------");                 
                                    #endif                  
                                    GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt_prev = GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt;
                                                              data.tracking_cnt_prev = GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt;
                                    //===================================================================================================//
                                                                        
                                    //===================================================================================================//
                                    
                                    /*******************************
                                     *  ตัว WebAPI UPDATE Data เป็นช่วงๆ
                                     **********************/
                                    GRAPHIC::web_api_interval_updater(true);  // ยิง WebAPI UPDATE DATA เป็นช่วงๆ ไปยัง Server ตามระยะช่วงๆที่กำหนด (มีการเช็คสถานะ เฉพาะเมื่อ state อยู่ที่โชว์หน้า GAUGE)
                                    
                                  }else{
                                    Serial.println("[MAIN][JSON] The value of the ErrorMsg code was found.");
                                  }
                                }else{
                                  Serial.println("[MAIN][JSON] error : Could not find key name ErrorMsg");
                                }
                              }else{
                                Serial.println("[MAIN][JSON] parse error");
                              }
                            }else{
                              Serial.printf("[MAIN][HTTPClient] httpCode : %d ....error\n", httpCode);
                            } });

#endif // USE_IOT
#endif // USE_BARCODE
}
// ===========================================================================================================================================================================//