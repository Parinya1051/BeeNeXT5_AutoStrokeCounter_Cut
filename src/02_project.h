#pragma once

#include <BlynkGOv3.h>            // สำหรับกราฟิก และอื่นๆ
#include <CH9350_USBKEY.h>        // HID Module สำหรับ ต่ออ่าน Barcode
#include <Adafruit_MCP23008.h>    // สำหรับอ่าน MRT_EXI0404  I2C
#include <BlynkGO_HTTPClient32.h> // สำหรับการสื่อสารทาง HTTP Client
#include <ArduinoJson.h>          //JSON Formatt
// #include <DFRobot_RTU.h>          //For Modbus RTU



/******************************************************************
 * History
 ************************
 * V 0.9.0
 *   - barcode scan    : OK
 *   - i2c MRT_EXI0404 : OK
 *   - GRAPHIC         : 80%
 *
 * V 0.9.1 @21/01/2567
 *   - เพิ่มระบบ simulation mode เปิดสวิตซ์ให้แสดงได้จาก หน้าตั้งค่า > server setting
 *
 * V 0.9.2 @22/01/2567
 *    - เพิ่ม WebAPI UPDATE Data ให้มีทั้งโหมดจำลอง และโหมดทำงานจริงได้
 *
 * V 0.9.3 @25/03/2567
 *    - เปลี่ยน BlynkGO_HTTPClient --> BlynkGO_HTTPClient32
 *
 * V 0.9.4 @26/03/2567
 *    - แก้ url สำหรับ update ไม่ได้ส่ง tracking_cnt ไป แต่ส่ง tracking_diff แทน
 *      โดยมีการจำค่า tracking_cnt_prev ไว้ด้วย เมื่อได้รับค่าจาก server มาครั้งแรก
 *    - fixed หน้าตา JSON ที่ ตอบคืนกลับมาให้ถอด ถูกตาม key จริง
 *
 * V 0.9.5 @27/03/2567
 *    - เช็ค state ทำงาน กับ state ปุ่มทั้งระบบ clean
 *    -
 *
 * by BlynkGO
 *
 * ****************************************************************/

//-----------------------------------------------------------------
// Version
//-----------------------
/** Major version number (X.x.x) */
#define MACHINE_TRACKING_VERSION_MAJOR 2
/** Minor version number (x.X.x) */
#define MACHINE_TRACKING_VERSION_MINOR 0
/** Patch version number (x.x.X) */
#define MACHINE_TRACKING_VERSION_PATCH 1

#define MACHINE_TRACKING_VERSION_TEXT (String(MACHINE_TRACKING_VERSION_MAJOR) + "." + String(MACHINE_TRACKING_VERSION_MINOR) + String(MACHINE_TRACKING_VERSION_PATCH))

/**
 * Macro to convert IDF version number into an integer
 *
 * To be used in comparisons, such as BLYNKGO_VERSION >= MACHINE_TRACKING_VERSION_VAL(3, 0, 0)
 */
#define MACHINE_TRACKING_VERSION_VAL(major, minor, patch) ((major << 16) | (minor << 8) | (patch))

/**
 * Current IDF version, as an integer
 *
 * To be used in comparisons, such as BLYNKGO_VERSION >= MACHINE_TRACKING_VERSION_VAL(3, 0, 0)
 */
#define BLYNKGO_VERSION MACHINE_TRACKING_VERSION_VAL(MACHINE_TRACKING_VERSION_MAJOR, \
                                                     MACHINE_TRACKING_VERSION_MINOR, \
                                                     MACHINE_TRACKING_VERSION_PATCH)

//-----------------------------------------------------------------
enum
{
  Y0 = 0,
  Y1,
  Y2,
  Y3,
  IN4,
  IN5,
  IN6,
  IN7
};
enum
{
  P_STATE_STANDBY,            // สถานะ standby
  P_STATE_ADMIN,              // สถานะ admin (อยู่ระหว่างเข้าไปหน้า admin เพื่อตั้งค่า)
  P_STATE_SCANNER_STEP1,      // สถานะ เข้าโหมด scan barcode's step1 โดยจะมีการปิด machine และ ปิดเสียงเตือนกระพริบ ส่วน เกจจ์มีการ reset
  P_STATE_SCANNER_STEP2,      // สถานะ เข้าโหมด scan barcode's step2 โดยจะมีการปิด machine และ ปิดเสียงเตือนกระพริบ ส่วน เกจจ์มีการ reset
  P_STATE_GAUGE_NORMAL_ZONE,  // สถานะ กำลังทำงาน  โดย tracking counter อยู่ใน zone ปกติ เพื่อ ทำการ รอนับเพิ่ม counter
  P_STATE_GAUGE_WARNING_ZONE, // สถานะ กำลังทำงาน  โดย tracking counter อยู่ใน zone warning ค่าปัจจุบันที่นับเข้าสู่พื้นที่เตรียมตัวระวัง ว่าใกล้ถึง tracking max
  P_STATE_DONE,               // สถานะ สิ้นสุดการทำงาน ค่า tracking counter มาถึง tracking max
  P_STATE_TASK_RESET,         // สถานะ เกิด IN4 เป็น LOW ให้มีการ reset งาน โดยมีหน้า แดงขึ้นก่อน
};

//-----------------------------------------------------------------
// User Config
//-----------------------------------------------------------------

#define USE_DEBUG_WIFI      0             // 0 หรือ 1 : สำหรับ ปิด เปิด Debug  print debug  for WIFI and MQTT ConnectCounter 
#define USE_DEBUG           0             // 0 หรือ 1 : สำหรับ ปิด เปิด Debug  print debug 
#define GRAPHIC_DEBUG       0             // 0 หรือ 1 : สำหรับ ปิด เปิด Debug state on display
#define USE_GRAPHIC         1             // 1 หรือ 0 : สำหรับ เปิด/ปิด การใช้งาน Graphic หรือไม่
#define USE_BARCODE         1             // 1 หรือ 0 : สำหรับ เปิด/ปิด การใช้งาน Barcode หรือไม่
#define USE_IOT             1             // 1 หรือ 0 : สำหรับ เปิด/ปิด การ request Web API โดย จะทำงานเมื่อ USE_BARCODE เป็น 1
#define USE_MRT_EXI0404_I2C 1             // 1 หรือ 0 : สำหรับ เปิด/ปิด I2C MRT-EXI0404 IO expander
#define USE_WEB             0             // 1 หรือ 0 : สำหรับ เปิด/ปิด WEB UP DATE
#define USE_MQTT            1             // 1 หรือ 0 : สำหรับ เปิด/ปิด MQTT UP DATE
#define USE_MQTT_EVERY_COUNT_UPDATE 1     // 1 หรือ 0 : สำหรับ เปิด/ปิด Publish ทุกๆคั้งที่มีการ นับ


// HID Module  ( DIP [12345] : [OFF,OFF,OFF,ON,ON] )
#define HID_BARCODE_RX_17       17 // ต่อ TX ของ HID Module
#define HID_BARCODE_TX_18       18 // ต่อ RX ของ HID Module
#define Serial_Barcode          Serial1

// เลือกใช้อย่างใดอย่างหนึ่ง Jumper ระหว่าง HID USB กับ Port RS232
#define RS232_RX_17             17 // ต่อ TX ของ RS232 Module
#define RS232_TX_18             18 // ต่อ RX ของ RS232 Module
#define Serial_RS232            Serial1

// RS485 MODUL HW-0591)
#define RS485_RX_11             11 // ต่อ RX ของ RS485 Module
#define RS485_TX_12             12 // ต่อ TX ของ RS485 Module
#define Serial_RS485            Serial2

#define MRT_I2C_SDA             19 // ร่วม I2C ของ Touch (Wire)
#define MRT_I2C_SCL             20 // ร่วม I2C ของ Touch (Wire)

//-----------------------------------------------------------------
// Default Config
//-----------------------
#define DEFAULT_PASSCODE 123456
#define DEFAULT_MACHINE_NAME              "Machine 01"
#define DEFAULT_TRACKING_NUMBER           "TEST-SM-001"
#define DEFAULT_TRACKING_WARNING_ZONE     20000                           // tracking ที่ถึงจุดเตือนว่าเข้า zone เตือน ว่ากำลังเข้าสู่พื้นที่จะถึง tracking max
#define DEFAULT_TRACKING_MAX              30000                           // tracking ค่ามากสุด ที่จะหยุดระบบทั้งหมดสิ้นสุดในรอบนั้นๆ
#define DEFAULT_TRACKING_NORMAL_ZONE_COLOR TFT_COLOR_HSV(170, 100, 60)    // TFT_COLOR_HSV(220,100,100)   // สีของ zone ปกติ
#define DEFAULT_TRACKING_WARNING_ZONE_COLOR TFT_COLOR_HSV(10, 100, 100)   // TFT_COLOR_HSV( 15,100,100)   // สีของ zone แจ้งเตือน


#define DEFAULT_SERVER_NAME               "http://mmct-ws.mektec.co.th"
#define DEFAULT_SERVER_PORT               80
#define DEFAULT_WEB_API_UPDATE_INTERVAL   1 // หน่วยเป็นนาที

#define DEFAULT_MQTT_SERVER_NAME          "SMFMQTTBrokerSvr.mektec.co.th"  // MQTT Server 192.168.226.221  
#define DEFAULT_MQTT_SERVER_PORT          1883

#define DEFAULT_WEB_SERVERSERVICE         "http://smfwebservicesvr.mektec.co.th"  //http://smfwebservicesvr.mektec.co.th/CheckmachineInfo?S000-00-00-0001   
#define DEFAULT_WEB_FIRMWARE_UPDATE       "192.168.226.222"     // #define DEFAULT_WEB_FIRMWARE_UPDATE "http://smfwebservicesvr.mektec.co.th"
#define DEFAULT_NODERED_PORT              1884                  // NODERED_PORT
#define DEFAULT_SERVICE_PORT              80                    // SERVERSERVICE_PORT

#define MQTT_USER "myhome"
#define MQTT_PASSWORD "myraspi"

#define DEFAULT_PIN_OUTPUT_FOR_MACHINE      Y1     // ขา pin output บน MRT_EXI0404 สำหรับ เปิด/ปิด machine (Y0,...Y3)
#define DEFAULT_PIN_OUTPUT_FOR_BUZZER       Y0     // ขา pin output บน MRT_EXI0404 สำหรับ เปิด/ปิด buzzer  (Y0,...Y3)
#define DEFAULT_PIN_OUTPUT_FOR_Y2           Y2     // ขา pin output บน MRT_EXI0404 สำหรับ spare
#define DEFAULT_PIN_OUTPUT_FOR_Y3           Y3     // ขา pin output บน MRT_EXI0404 สำหรับ spare
#define DEFAULT_PIN_INPUT_FOR_TASK_RESET    IN5    // ขา pin input  บน MRT_EXI0404 สำหรับ รับค่า pulse 1 ลูก ในการสั่งเริ่มต้นงานใหม่ตั้งแต่การเริ่ม barcode scan (IN4,...IN7)
#define DEFAULT_PIN_INPUT_FOR_TRACKING_ADD  IN4    // ขา pin input  บน MRT_EXI0404 สำหรับ รับค่า pulse 1 ลูก ในการสั่ง เพิ่ม counter ให้ tracking cnt      (IN4,...IN7)
#define DEFAULT_PIN_INPUT_FOR_IN6           IN6    // ขา pin input  บน MRT_EXI0404 สำหรับ spare
#define DEFAULT_PIN_INPUT_FOR_IN7           IN7    // ขา pin input  บน MRT_EXI0404 สำหรับ spare

#define DEFAULT_BAUDRATE_RS232            4   //{"9600", "19200", "38400", "57600", "115200"};
#define DEFAULT_CONFIG_RS232              6   //{"SERIAL_7N1", "SERIAL_7N2", "SERIAL_7O1", "SERIAL_7O2", "SERIAL_7E1", "SERIAL_7E2", "SERIAL_8N1", "SERIAL_8N2", "SERIAL_8O1", "SERIAL_8O2", "SERIAL_8E1", "SERIAL_8E2"};
#define DEFAULT_BAUDRATE_RS485            4   //{"9600", "19200", "38400", "57600", "115200"};
#define DEFAULT_CONFIG_RS485              6   //{"SERIAL_7N1", "SERIAL_7N2", "SERIAL_7O1", "SERIAL_7O2", "SERIAL_7E1", "SERIAL_7E2", "SERIAL_8N1", "SERIAL_8N2", "SERIAL_8O1", "SERIAL_8O2", "SERIAL_8E1", "SERIAL_8E2"};
#define ALERT_WIFI_OVERTIME               "1.เนื่องจากสัญญาณ WIFI  ขาดการติดต่อนานเกิน\n กว่าเวลาที่กำหนด\n2.จึงจำเป็นต้องทำการ รีสตาร์ท กล่อง SMART BOX นี้ \n3.กรุณากดปุ่ม ด้านล้าง เพื่อ รีสตาร์ท \n4.ตรวจสอบสัญญาณ WiFi ตรงมุมด้านขวาบน ว่าถูกต้องหรือไม่\n5...........................................\n";
#define ALERT_MQTT_OVERTIME               "1.เนื่องจากสัญญาณ MQTT  ขาดการติดต่อนานเกิน\n กว่าเวลาที่กำหนด\n2.จึงจำเป็นต้องทำการ รีสตาร์ท กล่อง SMART BOX นี้ \n3.กรุณากดปุ่ม ด้านล้าง เพื่อ รีสตาร์ท \n4.ตรวจสอบสัญญาณ WiFi ตรงมุมด้านขวาบน ว่าถูกต้องหรือไม่\n5...........................................\n";
#define ALERT_UPDATE_FIRMWARE             "Firmware Updating Smart Box\n\n           จอจะกระพริบ          \n           ห้ามปิดเครื่อง          \n"
#define ALERT_UPDATE_FIRMWARE_ERR         "Firmware Updating Smart Box\n\nเกิดข้อผิดพลาดในการ UPDATE\n"
//-----------------------------------------------------------------
// System Config
//-----------------------

#define WEP_API_GET_DATA                  "/API/TMS/GetItemStorage?TrackingNo=[tracking_no]"
#define WEP_API_UPDATE_DATA               "/API/TMS/StrokeRecord?TrackingNo=[tracking_no]&Stroke=[tracking_diff]&ProgramVersion=[version]&MachineName=[machine_name]"
#define URL_UPDATE_ESP32                  "/esp32/update?"
#define URL_GET_VERSION                   "/CheckmachineInfo?[machine_name]"

#define URL_GET_DATA_TEMPLATE             StringX::printf("%s:%d%s", data.server_name, data.server_port, WEP_API_GET_DATA)
#define URL_UPDATE_DATA_TEMPLATE          StringX::printf("%s:%d%s", data.server_name, data.server_port, WEP_API_UPDATE_DATA)
#define URL_GET_VERSION_TEMPLATE          StringX::printf("%s:%d%s", data.web_serverservice_name, data.web_service_port, URL_GET_VERSION)
#define INFO_ALERT_UPDATEFIRMWARE         StringX::printf("%s", ALERT_UPDATEFIRMWARE)
#define INFO_ALERT_UPDATEFIRMWARE_ERR     StringX::printf("%s", ALERT_UPDATE_FIRMWARE_ERR)

#define MACHINE_ON                        MRT_EXI0404::digitalWrite(data.pin_output_for_machine, HIGH)
#define MACHINE_OFF                       MRT_EXI0404::digitalWrite(data.pin_output_for_machine, LOW)
#define BUZZER_ON                         MRT_EXI0404::digitalWrite(data.pin_output_for_buzzer, HIGH)
#define BUZZER_OFF                        MRT_EXI0404::digitalWrite(data.pin_output_for_buzzer, LOW)
#define BUTTON_INPUT_ON                   LOW
#define BUTTON_INPUT_OFF                  HIGH

//-----------------------------------------------------------------
// Data
//-----------------------------------------------------------------

typedef struct _data_t
{
  uint32_t magicnumber;
  uint8_t state;
  uint8_t prev_state;
  String Version = ""; // version firmware
  String Version_new = "";
  char machine_id[100] = "";            // ชื่อหมายเลขเครื่องในระบบ
  char machine_name[200];               // ชื่อรหัสเครื่องจักร
  char tracking_no[100];                // ชื่อรหัสอุปกรณ์ (ให้ scan barcode ที่เครื่องมือ)
  int tracking_cnt;                     // tracking ค่าปัจจุบัน ครั้งแรกที่ อ่าน บาร์โคด  = "CurrentStroke" ใน json response
  int tracking_cntReturn;               // tracking  Return จาก Data Base ระหว่างการทำงาน  = "CurrentStroke" ใน json response   
  int tracking_cnt_prev;                // tracking ค่าเดิมที่ได้รับจาก server ก่อน update
  int tracking_warning_zone;            // tracking ค่าระดับที่เข้าสู่ ใกล้ จะเสร็จ / tracking ค่าที่เข้าสู่ zone มีเตือนให้เตรียมใกล้เสร็จ = "SafytyStroke"   ใน json response
  int tracking_max;                     // tracking ค่าสูงสุด                                                           = "PMStroke"      ใน json response
  char error_msg[200];                  // ข้อความ error จาก response ของ Web API tooling
  int error_msg_machine;                // ข้อความ error จาก response ของ Web API machine

  uint16_t tracking_normal_zone_color;
  uint16_t tracking_warning_zone_color;
  //WEB PATH
  uint8_t web_api_update_interval;      // ระยะเวลายิงทุกๆกี่นาที ในการยิง web api เพื่อทำการ update (หน่วยเป็นนาที)
  char server_name[200];                // ชื่อ server หรือ ip address ของ server ที่จะยิง web api
  uint16_t server_port;                 // หมายเลข port ของ server ที่จะยิง web api
  int32_t passcode;                     // รหัส passcode ของระบบป้องกันการเข้าไปตั้งค่าเล่น

  char mqtt_server_name[200];           // ชื่อ mqtt_server หรือ ip address ของ mqtt_server
  uint16_t mqtt_server_port;            // หมายเลข port ของ mqtt

  char web_serverservice_name[200];     // ชื่อ web_serverservice หรือ ip address ของ web_serverservice
  uint16_t web_nodered_port;            // หมายเลข port ของ NODERED
  uint16_t web_service_port;            // หมายเลข port ของ service

  bool is_simulation_mode;
  bool is_disp_sw_simulation;
  bool buzzer_state_blink;              // สถานะของการกระพริบของ Buzzer
  //OUTPUT
  uint8_t pin_output_for_machine;       // ขา pin output สำหรับ สั่ง machine ทำงาน/หยุดทำงาน
  uint8_t pin_output_for_buzzer;        // ขา pin output สำหรับ สั่ง buzzer ดัง/หยุดดัง
  uint8_t pin_output_for_y2;            // ขา pin input สำหรับ รับคำสั่ง spare
  uint8_t pin_output_for_y3;            // ขา pin input สำหรับ รับคำสั่ง spare
  //OUTPUT STATE
  bool y0;
  bool y1;
  bool y2;
  bool y3;
  //INPUT
  uint8_t pin_input_for_task_reset;     // ขา pin input สำหรับ รับคำสั่ง ให้มีการ reset เริ่มต้นใหม่จาก barcode scanner
  uint8_t pin_input_for_tracking_add;   // ขา pin input สำหรับ รับคำสั่ง ให้มีการเพิ่ม tracking counter ปัจจุบันไปอีก 1
  uint8_t pin_input_for_in6;            // ขา pin input สำหรับ รับคำสั่ง spare
  uint8_t pin_input_for_in7;            // ขา pin input สำหรับ รับคำสั่ง spare
  //INPUT STATE
  bool x4;
  bool x5;
  bool x6;
  bool x7;

  //RS232
  int rs232_bauddrate;                // สำหรับ รับค่า พารามิเตอร์ bautrate ของ rs232
  int rs232_databit;                  // สำหรับ รับค่า พารามิเตอร์ data bit ของ rs232
  //RS485
  int rs485_bauddrate;                // สำหรับ รับค่า พารามิเตอร์ bautrate ของ rs485
  int rs485_databit;                  // สำหรับ รับค่า พารามิเตอร์ data bit ของ rs485

} project_data_t;

extern project_data_t data;

//*****************************************************************//

//*****************************************************************//

//*****************************************************************//
typedef struct project_data_mqtt_t
{

  String MQTT_sub_topic;            // MQTT_sub_topic
  String MQTT_pub_topic;            // MQTT_pub_topic
  String MQTT_pub_payload;          // MQTT_pub_payload
  int wifi_disconnect_cnt = 0;          // นับจำนวนรอบที่ต่อ wifi ไม่ได้
  int mqtt_disconnect_cnt = 0;          // นับจำนวนรอบที่ต่อ mqtt ไม่ได้
  bool mqtt_sub_enable    = false;
  bool wificonnect_state  = false;
  bool mqttconnect_state  = false;;
  bool wifi_disconnect_cnt_state = false;;   // นับจำนวนรอบที่ต่อ wifi ไม่ได้
  bool mqtt_disconnect_cnt_state = false;;   // นับจำนวนรอบที่ต่อ mqtt ไม่ได้ 
               


} project_data_mqtt_t;

extern project_data_mqtt_t mqtt;
//*****************************************************************//

//-----------------------------------------------------------------
// extern widgets
//-----------------------
extern GWiFiManager wifi_manager;

//-----------------------------------------------------------------
// namespace
//-----------------------

namespace PROJECT_DATA
{
  void init(project_data_t &data);
  void load(project_data_t &data);
  void save(project_data_t &data);
  void loadDefault(project_data_t &data);
  String toJsonString(project_data_t &data);
}

typedef void (*common_cb_t)(void);
namespace GRAPHIC
{
  void design();
  void state_checker();
  void web_api_update();                             // ยิง WebAPI UPDATE DATA 1 รอบ ไปยัง Server โดยไม่มีการเช็คสถานะ
  void web_api_interval_updater(bool enable = true); // ยิง WebAPI UPDATE DATA เป็นช่วงๆ ไปยัง Server ตามระยะช่วงๆที่กำหนด (มีการเช็คสถานะ เฉพาะเมื่อ state อยู่ที่โชว์หน้า GAUGE)
  void tracking_no(String tracking_no);
  void page_task_alarm(bool enable = true);
  void mqtt_check();

  namespace APP_LOCKSCREEN
  {
    void reset();
    void mode_block_screen(int32_t passcode = 0, common_cb_t fn_ok = NULL, common_cb_t fn_cancel = NULL);
    void mode_change_passcode();
    void passcode(int32_t passcode);
    int32_t passcode();
    void onOK(common_cb_t fn_ok);
    void onCancel(common_cb_t fn_cancel);
  }

  namespace BARCODE_SCANNER
  {
    void design(uint8_t step = 1);
    void show(bool enable);
    void begin_system();
    void start_gauge();
    void alarm_machinename();

    extern project_data_t _data;
  }

  namespace GAUGE_TRACKING
  {
    void show(bool enable);
    void reset();
    void tracking_no(String tracking_no);
    String tracking_no();
    void tracking_next();
    void tracking_cnt(int tracking_cnt);
    int tracking_cnt();
    void tracking_warning_zone(int tracking_warning_zone);
    int tracking_warning_zone();
    void tracking_max(int tracking_max);
    int tracking_max();
    void info_blinking(bool enable, String into = "", color_t color = TFT_RED);
  }

  namespace PAGE_SETTING
  {
    void show(bool enable);
  }

  namespace PAGE_SETTING_BARCODE_SCANNER
  {
    void show(bool enable);
    void barcode_sample(String barcode);
  }

  namespace PAGE_SETTING_IO
  {
    void show(bool enable);

    // กำหนดสถานะกราฟิก สำหรับ OUTPUT Y0-Y3 เพื่อให้ตรงกับ บน MRT_EXI0404
    void y0(bool state);
    void y1(bool state);
    void y2(bool state);
    void y3(bool state);

    // กำหนดสถานะกราฟิก สำหรับ INPUT IN4-IN7 เพื่อให้ตรงกับ บน MRT_EXI0404
    void in4(bool state);
    void in5(bool state);
    void in6(bool state);
    void in7(bool state);
  }

  namespace MQTT_F
  {
    void mqtt_int();
    void mqtt_widget();
    void mqtt_sub();
    void mqtt_subDown();
    void mqtt_pub();
    void mqtt_onMessage();
    void mqtt_connected();
    void mqtt_disconnected();
  }

}

typedef void (*barcode_cb_t)(String barcode); // กำหนด ฟังชันค์ พ้อยเตอร์ ชื่อ barcode_cb_t เป็นแบบสตริง
namespace HID_BARCODE
{
  void init(barcode_cb_t func);
}

typedef void (*IOT_response_cb_t)(int httpCode, const char *response);
namespace IOT
{
  void request(String url, IOT_response_cb_t func);
}

extern Adafruit_MCP23008 mcp;
extern uint8_t Output_State;
namespace MRT_EXI0404
{
  void init();
  void reset();
  int8_t digitalRead(uint8_t pin);
  void digitalWrite(uint8_t pin, bool state);
  void printBinary();
}

namespace UPDATE_VERSION
{
  void update_started();
  void update_finished();
  void update_progress(int cur, int total);
  void update_error1(int err);
  void updateVersion();
  void handle();
  void getInformation(String URL_getInformation);
  void screenUpdatefirmware(String message, bool enable);

}


String Num2StringComma(int num);
