#include <BlynkGOv3.h>
#include "02_project.h"

FONT_DECLARE(BebasNeueBold_num_50);
FONT_DECLARE(prasanmit_70);
FONT_DECLARE(LatoBold_num_35);
FONT_DECLARE(LatoBold_num_50);
FONT_DECLARE(LatoBold_num_80);
FONT_DECLARE(EurostileNextLTPro_30);
FONT_DECLARE(EurostileNextLTPro_40);
FONT_DECLARE(EurostileNextWDPro_40);

IMG_DECLARE(img_background);
IMG_DECLARE(img_admin);
IMG_DECLARE(img_passcode);
IMG_DECLARE(img_machine);
IMG_DECLARE(img_alarm);
IMG_DECLARE(img_barcode_scanner80);
IMG_DECLARE(img_barcode_scanner);
IMG_DECLARE(img_barcode_scanner2);
IMG_DECLARE(img_server);
IMG_DECLARE(img_io_expander);
IMG_DECLARE(img_barcode_test_sm_001);
IMG_DECLARE(img_logommct);
IMG_DECLARE(img_SmartFac_Logo80);
IMG_DECLARE(img_gray);
IMG_DECLARE(img_default_70);

IMG_DECLARE(img_B5);
IMG_DECLARE(img_mqtt26);

/*******************************
 *  Widgets : หน้าหลัก
 ******************************/

GImage background(img_B5);              // วิตเจ็ตภาพพื้นหลัง

GWiFiManager wifi_manager;              // วิตเจ็ต wifi manager
GRect rect_block_wifi_manager;          // วิตเจ็ต สัมผัสขยายบังไม่ให้ wifi manager สัมผัสได้โดยตรง แต่จะให้เข้าสู่ ล็อคหน้าจอก่อน สำหรับ Power User ตั้งค่าเท่านั้น
//====================================================================================//

GImage logo(img_SmartFac_Logo80);       // logommct

GRect rect_programname;
GLabel lb_programname(rect_programname);
GLabel lb_version(rect_programname);
GRect rect_department;
GLabel lb_departmen(rect_department);
//====================================================================================//

//====================================================================================//
GCircle cir_mqtt;                        // วิตเจ็ตวงกลมรูป mqtt เพื่อแสดงทำงาน และไม่ทำงาน
GImage image_mqtt(img_mqtt26, cir_mqtt); // เอารูปภาพมาใส่ใว้ในวงกลม
//====================================================================================//

GLabel lb_mqtt_info;    // วิตเจ็ต สถานะ ข้อความ
GLabel lb_mqtt_message; // วิตเจ็ต สถานะ ข้อความ

GCircle cir_mqttsub_status;// วิตเจ็ต สถานะ MQTT_Onmessage
GLabel lb_mqttsub_status("0", cir_mqttsub_status);
//====================================================================================//
GCircle cir_machine; // วิตเจ็ตวงกลมรูป machine เพื่อแสดงทำงาน และไม่ทำงาน
GImage image_machine(img_machine, cir_machine);

GImageButton btn_alarm(img_alarm); // วิตเจ็ตปุ่ม alarm
GRect rect_touch_helper_btn_alarm; // วิตเจ็ต สัมผัสขยายพื้นที่ให้กดปุ่ม alarm ได้ง่ายขึ้น เวลาปุ่มมีการกระพริบ

GImageButton btn_admin(img_admin); // วิตเจ็ตเข้าหน้า Admin ( Power User / หน้าตั้งค่า ) ที่ก่อนเข้าได้จะมีหน้า ล็อคหน้าจอกันไว้ก่อน

GImageButton btn_scanner(img_barcode_scanner80); // วิตเจ็ตปุ่มกด สำหรับให้เริ่มระบบใหม่ ตั้งแต่การ เริ่ม scan barcode โดยผ่านหน้าจอ

GSwitch sw_simulation; // switch เลื่อน ON-OFF สำหรับเปิดปิดโหมด simulation
GLabel lb_simulation;
GCircle gsw_simulation;

GRect rect_alarm_titlebar;
GLabel lb_alarm_titlebar(rect_alarm_titlebar);
GLabel lb_alarm_message(rect_alarm_titlebar);
GButton btn_alarm_ok(rect_alarm_titlebar);

// String text_alarm = {"1.เนื่องจากสัญญาณ WIFI หรือ MQTT ขาดการติดต่อนานเกิน\nกว่าเวลาที่กำหนด\n2.จึงจำเป็นต้องทำการ รีสตาร์ท กล่อง SMART BOX นี้ \n3.กรุณากดปุ่ม ด้านล้าง เพื่อ รีสตาร์ท \n4.xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n5.xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"};

/*******************************
 * namespace GRAPHIC
 **********************/
namespace GRAPHIC
{
  void design();
  void state_checker();
  void web_api_update();                      // ยิง WebAPI UPDATE DATA 1 รอบ ไปยัง Server โดยไม่มีการเช็คสถานะ
  void web_api_interval_updater(bool enable); // ยิง WebAPI UPDATE DATA เป็นช่วงๆ ไปยัง Server ตามระยะช่วงๆที่กำหนด (มีการเช็คสถานะ เฉพาะเมื่อ state อยู่ที่โชว์หน้า GAUGE)
  void mqtt_check();

  namespace ALARM
  {
    void show(bool enable);
    void blinking(bool enable, bool show);
    void OFF();
    void ON();
  }

  /*******************************
   *  Barcode Scanner
   **********************/
  namespace BARCODE_SCANNER
  {
    void design(uint8_t step); // 1 or 2
    void show(bool enable);

    void begin_system();
    void start_gauge();
    void alarm_machinename();

    /*******************************
     * Widgets : Barcode Scanner
     **********************/
    GRect rect_barcode_scanner;
    GLabel lb_step_num(rect_barcode_scanner);
    GLabel lb_step_info(rect_barcode_scanner);
    GLabel lb_step_info2(rect_barcode_scanner);
    GTextArea ta_tracking_no(rect_barcode_scanner);
    GTextArea ta_machine_name(rect_barcode_scanner);

    GLabel lb_error_msg(rect_barcode_scanner);

    GButton btn_next_back(rect_barcode_scanner);
    GButton btn_cancel(rect_barcode_scanner);
    GButton btn_start(rect_barcode_scanner);

    project_data_t _data;
  }

  /*******************************
   *  หน้า lockscreen
   **********************/
  namespace APP_LOCKSCREEN
  {
    void design();
    void reset();
    void clear_graphic();

    void mode_block_screen(int32_t passcode, common_cb_t fn_ok, common_cb_t fn_cancel);
    void mode_change_passcode();
    void passcode(int32_t passcode);
    int32_t passcode();
    void onOK(common_cb_t fn_ok);
    void onCancel(common_cb_t fn_cancel);

    //-----------------------------------
    int32_t _passcode_system = 123456;
    String _passcode_user_input;
    String _first_time_passcode_setting;
    common_cb_t _fn_ok;
    common_cb_t _fn_cancel;
    bool _is_matched;
    uint8_t _mode; // mode 1 (block screen)  mode 2 (ตั้ง passcode)

    /*******************************
     * Widgets : หน้า lockscreen
     **********************/
    GRect app_lockscreen;
    GLabel lb_lockscreen(app_lockscreen);
    GContainer cont_cir_dot_lockscreen(app_lockscreen);
    GCircle cir_dot_lockscreen[6];
    GContainer cont_btn_lockscreen(app_lockscreen);
    GButton btn_lockscreen[10];
    GButton btn_lockscreen_cancel(app_lockscreen);

  } // namespace APP_LOCKSCREEN

  /*******************************
   *  gauge tracking
   **********************/
  namespace GAUGE_TRACKING
  {
    void design();
    void redesign_arc();

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

    void info_blinking(bool enable, String into, color_t color);

/*******************************
 * Widgets : gauge tracking
 **********************/
#define NORMAL_ZONE 0
#define NEAR_MAX_ZONE 1

    GRect rect_gauge;
    GCircle cir_gauge(rect_gauge);
    GLabel lb_gauge(cir_gauge);
    GLabel lb_manchine_name(cir_gauge);
    GLabel lb_tracking_no(cir_gauge);
    GLine ln_gauge_frame[2];
    GLabel lb_tracking_warning_zone_info(cir_gauge);
    GLabel lb_tracking_warning_zone_value(cir_gauge);
    GLabel lb_tracking_max_info(cir_gauge);
    GLabel lb_tracking_max_value(cir_gauge);
    GLabel lb_tracking_warning(cir_gauge);
    GLabel lb_update_info(cir_gauge);

    GArc arc_gauge_level[2];
    GLine ln_gauge_near_done_zone[2];
    GCircle cir_gauge_indic(rect_gauge);
  }

  /*******************************
   *  กำหนดให้ MACHINE ON หรือ OFF
   **********************/
  namespace MACHINE
  {
    void ON();
    void OFF();
    bool isON();
    inline bool isOFF() { return !isON(); }
  }

  /*******************************
   *  หน้า Setting/หน้า Admin
   **********************/
  namespace PAGE_SETTING
  {
    void design();
    void show(bool enable);

    /*******************************
     * Widgets : หน้า Setting/หน้า Admin
     **********************/
    GRect rect_setting;
    GRect rect_setting_titlebar(rect_setting);
    GButton btn_setting_back(rect_setting_titlebar);
    GLabel lb_setting_titlebar(rect_setting_titlebar);

    GContainer cont_icons(rect_setting);
    GIcon btn_setting_passcode(img_passcode, "Passcode ใหม่", cont_icons);
    GIcon btn_setting_barcode_scanner(img_barcode_scanner2, "Barcode Scanner", cont_icons);
    GIcon btn_setting_server(img_server, "Server", cont_icons);
    GIcon btn_setting_io_expander(img_io_expander, "IO Setting", cont_icons);
    // GIcon btn_setting_serialport(img_RS232_485_logo70, "Serial Port", cont_icons);
    GIcon btn_setting_setdefault(img_default_70, "Set Default", cont_icons);
    


  }

  /*******************************
   *  หน้า Barcode Scanner Setting
   ***************************/
  namespace PAGE_SETTING_BARCODE_SCANNER
  {
    void design();
    void show(bool enable);
    void barcode_sample(String barcode);

    /*******************************
     * Widgets : หน้า barcode scanner Setting
     **********************/
    GRect rect_setting_barcode_scanner;
    GRect rect_setting_barcode_scanner_titlebar(rect_setting_barcode_scanner);
    GButton btn_setting_barcode_scanner_back(rect_setting_barcode_scanner_titlebar);
    GLabel lb_setting_barcode_scanner_titlebar(rect_setting_barcode_scanner_titlebar);

    GImage image_barcode_sample(img_barcode_test_sm_001, rect_setting_barcode_scanner);
    GLabel lb_barcode_scan_test(rect_setting_barcode_scanner);
    GLabel lb_info(rect_setting_barcode_scanner);
  }
  

  /*******************************
   *  หน้า Server Setting
   ***************************/
  namespace PAGE_SETTING_SERVER
  {
    void design();
    void show(bool enable);

    /*******************************
     * Widgets : หน้า Server Setting
     **********************/
    GRect rect_setting_server;
    GRect rect_setting_server_titlebar(rect_setting_server);
    GButton btn_setting_server_back(rect_setting_server_titlebar);
    GLabel lb_setting_server_titlebar(rect_setting_server_titlebar);
    // SERVER//
    GLabel lb_server_name(rect_setting_server);
    GTextArea ta_server_name(rect_setting_server);

    GLabel lb_server_port(rect_setting_server);
    GTextArea ta_server_port(rect_setting_server);

    GLabel lb_web_api_update_interval(rect_setting_server);
    GDropdownList dd_web_api_update_interval(rect_setting_server);

    GLabel lb_disp_sw_simulation(rect_setting_server);
    GSwitch sw_disp_sw_simulation(rect_setting_server);

    // MQTT//
    GLabel lb_mqtt_server_name(rect_setting_server);
    GTextArea ta_mqtt_server_name(rect_setting_server);

    GLabel lb_mqtt_server_port(rect_setting_server);
    GTextArea ta_mqtt_server_port(rect_setting_server);
    // NODERED//
    GLabel lb_web_serverservice_name(rect_setting_server);
    GTextArea ta_web_serverservice_name(rect_setting_server);

    GLabel lb_web_nodered_port(rect_setting_server);
    GTextArea ta_web_nodered_port(rect_setting_server);

  }
  /*******************************
   *  หน้า IO Setting
   ***************************/
  namespace PAGE_SETTING_IO
  {
    /**********************************/
    void design();
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

    /*******************************
     * Widgets : หน้า IO Setting
     **********************/
    GRect rect_setting_io;
    GRect rect_setting_io_titlebar(rect_setting_io);
    GButton btn_setting_io_back(rect_setting_io_titlebar);
    GLabel lb_setting_io_titlebar(rect_setting_io_titlebar);

    GContainer cont_io(rect_setting_io);
    //////////////////////////////////////////////////////
    GContainer cont_output(cont_io);

    GRect rect_cont_btn_io_output(cont_output);
    GLabel lb_cont_btn_io_output(rect_cont_btn_io_output);
    GContainer cont_btn_io_output(cont_output);
    GButton btn_io_output[4];
    ///////////////////////////////////////////////////////
    GContainer cont_input(cont_io);

    GRect rect_cont_btn_io_input(cont_input);
    GLabel lb_cont_btn_io_input(rect_cont_btn_io_input);
    GContainer cont_btn_io_input(cont_input);
    GButton btn_io_input[4];
    //////////////////////////////////////////////////////
    GLabel lb_pin_output_for_machine(rect_setting_io);
    GDropdownList dd_pin_output_for_machine(rect_setting_io);

    GLabel lb_pin_output_for_buzzer(rect_setting_io);
    GDropdownList dd_pin_output_for_buzzer(rect_setting_io);
    //////////////////////////////////////////////////////
    GLabel lb_pin_input_for_task_reset(rect_setting_io);
    GDropdownList dd_pin_input_for_task_reset(rect_setting_io);

    GLabel lb_pin_input_for_tracking_add(rect_setting_io);
    GDropdownList dd_pin_input_for_tracking_add(rect_setting_io);

    GLabel lb_warning(rect_setting_io);

    /**********************************/
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
  /**********************************/

} // namespace GRAPHIC

//----------------------------------------------------------------------------------------------
void GRAPHIC::design()
{
  // background.position(0, 0);
  background.align(ALIGN_CENTER);

  wifi_manager.align(ALIGN_TOP_RIGHT, -20, 0);
  wifi_manager.font(prasanmit_70);

  // ตัวตั้งกันไม่ให้กด wifi_manager เดิมโดยตรง แต่ให้เข้าไปหน้า lock screen ใส่ passcode ให้ถูกต้องก่อน
  // rect_block_wifi_manager.size(wifi_manager.width(), wifi_manager.height());
  rect_block_wifi_manager.size(80, 80);
  rect_block_wifi_manager.radius(20);
  rect_block_wifi_manager.opa(0);
  rect_block_wifi_manager.border(5, TFT_GREEN);
  rect_block_wifi_manager.align(wifi_manager, ALIGN_CENTER);
  rect_block_wifi_manager.onClicked([](GWIDGET)
                                    { 
                                      //Enable PASS WORD
                                      GRAPHIC::APP_LOCKSCREEN::mode_block_screen(
                                          data.passcode, // รหัส passcode สำหรับ lock หน้าจอ
                                          []()
                                          { wifi_manager.event_send(EVENT_CLICKED); } // เมื่อผู้ใช้ ใส่ passcode ถุกต้อง ให้ไปทำอะไร : สั่งให้ wifi_manager เสมือนถูกกด เพื่อเข้าไปตั้งค่าไวไฟ
                                      ); });
  // logo.size(165,80);
  logo.align(ALIGN_TOP_LEFT, 10, 10); // logo

  rect_programname.show(true);
  rect_programname.size(350, 50); // กรอบ ชื่อโปรแกรม
  rect_programname.color(TFT_BLUE);
  rect_programname.border(5, TFT_BLUE);
  rect_programname.opa(0);
  rect_programname.radius(25);
  rect_programname.shadow(30, TFT_BLUE);
  // rect_programname.shadow_spread(5);
  rect_programname.shadow_offset(6, 6);
  rect_programname.align(ALIGN_IN_TOP_MID,0,5);          //ตำแหน่งวาง ด้านบนจอ
  // rect_programname.align(ALIGN_BOTTOM_MID, 0, -5); // ตำแหน่งวาง ด้านบล่างจอ
  lb_programname = "AutoStrokeCounter";            // label programname
  lb_programname.align(rect_programname, ALIGN_LEFT_MID,20);
  lb_programname.font(prasanmit_40, TFT_WHITE);

  lb_version.show(true);
  lb_version = "V" + String(data.Version); // label version
  lb_version.align(rect_programname, ALIGN_RIGHT_BOTTOM, -20, -2);
  lb_version.font(prasanmit_30, TFT_BLACK);

  rect_department.show(false);
  rect_department.size(350, 50); // กรอบ ชื่อแผนก
  rect_department.border(5, TFT_BLUE);
  rect_department.opa(0);
  rect_department.radius(20);
  rect_department.shadow(40, TFT_YELLOW);
  rect_department.shadow_spread(5);
  rect_department.align(ALIGN_BOTTOM_MID, 0, -5);
  lb_departmen = "MMCT Smart Factory"; // label ชื่อแผนก
  lb_departmen.align(rect_department, ALIGN_TOP_MID);
  lb_departmen.font(prasanmit_40, TFT_WHITE);

  //====================หน้าต่างแจ้งเตือน==========================//
  rect_alarm_titlebar.show(false);
  rect_alarm_titlebar.size(600, 350);
  rect_alarm_titlebar.color(TFT_RED);
  rect_alarm_titlebar.radius(20);
  rect_alarm_titlebar.border(4, TFT_WHITE);
  rect_alarm_titlebar.align(ALIGN_TOP_MID, 0, 60);

  lb_alarm_titlebar = "แจ้งเตือน";
  lb_alarm_titlebar.align(rect_alarm_titlebar, ALIGN_TOP_MID, 0, 10);
  lb_alarm_titlebar.font(prasanmit_40, TFT_WHITE);

  // lb_alarm_message = ALERT_WIFI_OVERTIME;
  lb_alarm_message.font(prasanmit_30, TFT_WHITE);
  lb_alarm_message.align(rect_alarm_titlebar, ALIGN_TOP_LEFT, 20, 50);
  lb_alarm_message.text_align(TEXT_ALIGN_LEFT);

  btn_alarm_ok = "Restart";
  btn_alarm_ok.size(100, 50);
  btn_alarm_ok.radius(5);
  btn_alarm_ok.color(TFT_SILVER);
  btn_alarm_ok.align(rect_alarm_titlebar, ALIGN_BOTTOM_MID, 0, -10);
  btn_alarm_ok.onClicked([](GWIDGET)

                         { ESP.restart(); });

  //=======================================================//

#if USE_MQTT
  cir_mqtt.size(80, 80);
  cir_mqtt.color(TFT_RED);
  cir_mqtt.align(wifi_manager, ALIGN_BOTTOM, 0, 5);
  image_mqtt.align(ALIGN_CENTER);
#endif

  cir_machine.size(80, 80);
  cir_machine.color(TFT_RED);
  cir_machine.align(wifi_manager, ALIGN_BOTTOM, 0, 105);
  image_machine.align(ALIGN_CENTER);

  btn_alarm.align(cir_machine, ALIGN_BOTTOM, 0, 20);
  btn_alarm.onClicked([](GWIDGET)
                      {
    GRAPHIC::ALARM::OFF();      // ไม่ให้ ปุ่ม alarm กระพริบ แต่แสดงอยู่
    GRAPHIC::ALARM::show(true); });

  rect_touch_helper_btn_alarm.size(90, 90);
  rect_touch_helper_btn_alarm.opa(0);
  rect_touch_helper_btn_alarm.align(btn_alarm, ALIGN_CENTER);
  rect_touch_helper_btn_alarm.onClicked([](GWIDGET)
                                        { btn_alarm.event_send(EVENT_CLICKED); });

  btn_admin.align(btn_alarm, ALIGN_BOTTOM, 0, 20);
  btn_admin.onClicked([](GWIDGET)
                      { GRAPHIC::APP_LOCKSCREEN::mode_block_screen(
                            data.passcode, // รหัส passcode สำหรับ lock หน้าจอ
                            []()
                            { GRAPHIC::PAGE_SETTING::show(true); } // หาก user ใส่รหัสถูกให้ไปทำอะไร : ให้แสดงหน้า ตั้งค่าขึ้นมา
                        ); });

  btn_scanner.align(ALIGN_LEFT_MID, 10, 0); // Barcode Position
  btn_scanner.onClicked([](GWIDGET)
                        {                            
                            lb_mqttsub_status = "0";                                                  
                             GRAPHIC::BARCODE_SCANNER::begin_system();                                                         
                          });

  GRAPHIC::BARCODE_SCANNER::_data.is_simulation_mode = (data.is_simulation_mode = false);
  sw_simulation.show(false);
  sw_simulation.size(100, 50);
  sw_simulation.align(ALIGN_BOTTOM_LEFT, 10, -30);

  sw_simulation.onPressed([](GWIDGET)
                          {
                                if(sw_simulation.isOFF())
                                {                               
                                  gsw_simulation.color(TFT_GREEN);
                                  GRAPHIC::BARCODE_SCANNER::_data.is_simulation_mode = (data.is_simulation_mode = true);
                                }
                                else
                                {
                                  gsw_simulation.color(TFT_RED);
                                  GRAPHIC::BARCODE_SCANNER::_data.is_simulation_mode = (data.is_simulation_mode = false);
                                }
                                   
    static SoftTimer timer_simulation;
    if(data.is_simulation_mode) {
      // จำลองใส่ค่าเอง
      timer_simulation.setInterval(10,[](){
        if(data.state >= P_STATE_GAUGE_NORMAL_ZONE) {
          GRAPHIC::GAUGE_TRACKING::tracking_cnt(GRAPHIC::GAUGE_TRACKING::tracking_cnt()+random(80,10));
        }
      });
      Serial.print("is_simulation_mode1 ");Serial.println(data.is_simulation_mode);
    }else{
      
      Serial.print("is_simulation_mode2 ");Serial.println(data.is_simulation_mode);
      timer_simulation.del();
      
    } });
  // data.is_simulation_mode ? sw_simulation.ON() : sw_simulation.OFF();

  lb_simulation = "Simulation MODE";
  lb_simulation.align(sw_simulation, ALIGN_BOTTOM_LEFT, 0, -5);

  gsw_simulation.show(false);
  gsw_simulation.radius(20);
  gsw_simulation.align(sw_simulation, ALIGN_OUT_RIGHT_MID);
  gsw_simulation.color(TFT_RED);
  /*******************************
   *  barcode scanner area
   **********************/
  GRAPHIC::BARCODE_SCANNER::design();

  /*******************************
   *  gauge tracking
   **********************/
  GRAPHIC::GAUGE_TRACKING::design();

  /*******************************
   *  หน้า Setting/หน้า Admin
   **********************/
  GRAPHIC::PAGE_SETTING::design();

  /*******************************
   *  หน้า Setting : Barcode Scanner
   ***************************/
  GRAPHIC::PAGE_SETTING_BARCODE_SCANNER::design();

  /*******************************
   *  หน้า Setting : Server
   ***************************/
  GRAPHIC::PAGE_SETTING_SERVER::design();

  /*******************************
   *  หน้า Setting : IO
   ***************************/
  GRAPHIC::PAGE_SETTING_IO::design();

  /*******************************
   *  หน้า Setting : SerialPort
   ***************************/
  // GRAPHIC::PAGE_SETTING_SERIALPORT::design();

  /*******************************
   *  หน้า lockscreen
   **********************/
  GRAPHIC::APP_LOCKSCREEN::design();
  GRAPHIC::APP_LOCKSCREEN::passcode(data.passcode);

  // เริ่มต้นทำงานโดยเสมือนว่า user กดปุ่ม btn_scanner
  // btn_scanner.event_send(EVENT_CLICKED);
  GRAPHIC::BARCODE_SCANNER::begin_system();
  
  #if USE_MQTT
  GRAPHIC::MQTT_F::mqtt_widget();    // MQTT_OnWitget                
  #endif // USE_MQTT


}

#if GRAPHIC_DEBUG
GLabel lb_check_state;
#endif

void GRAPHIC::state_checker()
{
  Serial.println("[state_checker] called");
  static SoftTimer timer_state_checker;
  timer_state_checker.setInterval(100, []()
                                  {

    enum {PIN_STATE_STANDBY, PIN_STATE_ON, PIN_STATE_OFF = PIN_STATE_STANDBY, PIN_STATE_HIGH= PIN_STATE_ON }; // BUTTON_SWITCH
    static uint8_t pin_state_task_reset = PIN_STATE_OFF;
    static uint8_t pin_state_counter    = PIN_STATE_STANDBY;
    //----------------------------------------------------------------------------------------------//  
    data.x4 = MRT_EXI0404::digitalRead(data.pin_input_for_task_reset);
    data.x5 = MRT_EXI0404::digitalRead(data.pin_input_for_tracking_add);
    data.x6 = MRT_EXI0404::digitalRead(data.pin_input_for_in6);
    data.x7 = MRT_EXI0404::digitalRead(data.pin_input_for_in7);   
    //----------------------------------------------------------------------------------------------//

    switch(pin_state_task_reset){
      case PIN_STATE_OFF:
        if(BUTTON_INPUT_ON == MRT_EXI0404::digitalRead(data.pin_input_for_task_reset) ){           
          pin_state_task_reset = PIN_STATE_ON;
          GRAPHIC::page_task_alarm(false);

#if GRAPHIC_DEBUG
            Serial.printf("[MASTER ON (1)] %d\n",  data.state);
            lb_check_state.color(TFT_RED);
            lb_check_state.align(ALIGN_BOTTOM);
            lb_check_state = data.state;
            lb_check_state.toForeground();
#endif


          if( data.state == P_STATE_TASK_RESET || data.state == P_STATE_STANDBY || data.state == P_STATE_SCANNER_STEP1 ) {
            // pin_state_task_reset = PIN_STATE_ON;
            // data.state = P_STATE_STANDBY; 
            GRAPHIC::BARCODE_SCANNER::begin_system();
          }
        }else{
          pin_state_task_reset = PIN_STATE_OFF;
          GRAPHIC::page_task_alarm(true);
          lb_mqttsub_status = ("0");
          
         
#if GRAPHIC_DEBUG
            Serial.printf("[MASTER OFF (1)] %d\n",  data.state);
            lb_check_state.color(TFT_RED);
            lb_check_state.align(ALIGN_BOTTOM);
            lb_check_state = data.state;
            lb_check_state.toForeground();
#endif
        }
        break;

      case PIN_STATE_ON:
        if(BUTTON_INPUT_OFF == MRT_EXI0404::digitalRead(data.pin_input_for_task_reset) ){       


#if GRAPHIC_DEBUG
            Serial.printf("[MASTER OFF (2)] %d\n",  data.state);
            lb_check_state.color(TFT_RED);
            lb_check_state.align(ALIGN_BOTTOM);
            lb_check_state = data.state;
            lb_check_state.toForeground();
#endif
          if(data.state == P_STATE_GAUGE_NORMAL_ZONE || data.state == P_STATE_GAUGE_WARNING_ZONE) {
#if USE_WEB
            Serial.println("[WebAPI] update before Master OFF");
            GRAPHIC::web_api_update();
#endif
#if USE_MQTT 
            Serial.println("[WebAPI] update before Master OFF");
            GRAPHIC::MQTT_F::mqtt_pub();
#endif
          }
          pin_state_task_reset = PIN_STATE_OFF;
          // GRAPHIC::page_task_alarm(true);
        }else{
          pin_state_task_reset = PIN_STATE_ON;
          GRAPHIC::page_task_alarm(false);
#if GRAPHIC_DEBUG
          Serial.printf("[MASTER ON (2)] %d\n",  data.state);
          lb_check_state.color(TFT_RED);
          lb_check_state.align(ALIGN_BOTTOM);
          lb_check_state = data.state;
          lb_check_state.toForeground();
#endif
        }
        break;
      // case PIN_STATE_OFF:
      //   break;      
    }

    if( GRAPHIC::GAUGE_TRACKING::tracking_warning_zone() == 0 ||  GRAPHIC::GAUGE_TRACKING::tracking_max() == 0 ) return;

    // เฉพาะ data.state ที่เป็น เกจจ์ Normal Zone หรือ อยู่โหมดเกจจ์ Warning Zone  เท่านั้น
    // ค่อยให้เช็ค ขา pin สำหรับเพิ่ม tracking counter หาก เป็น HIGH-->LOW ค่อยเพิ่ม tracking counter ปัจจุบันขึ้นอีก 1
    if( data.state == P_STATE_GAUGE_NORMAL_ZONE || data.state == P_STATE_GAUGE_WARNING_ZONE) {

      switch(pin_state_counter) {        
        case PIN_STATE_STANDBY:
        // Serial.println("PIN_STATE_STANDBY");
          if(HIGH == MRT_EXI0404::digitalRead(data.pin_input_for_tracking_add) ){
            pin_state_counter = PIN_STATE_HIGH;
            
          }
          break;
        case PIN_STATE_HIGH:   // Program จะกระโดดเข้ามาทำงานในนี้ทุกๆครั้งที่มีการกด
        // Serial.println("PIN_STATE_HIGH");
          if(LOW == MRT_EXI0404::digitalRead(data.pin_input_for_tracking_add) ){
            GRAPHIC::GAUGE_TRACKING::tracking_next();
            // Serial.println("|************************************************************************************************************|"); 
            Serial.println("  Add_+1::" + String(data.tracking_cnt));
          
#if USE_MQTT
#if USE_MQTT_EVERY_COUNT_UPDATE   
                  Serial.println("  mqtt_update ทุกๆครั้งที่นับ");
                  GRAPHIC::MQTT_F::mqtt_pub();                                             //mqtt-update เมื่่อถูกกด
#endif
#endif       
            pin_state_counter = PIN_STATE_STANDBY;
            
          }
          break;
      }
    }


    switch(data.state){
      case P_STATE_STANDBY:
      case P_STATE_ADMIN:
      case P_STATE_SCANNER_STEP1:
      case P_STATE_SCANNER_STEP2:
        GRAPHIC::MACHINE::OFF();      // ปิดเครื่องจักร
        GRAPHIC::ALARM  ::OFF();      // ปิด เสียง alarm
        break;

      ////4  
      case P_STATE_GAUGE_NORMAL_ZONE:       
        if( GRAPHIC::GAUGE_TRACKING::tracking_cnt() >= GRAPHIC::GAUGE_TRACKING::tracking_max() ) {    // เมื่อ tracking_cnt ถึงจุด max ตั้งแต่ ยิง ครั้งแรก
              GRAPHIC::MACHINE::OFF();      // ปิดเครื่องจักร
              GRAPHIC::ALARM  ::ON();       // เสียง alarm ดัง และที่หน้าจอแสดงกราฟิก alarm กระพริบ
              GRAPHIC::GAUGE_TRACKING::info_blinking(true, "DONE!!!", TFT_BLUE);                      //กระพริบเตือนข้อความ DONE!!!         
              if(data.tracking_no != "" && data.tracking_cnt > 0)                                     // ถ้า จำนวน tracking_cnt มากกว่าค่า tracking_max ให้ update ข้อมูลครั้งสุดท้ายไป เมื่อเสร็จสิ้น
              // Serial.println("P_STATE_GAUGE_NORMAL_ZONE 4-2");       
              GRAPHIC::web_api_interval_updater(false);                                               // ปิดไม่ให้มีการ update อีกหลังเสร็จสิ้น
              data.state = P_STATE_DONE;
              // Serial.println("P_STATE_GAUGE_NORMAL_ZONE 4-22");
          
          
        }else{                                                                                                // เมื่อ counter น้อยกว่าจุดเตือน
        if ( GRAPHIC::GAUGE_TRACKING::tracking_cnt() >= GRAPHIC::GAUGE_TRACKING::tracking_warning_zone() ) {  // เมื่อ counter เลยจุด เตือน
            if(mqtt.wifi_disconnect_cnt_state ||mqtt.mqtt_disconnect_cnt_state ){                             //ถ้าwifi ไม่สามารถ connected ได้ภายในเวลาที่กำหนด   3-08-2024
               data.state = P_STATE_GAUGE_WARNING_ZONE;
              // Serial.println("P_STATE_GAUGE_NORMAL_ZONE 4-3");

              }else{
              GRAPHIC::MACHINE::ON();       // เปิดเครื่องจักร
              GRAPHIC::ALARM  ::OFF();      // ปิด เสียง alarm
              GRAPHIC::GAUGE_TRACKING::info_blinking(true, "WARNING",TFT_YELLOW );                          //กระพริบเตือนข้อความ WARNING
              data.state = P_STATE_GAUGE_WARNING_ZONE;
              // Serial.println("P_STATE_GAUGE_NORMAL_ZONE 4-4");                                           //ถ้าค่า  tracking_cnt น้อยกว่า tracking_max และมากกว่า tracking_warning_zone จะทำงานใน โปรแกรมนี้ 1 ครั้ง
                   }
          }else{
            if(mqtt.wifi_disconnect_cnt_state ||mqtt.mqtt_disconnect_cnt_state){                             //ถ้าwifi ไม่สามารถ connected ได้ภายในเวลาที่กำหนด  3-08-2024
                GRAPHIC::MACHINE::OFF();     // ปิดเครื่องจักร
                GRAPHIC::ALARM  ::ON();      // เสียง alarm ดัง และที่หน้าจอแสดงกราฟิก 
                data.state = P_STATE_GAUGE_NORMAL_ZONE;   
                // Serial.println("P_STATE_GAUGE_NORMAL_ZONE 4-5");                 
              }else{  
              GRAPHIC::MACHINE::ON();       // เปิดเครื่องจักร
              GRAPHIC::ALARM  ::OFF();      // ปิด เสียง alarm
              GRAPHIC::GAUGE_TRACKING::info_blinking(false);
              data.state = P_STATE_GAUGE_NORMAL_ZONE;
              // Serial.println("P_STATE_GAUGE_NORMAL_ZONE 4-6");                                             //ถ้าค่า  tracking_cnt ไม่เกินค่า tracking_max และค่า  tracking_warning_zone จะวนทำงานใน โปรแกรมนี้      
                   }
          }          
        }
        
        break;
      ////5  
      case P_STATE_GAUGE_WARNING_ZONE:  
         
        if( GRAPHIC::GAUGE_TRACKING::tracking_cnt() >= GRAPHIC::GAUGE_TRACKING::tracking_max() ) //ถ้าค่า  tracking_cnt มากกว่า  tracking_warning_zone หลังจากนั้น ค่ามากกว่า tracking_max ในโปรแกรม P_STATE_GAUGE_WARNING_ZONE 5-1 จะ mqtt_pub ข้อมูลครั้งสุดท้ายไป
            {           
                GRAPHIC::MACHINE::OFF();                                                                        // ปิดเครื่องจักร
                GRAPHIC::ALARM  ::ON();                                                                         // เสียง alarm ดัง และที่หน้าจอแสดงกราฟิก alarm กระพริบ
                GRAPHIC::GAUGE_TRACKING::info_blinking(true, "DONE!!!", TFT_RED);
                // Serial.println("P_STATE_GAUGE_WARNING_ZONE 5-1"); 
                if(data.tracking_no != "" && data.tracking_cnt > 0) 
                // Serial.println("P_STATE_GAUGE_WARNING_ZONE 5-2"); 
                GRAPHIC::web_api_interval_updater(false);                                                        // ปิดไม่ให้มีการ update อีกหลังเสร็จสิ้น
                data.state = P_STATE_DONE;            
            }
        else if ( GRAPHIC::GAUGE_TRACKING::tracking_cnt() > GRAPHIC::GAUGE_TRACKING::tracking_warning_zone() ) 
            {   
                if(mqtt.wifi_disconnect_cnt_state || mqtt.mqtt_disconnect_cnt_state){                           //ถ้าwifi ไม่สามารถ connected ได้ภายในเวลาที่กำหนด   3-08-2024
                GRAPHIC::MACHINE::OFF();      // ปิดเครื่องจักร
                GRAPHIC::ALARM  ::ON();       // เสียง alarm ดัง และที่หน้าจอแสดงกราฟิก alarm 
                data.state = P_STATE_GAUGE_WARNING_ZONE;
                // Serial.println("P_STATE_GAUGE_WARNING_ZONE 5-3");
                }else{
                GRAPHIC::MACHINE::ON();       // เปิดเครื่องจักร
                GRAPHIC::ALARM  ::OFF();      // ปิด เสียง alarm                
                data.state = P_STATE_GAUGE_WARNING_ZONE;
                // Serial.println("P_STATE_GAUGE_WARNING_ZONE 5-4");            //ถ้าค่า  tracking_cnt ไม่เกินค่า tracking_max แต่มากว่าค่า  tracking_warning_zone จะทำใน P_STATE_GAUGE_NORMAL_ZONE 4-4 1 คร้ง แล้ว กระโดดมาวนทำงานใน โปรแกรมนี้
                }                  
            }
      break;

      ////6  
      case P_STATE_DONE:                                                          //ถ้าค่า  tracking_cnt มากกว่าค่า tracking_max จะทำใน P_STATE_GAUGE_NORMAL_ZONE 4-2 1 คร้ง แล้ว กระโดดมาวนทำงานใน โปรแกรมนี้ P_STATE_DONE
        Serial.println("P_STATE_DONE 6-1");
        break;                 
      ////7  
      case P_STATE_TASK_RESET: 

        break;

      } }, true);
}

void GRAPHIC::web_api_update()
{

  String URL_update_data = URL_UPDATE_DATA_TEMPLATE;

  URL_update_data.replace("[tracking_no]", data.tracking_no);
  // URL_update_data.replace("[tracking_cnt]", String(data.tracking_cnt));

  int32_t tracking_diff = data.tracking_cnt - data.tracking_cnt_prev;
  URL_update_data.replace("[tracking_diff]", String(tracking_diff));
  URL_update_data.replace("[version]", data.Version);
  URL_update_data.replace("[machine_name]", data.machine_name);

  Serial.printf("[GRAPHIC][WebUpdate] %s\n", URL_update_data.c_str());

#if USE_IOT
  if (data.is_simulation_mode)
  { // โหมด จำลอง UPDATE server ไม่ต้องมีอยู่จริง
  }
  else
  { // โหมด UPDATE ไปยัง server จริง
    if (!WiFi.isConnected())
      return;
  }

  IOT::request(URL_update_data, [](int httpCode, const char *response)
               {
    if(httpCode==200){
      static SoftTimer timer_graphic;

      // RESP    : {"TrackingNo":"TEST-SM-002","CurrentStroke":299960,"SafytyStroke":20000,"PMStroke":30000,"ErrorMsg":"-"}
      Serial.printf("[GRAPHIC][WebAPI Update] resp : \n", response );
      StaticJsonDocument<250> json;
      auto error = deserializeJson(json, response);
      if(!error){
        Serial.println("-------------------------------------");
        Serial.println("[GRAPHIC][JSON]");
        if( json.containsKey("ErrorMsg")) {
          snprintf(GRAPHIC::BARCODE_SCANNER::_data.error_msg, sizeof( GRAPHIC::BARCODE_SCANNER::_data.error_msg), "%s", json["ErrorMsg"].as<const char*>());
          String ErrorMsg = String(GRAPHIC::BARCODE_SCANNER::_data.error_msg); ErrorMsg.trim();
          if(ErrorMsg == "-") {

            if( json.containsKey("TrackingNo")    ) snprintf(GRAPHIC::BARCODE_SCANNER::_data.tracking_no, sizeof( GRAPHIC::BARCODE_SCANNER::_data.tracking_no), "%s", json["TrackingNo"].as<const char*>());
            if( json.containsKey("CurrentStroke") )  GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt          = json["CurrentStroke"] .as<int>();
            if( json.containsKey("SafytyStroke") )   GRAPHIC::BARCODE_SCANNER::_data.tracking_warning_zone = json["SafytyStroke"]   .as<int>();
            if( json.containsKey("PMStroke") )       GRAPHIC::BARCODE_SCANNER::_data.tracking_max          = json["PMStroke"]       .as<int>();

            Serial.println("-------------------------------------"); 
            Serial.println(GRAPHIC::BARCODE_SCANNER::_data.tracking_no);
            Serial.println(GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt);
            Serial.println(GRAPHIC::BARCODE_SCANNER::_data.tracking_warning_zone);
            Serial.println(GRAPHIC::BARCODE_SCANNER::_data.tracking_max);
            Serial.println("-------------------------------------");
            
        
            GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt_prev = GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt;
                                       data.tracking_cnt_prev = GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt;

            GRAPHIC::GAUGE_TRACKING::lb_update_info = "Updated!";
            GRAPHIC::GAUGE_TRACKING::lb_update_info.font(prasanmit_35, TFT_COLOR_HSV(220,100,50));
            GRAPHIC::GAUGE_TRACKING::lb_update_info.show(true);
            timer_graphic.delay(3000,[]{
              GRAPHIC::GAUGE_TRACKING::lb_update_info.show(false);
            });
          }else{
           
            GRAPHIC::GAUGE_TRACKING::lb_update_info = "ERR : Update!!";
            GRAPHIC::GAUGE_TRACKING::lb_update_info.font(prasanmit_35, TFT_COLOR_HSV(200,75,100));
            GRAPHIC::GAUGE_TRACKING::lb_update_info.show(true);
            timer_graphic.delay(3000,[]{
              GRAPHIC::GAUGE_TRACKING::lb_update_info.show(false);
            });
          }
        }
      }else{
        
        Serial.println("[GRAPHIC][JSON] parse error");
        GRAPHIC::GAUGE_TRACKING::lb_update_info = "ERR : JSON!!!";
        GRAPHIC::GAUGE_TRACKING::lb_update_info.font(prasanmit_35, TFT_COLOR_HSV(200,75,100));
        GRAPHIC::GAUGE_TRACKING::lb_update_info.show(true);
        timer_graphic.delay(3000,[]{
          GRAPHIC::GAUGE_TRACKING::lb_update_info.show(false);
        });
      }
    }else{
     
      Serial.printf("[GRAPHIC][HTTPClient] httpCode : %d ....error\n", httpCode);
      // ...... <--- กรณีติดต่อ server แล้ว ไม่ใช่ 200 กลับมา
    } });
#endif
}

// ทำงาน update เป็นช่วงๆ
void GRAPHIC::web_api_interval_updater(bool enable)
{
#if USE_IOT
  static SoftTimer timer_web_api_interval_updater;

  if (enable)
  {
    Serial.printf("[GRAPHIC][WebUpdate Interval] %d\n", data.web_api_update_interval);
    timer_web_api_interval_updater.setInterval(data.web_api_update_interval * 60 * 1000, []()
                                               {
      if(data.state == P_STATE_GAUGE_NORMAL_ZONE || data.state == P_STATE_GAUGE_WARNING_ZONE) {
#if USE_WEB   
        Serial.println("web_api_update ตามรอบเวลา");
        GRAPHIC::web_api_update();
#endif

#if USE_MQTT
        Serial.println("mqtt_update ตามรอบเวลา");      
        MQTT_F::mqtt_pub();
#endif 

      } }, false); // true คือเริ่มทำงาน แต่ครั้งแรกเลย แล้วถัดๆไป ค่อย ทำงานเป็นช่วงๆ
  }
  else
  {
    timer_web_api_interval_updater.del();
  }
#endif
}

void GRAPHIC::ALARM::blinking(bool enable, bool show)
{
  if (show == false)
    enable = false;

  static SoftTimer timer;
  if (enable)
  {
    btn_alarm.show(true);
    rect_touch_helper_btn_alarm.show(true);
    timer.setInterval(500, []()
                      {
      btn_alarm.show(!btn_alarm.show());
      if(data.buzzer_state_blink){
        BUZZER_ON;
        data.buzzer_state_blink=(!data.buzzer_state_blink);
      }
         else {
        BUZZER_OFF;
        data.buzzer_state_blink=(!data.buzzer_state_blink);

         } });
  }
  else
  {
    timer.del();
    btn_alarm.show(show);
    rect_touch_helper_btn_alarm.show(show);
    BUZZER_OFF;
  }
}

void GRAPHIC::ALARM::show(bool enable)
{
  btn_alarm.show(enable);
}

void GRAPHIC::ALARM::OFF()
{
#if USE_IOT
  // BUZZER_OFF;//.........................................................
#endif
  GRAPHIC::ALARM::blinking(false, false);
}

void GRAPHIC::ALARM::ON()
{
#if USE_IOT
  // BUZZER_ON;//...........................................................
#endif
  GRAPHIC::ALARM::blinking(true, true);
}

void GRAPHIC::tracking_no(String tracking_no)
{
  GRAPHIC::GAUGE_TRACKING::tracking_no(tracking_no);
}

// NewStep
/*******************************
 *  barcode scanner
 **********************/
void GRAPHIC::BARCODE_SCANNER::design(uint8_t step)
{
  rect_barcode_scanner.show(true);
  rect_barcode_scanner.size(350, 350);
  // rect_barcode_scanner.border(2, TFT_BLACK);
  rect_barcode_scanner.shadow(30, TFT_BLACK);
  rect_barcode_scanner.shadow_offset(6, 6);
  rect_barcode_scanner.corner_radius(40);
  lb_step_num.font(LatoBold_num_50, TFT_BLACK);
  lb_step_num.align(ALIGN_TOP_RIGHT, -30, 10);

  lb_step_info.font(prasanmit_70, TFT_BLACK);
  lb_step_info.align(ALIGN_TOP_LEFT, 30, 40);

  lb_step_info2.font(prasanmit_40, TFT_BLACK);
  lb_step_info2.align(lb_step_info, ALIGN_BOTTOM_MID, 0, -10);

  ta_tracking_no.clickable(false);
  ta_tracking_no.font(EurostileNextLTPro_30, TFT_COLOR_MIX(TFT_BLACK, TFT_WHITE, 180));
  ta_tracking_no.placeholder_font(EurostileNextLTPro_30);
  ta_tracking_no.width(rect_barcode_scanner.width() * 80 / 100);
  ta_tracking_no.text_align(TEXT_ALIGN_CENTER);
  ta_tracking_no.corner_radius(10);
  ta_tracking_no.border(2, TFT_BLACK);
  // ta_tracking_no.cursor_type(CURSOR_LINE);
  ta_tracking_no.cursor_type(CURSOR_HIDDEN);
  ta_tracking_no.align(lb_step_info2, ALIGN_BOTTOM, 0, 5);
  ta_tracking_no.placeholder("Tracking NO.");
  ta_tracking_no = String(GRAPHIC::BARCODE_SCANNER::_data.tracking_no);
  ta_tracking_no.onValueChanged([](GWIDGET)
                                { snprintf(GRAPHIC::BARCODE_SCANNER::_data.tracking_no, sizeof(GRAPHIC::BARCODE_SCANNER::_data.tracking_no), "%s", ta_tracking_no.toString().c_str()); });

  lb_error_msg.show(false);
  lb_error_msg.font(prasanmit_25, TFT_COLOR_HEX(0xef5350)); // TFT_RED);
  lb_error_msg.text_align(TEXT_ALIGN_CENTER);
  lb_error_msg.text_line_space(-10);
  lb_error_msg.align(ta_tracking_no, ALIGN_BOTTOM, 0, 5);

  ta_machine_name.clickable(false);
  ta_machine_name.show(false);
  ta_machine_name.font(EurostileNextLTPro_30, TFT_COLOR_MIX(TFT_BLACK, TFT_WHITE, 180));
  ta_machine_name.placeholder_font(EurostileNextLTPro_30);
  ta_machine_name.width(rect_barcode_scanner.width() * 80 / 100);
  ta_machine_name.text_align(TEXT_ALIGN_CENTER);
  ta_machine_name.corner_radius(10);
  ta_machine_name.border(2, TFT_BLACK);
  // ta_machine_name.cursor_type(CURSOR_LINE);
  ta_machine_name.cursor_type(CURSOR_HIDDEN);
  ta_machine_name.align(lb_step_info2, ALIGN_BOTTOM, 0, 5);
  ta_machine_name.placeholder("Machine Name");
  ta_machine_name = String(GRAPHIC::BARCODE_SCANNER::_data.machine_name);
  ta_machine_name.onValueChanged([](GWIDGET)
                                 { snprintf(GRAPHIC::BARCODE_SCANNER::_data.machine_name, sizeof(GRAPHIC::BARCODE_SCANNER::_data.machine_name), "%s", ta_machine_name.toString().c_str()); });

  btn_cancel = "Cancel";
  btn_cancel.round_design();
  btn_cancel.style[PRESSED].color(TFT_BLACK, TFT_BLACK);
  btn_cancel.style[PRESSED].opa(80);
  btn_cancel.width(120);
  btn_cancel.font_color(TFT_BLACK);
  btn_cancel.border(2, TFT_BLACK);
  btn_cancel.align(ALIGN_BOTTOM_LEFT, 20, -20);
  btn_cancel.onClicked([](GWIDGET)
                       {
      GRAPHIC::BARCODE_SCANNER::show(false);
      GRAPHIC::GAUGE_TRACKING::show(true);
      data.state = P_STATE_STANDBY; });

  btn_start = "START";
  btn_start.round_design();
  btn_start.style[PRESSED].color(TFT_BLACK, TFT_BLACK);
  btn_start.style[PRESSED].opa(80);
  btn_start.width(120);
  btn_start.font_color(TFT_BLACK);
  btn_start.border(2, TFT_BLACK);
  btn_start.align(ALIGN_BOTTOM_RIGHT, -20, -20);
  btn_start.onClicked([](GWIDGET)
                      { GRAPHIC::BARCODE_SCANNER::start_gauge(); });

  switch (step)
  {
  default:
  case 1:
  {
    lb_step_num = "1";
    lb_step_info = "สแกนบาร์โคด";
    lb_step_info2 = "สำหรับ Machine Name";

    ta_tracking_no.show(false);
    ta_machine_name.show(true);

    btn_next_back = SYMBOL_ARROW_RIGHT " ถัดไป";
    btn_next_back.round_design();
    btn_next_back.style[PRESSED].color(TFT_BLACK, TFT_BLACK);
    btn_next_back.style[PRESSED].opa(80);
    btn_next_back.width(120);
    btn_next_back.font_color(TFT_BLACK);
    btn_next_back.border(2, TFT_BLACK);
    btn_next_back.align(ALIGN_BOTTOM_RIGHT, -20, -20);
    btn_next_back.onClicked([](GWIDGET) { // ปุ่มถัดไป
      
      GRAPHIC::GAUGE_TRACKING::tracking_no(_data.machine_name);
      GRAPHIC::BARCODE_SCANNER::design(2);

      data.state = P_STATE_SCANNER_STEP1;
    });

    btn_cancel.show(true);
    btn_start.show(false);

    break;
  }
  case 2:
  {
    lb_step_num = "2";
    lb_step_info = "สแกนบาร์โคด";
    lb_step_info2 = "สำหรับ Tracking NO.";

     
    ta_tracking_no.show(true);
    ta_machine_name.show(false);    
    btn_next_back = SYMBOL_ARROW_LEFT " ถอยกลับ";
    btn_next_back.round_design();
    btn_next_back.width(120);
    btn_next_back.font_color(TFT_BLACK);
    btn_next_back.border(2, TFT_BLACK);
    btn_next_back.align(ALIGN_BOTTOM_LEFT, 20, -20);
    btn_next_back.onClicked([](GWIDGET) { // ปุ่มถอยกลับ
      GRAPHIC::BARCODE_SCANNER::design(1);
      data.state = P_STATE_SCANNER_STEP2;
    });

    btn_cancel.show(false);
    btn_start.show(true);

    break;
  }
  }
}

void GRAPHIC::BARCODE_SCANNER::show(bool enable)
{
  rect_barcode_scanner.show(enable);
}

void GRAPHIC::BARCODE_SCANNER::begin_system()
{
  memcpy(&GRAPHIC::BARCODE_SCANNER::_data, &data, sizeof(data)); // coppy  data ไปใว้ที่ _data ด้วย
  memset(&GRAPHIC::BARCODE_SCANNER::_data.machine_name, 0, sizeof(GRAPHIC::BARCODE_SCANNER::_data.machine_name));                // clear  _data.machine_name
  memset(&GRAPHIC::BARCODE_SCANNER::_data.tracking_no , 0, sizeof(GRAPHIC::BARCODE_SCANNER::_data.tracking_no));                  // clear  _data.machine_name
  memset(&GRAPHIC::BARCODE_SCANNER::_data.Version_new , 0, sizeof(GRAPHIC::BARCODE_SCANNER::_data.Version_new));                  // clear  _data.Version_new
  
  GRAPHIC::GAUGE_TRACKING::show(false);
  GRAPHIC::GAUGE_TRACKING::reset();
  GRAPHIC::MACHINE::OFF();
  GRAPHIC::ALARM::blinking(false, false);
  GRAPHIC::BARCODE_SCANNER::show(true);
  GRAPHIC::BARCODE_SCANNER::design(1);
  mqtt.mqtt_sub_enable = false;
  data.state = P_STATE_SCANNER_STEP1;
}

void GRAPHIC::BARCODE_SCANNER::alarm_machinename()
{
  GRAPHIC::BARCODE_SCANNER::design(1);
  GRAPHIC::BARCODE_SCANNER::lb_error_msg.show(true);
  GRAPHIC::BARCODE_SCANNER::lb_error_msg = "ไม่พบข้อมูล Machine Name นี้จาก Server\nให้สแกน Machine Name ใหม่";
  static SoftTimer timer;
  timer.delay(2000, []()
              { GRAPHIC::BARCODE_SCANNER::lb_error_msg.show(false); 
                GRAPHIC::BARCODE_SCANNER::ta_machine_name = ""; 
                GRAPHIC::BARCODE_SCANNER::ta_tracking_no = ""; });
  data.state = P_STATE_SCANNER_STEP1;
}

void GRAPHIC::BARCODE_SCANNER::start_gauge()
{
  Serial.println("          tart_gauge ");
  String _track_no = String(GRAPHIC::BARCODE_SCANNER::_data.tracking_no);
  _track_no.trim();
  String _error_msg = String(GRAPHIC::BARCODE_SCANNER::_data.error_msg);
  _error_msg.trim();
  if (_track_no == "")
  {
    GRAPHIC::BARCODE_SCANNER::design(1);

    GRAPHIC::BARCODE_SCANNER::lb_error_msg.show(true);
    GRAPHIC::BARCODE_SCANNER::lb_error_msg = "สแกนรหัส Tracking NO. ให้เรียบร้อย";
    static SoftTimer timer;
    timer.delay(2000, []()
                {
                  GRAPHIC::BARCODE_SCANNER::lb_error_msg.show(false);
                  GRAPHIC::BARCODE_SCANNER::ta_tracking_no = ""; });

    data.state = P_STATE_SCANNER_STEP1;
    return;
  }

  if (_error_msg != "-")
  {
    GRAPHIC::BARCODE_SCANNER::design(2);

    GRAPHIC::BARCODE_SCANNER::lb_error_msg.show(true);
    GRAPHIC::BARCODE_SCANNER::lb_error_msg = "ไม่พบข้อมูล Tracking NO. นี้จาก Server\nให้สแกน Tracking NO. ใหม่";
    static SoftTimer timer;
    timer.delay(2000, []()
                {
                  GRAPHIC::BARCODE_SCANNER::lb_error_msg.show(false);
                  GRAPHIC::BARCODE_SCANNER::ta_tracking_no = ""; 
                 
                  });
    data.state = P_STATE_SCANNER_STEP2;
    return;
  }

  GRAPHIC::BARCODE_SCANNER::lb_error_msg.show(false);
  GRAPHIC::BARCODE_SCANNER::show(false);
  GRAPHIC::GAUGE_TRACKING::show(true);

  static SoftTimer timer; // ตอน บันทึกลง flashMem จะมีคลื่นกวนส่วนจอได้ ทำให้เหมือนมีอาการจอเต้นได้ เลยให้ทิ้งช่วงเวลาค่อยบันทึก
  timer.delay(500, []()
              {
    // เพื่อให้ไม่บันทึกลง flashMem บ่อยๆเกินจำเป็น จะใช้ วิธีเช็คว่า ค่า machine_name ไม่ซ้ำของเดิมที่เคยบันทึกใน flashMem ค่อยบันทึกลง flashMem
    // ซึ่งจะช่วยให้ไม่ต้องบันทึลง flashMem บ่อยๆเกินจำเป็น
    if( String(GRAPHIC::BARCODE_SCANNER::_data.machine_name) != data.machine_name) {
      memcpy(&data, &GRAPHIC::BARCODE_SCANNER::_data, sizeof(data));
      PROJECT_DATA::save(data);   // อัพเดต machine_name ลงใน flashMem ด้วย
    }else{
      memcpy(&data, &GRAPHIC::BARCODE_SCANNER::_data, sizeof(data)); 
    }
    
    GRAPHIC::GAUGE_TRACKING::tracking_no          (data.tracking_no);
    GRAPHIC::GAUGE_TRACKING::tracking_warning_zone(data.tracking_warning_zone);
    GRAPHIC::GAUGE_TRACKING::tracking_max         (data.tracking_max);

    Serial.println("-----------------------------------");
    Serial.println("   [Tracking Sytem] Start Gauge!!!");
    Serial.printf("   [Machine Name] %s\n", data.machine_name);
    Serial.printf("   [Tracking NO.] %s\n", data.tracking_no);    
    Serial.println("-----------------------------------");

#ifdef USE_MQTT// USE_MQTT
    mqtt.mqtt_sub_enable = true;
    Serial.println("[GRAPHIC::BARCODE_SCANNER::barcode Step2] จบ อนุญาติให้ subscribe mqtt_sub_enable:" + String(mqtt.mqtt_sub_enable));

    GRAPHIC::MQTT_F::mqtt_sub();         // จะจำไว้ และเมื่อ mqtt เชื่อมต่อสำเร็จ จะ subscribe ให้อัตโนมัติ
    GRAPHIC::MQTT_F::mqtt_subDown();       // จะจำไว้ และเมื่อ mqtt เชื่อมต่อสำเร็จ จะ subscribe ให้อัตโนมัติ  
#endif         // USE_MQTT
   
    data.state = P_STATE_GAUGE_NORMAL_ZONE;
  }); 
}

/*******************************
 *  gauge tracking
 **********************/
void GRAPHIC::GAUGE_TRACKING::design()
{
  {
    rect_gauge.show(false);
    rect_gauge.size(400, 400);
    rect_gauge.corner_radius(20);
    rect_gauge.color(TFT_WHITE);
    rect_gauge.opa(0);
    cir_gauge.radius(358 / 2);
    cir_gauge.border(1, TFT_BLACK);
    cir_gauge.border_opa(80);
    cir_gauge.color(TFT_WHITE);
    cir_gauge.opa(80);
    cir_gauge.clickable(false);
    lb_gauge.font(LatoBold_num_80, TFT_BLACK);
    lb_gauge.align(ALIGN_CENTER, 0, 5);
    lb_gauge = Num2StringComma(0);

    lb_tracking_no.font(EurostileNextLTPro_30, TFT_COLOR_MIX(TFT_BLACK, TFT_WHITE, 180));
    lb_tracking_no.align(lb_gauge, ALIGN_TOP, 0, -10);
    lb_tracking_no = String(data.tracking_no);

    lb_tracking_warning.show(false);
    lb_tracking_warning.font(EurostileNextWDPro_40, TFT_RED);
    lb_tracking_warning.align(lb_tracking_no, ALIGN_TOP, 0, 5);
    lb_tracking_warning = "DONE!!!";

    for (int i = 0; i < 2; i++)
    {
      ln_gauge_frame[i].parent(cir_gauge);
      ln_gauge_frame[i].thickness(2, TFT_BLACK);
      ln_gauge_frame[i].opa(80);
    }
    ln_gauge_frame[0].setPoints(
        Point(50, lb_gauge.bottomY() + 5),
        Point(cir_gauge.width() - 50, lb_gauge.bottomY() + 5));
    ln_gauge_frame[1].setPoints(
        Point(cir_gauge.centerpoint().x, lb_gauge.bottomY() + 5),
        Point(cir_gauge.centerpoint().x, lb_gauge.bottomY() + 5 + 65));

    lb_tracking_warning_zone_value.font(LatoBold_num_35, TFT_YELLOW); // สีของค่าWARNING = data.tracking_warning_zone_color
    lb_tracking_warning_zone_value.align(ALIGN_TOP_RIGHT, -cir_gauge.centerpoint().x - 10, lb_gauge.bottomY() + 5 + 25);
    lb_tracking_warning_zone_value = Num2StringComma(data.tracking_warning_zone);

    lb_tracking_max_value.font(LatoBold_num_35, TFT_RED); // สีของค่าMAX
    lb_tracking_max_value.align(ALIGN_TOP_LEFT, cir_gauge.centerpoint().x + 10, lb_gauge.bottomY() + 5 + 25);
    lb_tracking_max_value = Num2StringComma(data.tracking_max);

    lb_tracking_max_info = "Max :";
    lb_tracking_max_info.font(prasanmit_25, TFT_COLOR_MIX(TFT_BLACK, TFT_WHITE, 180));
    lb_tracking_max_info.align(lb_tracking_max_value, ALIGN_TOP_LEFT, 0, 5);

    lb_tracking_warning_zone_info = "Warning :";
    lb_tracking_warning_zone_info.font(prasanmit_25, TFT_COLOR_MIX(TFT_BLACK, TFT_WHITE, 180));
    lb_tracking_warning_zone_info.align(ALIGN_TOP_RIGHT, -cir_gauge.centerpoint().x - 20 - 15, lb_tracking_max_info.topY());

    lb_update_info.show(false);
    lb_update_info = "Updated!";
    lb_update_info.font(prasanmit_35, TFT_COLOR_HSV(200, 100, 100));
    lb_update_info.align(ALIGN_BOTTOM, 0, -30);

    arc_gauge_level[NORMAL_ZONE].direction(ARC_CLOCKWISE);
    arc_gauge_level[NORMAL_ZONE].radius(cir_gauge.radius() - 3);
    arc_gauge_level[NORMAL_ZONE].thickness(8);
    arc_gauge_level[NORMAL_ZONE].color(data.tracking_normal_zone_color);
    arc_gauge_level[NORMAL_ZONE].angle_offset(270);
    arc_gauge_level[NORMAL_ZONE].parent(rect_gauge);
    arc_gauge_level[NORMAL_ZONE].align(ALIGN_CENTER);
    arc_gauge_level[NORMAL_ZONE].clickable(false);

    arc_gauge_level[NEAR_MAX_ZONE].direction(ARC_CLOCKWISE);
    arc_gauge_level[NEAR_MAX_ZONE].radius(cir_gauge.radius() - 3);
    arc_gauge_level[NEAR_MAX_ZONE].thickness(8);
    arc_gauge_level[NEAR_MAX_ZONE].color(data.tracking_warning_zone_color);
    arc_gauge_level[NEAR_MAX_ZONE].angle_offset(270);
    arc_gauge_level[NEAR_MAX_ZONE].parent(rect_gauge);
    arc_gauge_level[NEAR_MAX_ZONE].align(ALIGN_CENTER);
    arc_gauge_level[NEAR_MAX_ZONE].clickable(false);

    ln_gauge_near_done_zone[0].thickness(6, data.tracking_warning_zone_color);
    ln_gauge_near_done_zone[0].parent(rect_gauge);

    ln_gauge_near_done_zone[1].thickness(10, TFT_RED);
    ln_gauge_near_done_zone[1].parent(rect_gauge);

    GRAPHIC::GAUGE_TRACKING::redesign_arc();

    cir_gauge_indic.radius(16);
    cir_gauge_indic.color(TFT_WHITE);
    cir_gauge_indic.border(3, TFT_BLACK);
    cir_gauge_indic.center(Point(cir_gauge.radius() - 5, 200, 180, rect_gauge.centerpoint(), CLOCKWISE));
  }

  GRAPHIC::GAUGE_TRACKING::reset();
}

void GRAPHIC::GAUGE_TRACKING::show(bool enable)
{
  rect_gauge.show(enable);
}

void GRAPHIC::GAUGE_TRACKING::tracking_next()
{
  GRAPHIC::GAUGE_TRACKING::tracking_cnt(data.tracking_cnt + 1);
}

void GRAPHIC::GAUGE_TRACKING::tracking_cnt(int tracking_cnt)
{
  if (tracking_cnt == 0)
  {
    data.tracking_cnt = 0;
    lb_gauge = Num2StringComma(data.tracking_cnt);
    cir_gauge.color(TFT_COLOR_MIX(data.tracking_normal_zone_color, TFT_WHITE, 120));
    cir_gauge.opa(255);
    cir_gauge_indic.center(Point(cir_gauge.radius() - 5, 0, 270, rect_gauge.centerpoint(), CLOCKWISE));
    return;
  }

  if (data.tracking_max == 0 || data.tracking_warning_zone == 0)
    return;

  data.tracking_cnt = constrain(tracking_cnt, 0, data.tracking_max);
  cir_gauge_indic.center(Point(cir_gauge.radius() - 5, constrain(map(data.tracking_cnt, 0, data.tracking_max, 0, 360), 0, 359), 270, rect_gauge.centerpoint(), CLOCKWISE));
  lb_gauge = Num2StringComma(data.tracking_cnt);

  if (data.tracking_cnt < data.tracking_warning_zone)
  {
    cir_gauge.color(TFT_COLOR_MIX(data.tracking_normal_zone_color, TFT_WHITE, 120));
    cir_gauge.opa(255);
  }
  else
  {
    cir_gauge.color(TFT_COLOR_MIX(data.tracking_warning_zone_color, TFT_WHITE, 120));
    cir_gauge.opa(255);
  }
}

int GRAPHIC::GAUGE_TRACKING::tracking_cnt()
{
  return data.tracking_cnt;
}

void GRAPHIC::GAUGE_TRACKING::tracking_warning_zone(int tracking_warning_zone)
{
  if (tracking_warning_zone > 0)
  {
    data.tracking_warning_zone = tracking_warning_zone;
    GRAPHIC::GAUGE_TRACKING::redesign_arc();
    GRAPHIC::GAUGE_TRACKING::tracking_cnt(data.tracking_cnt);
  }
  else
  {
    data.tracking_warning_zone = 0;
    GRAPHIC::GAUGE_TRACKING::redesign_arc();
  }
}

int GRAPHIC::GAUGE_TRACKING::tracking_warning_zone()
{
  return data.tracking_warning_zone;
}

void GRAPHIC::GAUGE_TRACKING::tracking_max(int tracking_max)
{
  if (tracking_max > 0)
  {
    data.tracking_max = tracking_max;
    GRAPHIC::GAUGE_TRACKING::redesign_arc();
    GRAPHIC::GAUGE_TRACKING::tracking_cnt(data.tracking_cnt);
  }
  else
  {
    data.tracking_max = 0;
    GRAPHIC::GAUGE_TRACKING::redesign_arc();
  }
}

int GRAPHIC::GAUGE_TRACKING::tracking_max()
{
  return data.tracking_max;
}

void GRAPHIC::GAUGE_TRACKING::tracking_no(String tracking_no)
{
  snprintf(data.tracking_no, sizeof(data.tracking_no), "%s", tracking_no.c_str());
  lb_tracking_no = (tracking_no == "") ? "[ Tracking NO. ]" : data.tracking_no;
}

String GRAPHIC::GAUGE_TRACKING::tracking_no()
{
  return data.tracking_no;
}

void GRAPHIC::GAUGE_TRACKING::info_blinking(bool enable, String into_text, color_t color) // กำนดข้อความ สี การกระพริบ
{
  static SoftTimer timer;
  if (enable)
  {
    lb_tracking_warning.show(true);
    lb_tracking_warning = into_text;
    lb_tracking_warning.color(color);
    timer.setInterval(500, []()
                      { lb_tracking_warning.show(!lb_tracking_warning.show()); });
  }
  else
  {
    timer.del();
    lb_tracking_warning.hidden(true);
  }
}

void GRAPHIC::GAUGE_TRACKING::redesign_arc()
{
  {
    if (data.tracking_max != 0 && data.tracking_warning_zone != 0)
    {
      lb_tracking_max_value = Num2StringComma(data.tracking_max);
      lb_tracking_warning_zone_value = Num2StringComma(data.tracking_warning_zone);
      arc_gauge_level[NORMAL_ZONE].angle(0, map(data.tracking_warning_zone, 0, data.tracking_max, 0, 360));
      arc_gauge_level[NEAR_MAX_ZONE].angle(map(data.tracking_warning_zone, 0, data.tracking_max, 0, 360), 360);
      ln_gauge_near_done_zone[0].setPoints(
          Point(cir_gauge.radius() - 3, map(data.tracking_warning_zone, 0, data.tracking_max, 0, 360), 270, cir_gauge.center(), CLOCKWISE),
          Point(cir_gauge.radius() - 3 - 18, map(data.tracking_warning_zone, 0, data.tracking_max, 0, 360), 270, cir_gauge.center(), CLOCKWISE));
      ln_gauge_near_done_zone[1].setPoints(
          Point(cir_gauge.radius() - 3, 359, 270, cir_gauge.center(), CLOCKWISE),
          Point(cir_gauge.radius() - 3 - 30, 359, 270, cir_gauge.center(), CLOCKWISE));
    }
    else
    {
      lb_tracking_max_value = "       -";
      lb_tracking_warning_zone_value = "-       ";
      arc_gauge_level[NORMAL_ZONE].angle(0, 270);
      arc_gauge_level[NEAR_MAX_ZONE].angle(270, 360);
      ln_gauge_near_done_zone[0].setPoints(
          Point(cir_gauge.radius() - 3, 270, 270, cir_gauge.center(), CLOCKWISE),
          Point(cir_gauge.radius() - 3 - 18, 270, 270, cir_gauge.center(), CLOCKWISE));
      ln_gauge_near_done_zone[1].setPoints(
          Point(cir_gauge.radius() - 3, 359, 270, cir_gauge.center(), CLOCKWISE),
          Point(cir_gauge.radius() - 3 - 30, 359, 270, cir_gauge.center(), CLOCKWISE));
    }
  }
}

void GRAPHIC::GAUGE_TRACKING::reset()
{
  GRAPHIC::GAUGE_TRACKING::info_blinking(false);
  GRAPHIC::GAUGE_TRACKING::tracking_no(""); // ล้าง tracking_no ทั้งกราฟิกและ ใน data.tracking_no
  GRAPHIC::GAUGE_TRACKING::tracking_cnt(0);
  GRAPHIC::GAUGE_TRACKING::tracking_max(-1);
  GRAPHIC::GAUGE_TRACKING::tracking_warning_zone(-1);
}

/*******************************
 *  หน้า lockscreen
 **********************/
void GRAPHIC::APP_LOCKSCREEN::design()
{
  app_lockscreen.show(false);
  app_lockscreen.color(TFT_BLACK);
  lb_lockscreen = "Enter Passcode";
  lb_lockscreen.font(prasanmit_40);
  lb_lockscreen.align(ALIGN_TOP, 0, 10);

  cont_cir_dot_lockscreen.opa(0);
  cont_cir_dot_lockscreen.layout(LAYOUT_ROW_M, 5, 5, 5, 5, 15);
  cont_cir_dot_lockscreen.align(lb_lockscreen, ALIGN_BOTTOM, 0, 5);
  for (int i = 0; i < 6; i++)
  {
    cir_dot_lockscreen[i].radius(10);
    cir_dot_lockscreen[i].border(1, TFT_WHITE);
    cir_dot_lockscreen[i].color(TFT_WHITE);
    cir_dot_lockscreen[i].opa(0);
    cir_dot_lockscreen[i].parent(cont_cir_dot_lockscreen);
  }
  cont_btn_lockscreen.opa(0);
  cont_btn_lockscreen.layout(LAYOUT_PRETTY, 5, 5, 5, 5, 10);
  cont_btn_lockscreen.width(350);
  cont_btn_lockscreen.align(cont_cir_dot_lockscreen, ALIGN_BOTTOM, 0, 10);
  for (int i = 0; i < 10; i++)
  {
    int ii = (i < 9) ? i + 1 : 0;
    btn_lockscreen[ii] = String(ii);
    btn_lockscreen[ii].round_design();
    btn_lockscreen[ii].size(80, 80);
    btn_lockscreen[ii].font(LatoBold_num_50);
    btn_lockscreen[ii].opa(0);
    btn_lockscreen[ii].border(1, TFT_SILVER);
    btn_lockscreen[ii].border_opa(180);
    btn_lockscreen[ii].parent(cont_btn_lockscreen);
    btn_lockscreen[ii].onClicked([](GWIDGET)
                                 {
        int ii = (GButton*)widget-&btn_lockscreen[0];
        if(_passcode_user_input.length()<6) {
          _passcode_user_input += String(ii);
          cir_dot_lockscreen[_passcode_user_input.length()-1].opa(255);
        }

        if( _passcode_user_input.length() == 6){
          // Serial.println(_passcode_system);
          // Serial.println( _passcode_user_input);

          if(_mode == 1 )  {
            GRAPHIC::APP_LOCKSCREEN::_is_matched =  _passcode_system == _passcode_user_input.toInt();
            // Serial.println(_is_matched );
            if(GRAPHIC::APP_LOCKSCREEN::_is_matched){
              lb_lockscreen = "Passcode ถูกต้อง";
              lb_lockscreen.color(TFT_GREEN);
              btn_lockscreen_cancel.show(false);
            }else{
              lb_lockscreen = "Passcode ไม่ถูกต้อง";
              lb_lockscreen.color(TFT_RED);            
            }

            static SoftTimer timer;
            timer.delay(1500,[](){
              if(GRAPHIC::APP_LOCKSCREEN::_is_matched) {
                app_lockscreen.show(false);
                if(GRAPHIC::APP_LOCKSCREEN::_fn_ok != NULL) GRAPHIC::APP_LOCKSCREEN::_fn_ok();
                GRAPHIC::APP_LOCKSCREEN::reset();
              }else{
                GRAPHIC::APP_LOCKSCREEN::clear_graphic();
              }
            });
          }else
          if(_mode == 2){
            if(_first_time_passcode_setting == ""){
              _first_time_passcode_setting =  _passcode_user_input; // ให้จำค่ารอบแรกไว้ แล้วเริ่มใส่ใหม่
              GRAPHIC::APP_LOCKSCREEN::clear_graphic();
              lb_lockscreen = "ใส่ค่า Passcode ให้ตรงครั้งแรกอีกครั้ง";
              lb_lockscreen.color(TFT_CYAN);
              _mode = 2;
            }else{
              static SoftTimer timer;
              if(_first_time_passcode_setting ==  _passcode_user_input){
                data.passcode = _passcode_user_input.toInt();
                PROJECT_DATA::save(data);
                lb_lockscreen = "เปลี่ยน Passcode เป็นค่าใหม่เรียบร้อย";
                lb_lockscreen.color(TFT_BLUE);
                timer.delay(1500,[](){
                  app_lockscreen.show(false);
                  if(GRAPHIC::APP_LOCKSCREEN::_fn_ok != NULL) GRAPHIC::APP_LOCKSCREEN::_fn_ok();
                  GRAPHIC::APP_LOCKSCREEN::reset();
                });
              }else{
                lb_lockscreen = "Passcode ครั้ง 2 ใส่ไม่ตรงครั้งแรก";
                lb_lockscreen.color(TFT_RED);
                timer.delay(1500,[](){
                  app_lockscreen.show(false);
                  if(GRAPHIC::APP_LOCKSCREEN::_fn_ok != NULL) GRAPHIC::APP_LOCKSCREEN::_fn_ok();
                  GRAPHIC::APP_LOCKSCREEN::reset();
                });
              }
            }
          }
        } });
  }

  btn_lockscreen_cancel = "Cancel";
  btn_lockscreen_cancel.opa(0);
  btn_lockscreen_cancel.border(0);
  btn_lockscreen_cancel.style[PRESSED].color(TFT_WHITE, TFT_WHITE);
  btn_lockscreen_cancel.style[PRESSED].opa(20);
  btn_lockscreen_cancel.style[PRESSED].border(0);
  btn_lockscreen_cancel.corner_radius(RADIUS_CIRCLE);
  btn_lockscreen_cancel.align(cont_btn_lockscreen, ALIGN_IN_BOTTOM_RIGHT, 10, -10);
  btn_lockscreen_cancel.onClicked([](GWIDGET)
                                  {
      app_lockscreen.show(false);
      if(GRAPHIC::APP_LOCKSCREEN::_fn_cancel != NULL) {  GRAPHIC::APP_LOCKSCREEN::_fn_cancel(); } });
}

void GRAPHIC::APP_LOCKSCREEN::reset()
{
  GRAPHIC::APP_LOCKSCREEN::clear_graphic();
  _fn_ok = NULL;
  _fn_cancel = NULL;
}

void GRAPHIC::APP_LOCKSCREEN::clear_graphic()
{
  for (int i = 0; i < 6; i++)
  {
    cir_dot_lockscreen[i].opa(0);
  }
  _passcode_user_input = "";
  lb_lockscreen = "Enter Passcode";
  lb_lockscreen.color(TFT_WHITE);
  btn_lockscreen_cancel.show(true);
}

void GRAPHIC::APP_LOCKSCREEN::passcode(int32_t passcode) { _passcode_system = passcode; }
int32_t GRAPHIC::APP_LOCKSCREEN::passcode() { return _passcode_system; }
void GRAPHIC::APP_LOCKSCREEN::onOK(common_cb_t fn_ok) { _fn_ok = fn_ok; }
void GRAPHIC::APP_LOCKSCREEN::onCancel(common_cb_t fn_cancel) { _fn_ok = fn_cancel; }

void GRAPHIC::APP_LOCKSCREEN::mode_block_screen(int32_t passcode, common_cb_t fn_ok, common_cb_t fn_cancel)
{
  if (passcode > 0)
    APP_LOCKSCREEN::passcode(passcode);
  if (fn_ok != NULL)
    APP_LOCKSCREEN::onOK(fn_ok);
  if (fn_cancel != NULL)
    APP_LOCKSCREEN::onCancel(fn_cancel);
  app_lockscreen.show(true);
  app_lockscreen.toForeground();
  GRAPHIC::APP_LOCKSCREEN::clear_graphic();
  _mode = 1; // โหมด block screen
}

void GRAPHIC::APP_LOCKSCREEN::mode_change_passcode()
{
  app_lockscreen.show(true);
  app_lockscreen.toForeground();
  GRAPHIC::APP_LOCKSCREEN::clear_graphic();
  lb_lockscreen = "ตั้งค่า Passcode ค่าใหม่";
  lb_lockscreen.color(TFT_COLOR_HSV(170, 30, 100));
  _mode = 2; // โหมด ตั้งค่า passcode จำลง flashMem
  _first_time_passcode_setting = "";
}

/*******************************
 *  กำหนดให้ MACHINE ON หรือ OFF
 **********************/
void GRAPHIC::MACHINE::ON()
{
  cir_machine.color(TFT_GREEN);
#if USE_MRT_EXI0404_I2C

  MACHINE_ON;
#endif
}

void GRAPHIC::MACHINE::OFF()
{
  cir_machine.color(TFT_RED);
#if USE_MRT_EXI0404_I2C
  MACHINE_OFF;
#endif
}

bool GRAPHIC::MACHINE::isON()
{
}

/*******************************
 *  หน้า Setting/หน้า Admin
 **********************/
void GRAPHIC::PAGE_SETTING::design()
{
  rect_setting.show(false);
  rect_setting.color(TFT_BLACK);
  rect_setting_titlebar.height(60);
  rect_setting_titlebar.align(ALIGN_TOP);
  rect_setting_titlebar.color(TFT_COLOR_HEX(0x202020));
  btn_setting_back = SYMBOL_ARROW_LEFT;
  btn_setting_back.round_design();
  btn_setting_back.size(50, 50);
  btn_setting_back.align(ALIGN_LEFT, 10);
  btn_setting_back.onClicked([](GWIDGET)
                             {
        rect_setting.show(false);
        data.state = data.prev_state; });
  lb_setting_titlebar = "ตั้งค่า";
  lb_setting_titlebar.font(prasanmit_40);
  lb_setting_titlebar.align(btn_setting_back, ALIGN_RIGHT, 20);

  cont_icons.opa(0);
  cont_icons.width(700);
  cont_icons.layout(LAYOUT_GRID, 5, 5, 5, 5, 25);
  btn_setting_passcode.width(140);
  btn_setting_passcode.onClicked([](GWIDGET)
                                 { GRAPHIC::APP_LOCKSCREEN::mode_change_passcode(); });
  btn_setting_barcode_scanner.width(btn_setting_passcode);
  btn_setting_barcode_scanner.onClicked([](GWIDGET)
                                        { GRAPHIC::PAGE_SETTING_BARCODE_SCANNER::show(true); });
  btn_setting_server.width(btn_setting_passcode);
  btn_setting_server.onClicked([](GWIDGET)
                               { GRAPHIC::PAGE_SETTING_SERVER::show(true); });

  btn_setting_io_expander.width(btn_setting_passcode);
  btn_setting_io_expander.onClicked([](GWIDGET)
                                    { GRAPHIC::PAGE_SETTING_IO::show(true); });

  // btn_setting_serialport.width(btn_setting_passcode);
  // btn_setting_serialport.onClicked([](GWIDGET)
  //                                  { GRAPHIC::PAGE_SETTING_SERIALPORT::show(true); });

  btn_setting_setdefault.width(btn_setting_passcode);
  btn_setting_setdefault.onClicked([](GWIDGET)
                                   { 
                                     PROJECT_DATA::loadDefault(data);   // บังคับโหลดจาก default Normal close odd 
                                     PROJECT_DATA::save(data);
                                     PROJECT_DATA::init(data);
                                     GRAPHIC::PAGE_SETTING_SERVER::design();
                                     GRAPHIC::PAGE_SETTING_IO::design(); 
                                    //  GRAPHIC::PAGE_SETTING_SERIALPORT::design();                                        
                              
                                    });               
}

void GRAPHIC::PAGE_SETTING::show(bool enable)
{
  rect_setting.show(enable);
  rect_setting.toForeground();
  data.prev_state = data.state;  //เก็บสถานะก่อนเข้า โหมด แอดมิน
  data.state = P_STATE_ADMIN;    //กำหนด data.state ให้เท่ากับ สถานะโหมด แอดมิน
}

/*******************************
 *  หน้า Barcode Scanner Setting
 ***************************/
void GRAPHIC::PAGE_SETTING_BARCODE_SCANNER::design()
{
  rect_setting_barcode_scanner.show(false);
  rect_setting_barcode_scanner.color(TFT_BLACK);
  rect_setting_barcode_scanner_titlebar.height(60);
  rect_setting_barcode_scanner_titlebar.align(ALIGN_TOP);
  rect_setting_barcode_scanner_titlebar.color(TFT_COLOR_HEX(0x202020));
  btn_setting_barcode_scanner_back = SYMBOL_ARROW_LEFT;
  btn_setting_barcode_scanner_back.round_design();
  btn_setting_barcode_scanner_back.size(50, 50);
  btn_setting_barcode_scanner_back.align(ALIGN_LEFT, 10);
  btn_setting_barcode_scanner_back.onClicked([](GWIDGET)
                                             { rect_setting_barcode_scanner.show(false); });
  lb_setting_barcode_scanner_titlebar = "Barcode Scanner Tester";
  lb_setting_barcode_scanner_titlebar.font(prasanmit_40);
  lb_setting_barcode_scanner_titlebar.align(btn_setting_barcode_scanner_back, ALIGN_RIGHT, 20);

  image_barcode_sample.align(rect_setting_barcode_scanner_titlebar, ALIGN_BOTTOM, 0, 60);
  lb_barcode_scan_test = "[ ทดสอบสแกนบาร์โคด ]";
  lb_barcode_scan_test.font(prasanmit_70, TFT_WHITE);
  lb_barcode_scan_test.align(image_barcode_sample, ALIGN_BOTTOM, 0, 30);

  lb_info = "ตัวอย่างบาร์โคดสำหรับทดสอบสแกน";
  lb_info.color(TFT_SILVER);
  lb_info.align(image_barcode_sample, ALIGN_TOP);
}

void GRAPHIC::PAGE_SETTING_BARCODE_SCANNER::show(bool enable)
{
  rect_setting_barcode_scanner.show(enable);
  rect_setting_barcode_scanner.toForeground();
  data.state = P_STATE_ADMIN;
}

void GRAPHIC::PAGE_SETTING_BARCODE_SCANNER::barcode_sample(String barcode)
{
  GRAPHIC::PAGE_SETTING_BARCODE_SCANNER::lb_barcode_scan_test.show(true);
  GRAPHIC::PAGE_SETTING_BARCODE_SCANNER::lb_barcode_scan_test = barcode;
  static SoftTimer timer;
  timer.delay(3000, []()
              { GRAPHIC::PAGE_SETTING_BARCODE_SCANNER::lb_barcode_scan_test = "[ ทดสอบสแกนบาร์โคด ]"; });
}

/*******************************
 *  หน้า Server Setting
 ***************************/
void GRAPHIC::PAGE_SETTING_SERVER::design()
{
  rect_setting_server.show(false);
  rect_setting_server.color(TFT_BLACK);
  rect_setting_server_titlebar.height(60);
  rect_setting_server_titlebar.align(ALIGN_TOP);
  rect_setting_server_titlebar.color(TFT_COLOR_HEX(0x202020));
  btn_setting_server_back = SYMBOL_ARROW_LEFT;
  btn_setting_server_back.round_design();
  btn_setting_server_back.size(50, 50);
  btn_setting_server_back.align(ALIGN_LEFT, 10);
  btn_setting_server_back.onClicked([](GWIDGET)
                                    {
        rect_setting_server.show(false);
        PROJECT_DATA::save(data);
        PROJECT_DATA::load(data);         // ถ้ามีบน flashMem จะดึงลงมา
        GRAPHIC::web_api_interval_updater(); });

  lb_setting_server_titlebar = "Server Setting";
  lb_setting_server_titlebar.font(prasanmit_40);
  lb_setting_server_titlebar.align(btn_setting_server_back, ALIGN_RIGHT, 20);

  // Server_Name//
  lb_server_name = "Server Name : ";
  lb_server_name.font(prasanmit_40, TFT_WHITE);
  lb_server_name.align(rect_setting_server_titlebar, ALIGN_BOTTOM_LEFT, 10, 20); // ตำแหน่ง balbel lb_server_name x,y
  ta_server_name.width(450);
  ta_server_name.placeholder("Server Name / Server IP");
  ta_server_name.cursor_type(CURSOR_NONE);
  ta_server_name.corner_radius(15);
  ta_server_name.font_color(TFT_BLUE);
  ta_server_name.align(lb_server_name, ALIGN_RIGHT, 100, 10); // ตำแหน่ง balbel ta_server_name x,y
  ta_server_name = String(data.server_name);
  ta_server_name = data.server_name;
  ta_server_name.onPressed([](GWIDGET)
                           { dd_web_api_update_interval.close(); });
  ta_server_name.onValueChanged([](GWIDGET)
                                {
                                  String server_name = ta_server_name.toString();
                                  if( !server_name.startsWith("http") && !server_name.startsWith("HTTP") ){
                                    server_name = String("http://") + server_name;                                    
                                    ta_server_name.text(server_name);  //
                                  }
                                  snprintf(data.server_name, sizeof(data.server_name), "%s", server_name.c_str()); 
                                  #if USE_DEBUG
                                  Serial.print("[PAGE_SETTING_SERVER]Server name:");
                                  Serial.println(data.server_name);
                                  #endif
                                });

  // Server_Port//
  lb_server_port = "Server Port : ";
  lb_server_port.font(prasanmit_40, TFT_WHITE);
  lb_server_port.align(lb_server_name, ALIGN_BOTTOM_LEFT, 0, 15);
  ta_server_port.width(150);
  ta_server_port.placeholder("Server Port");
  ta_server_port.cursor_type(CURSOR_NONE);
  ta_server_port.corner_radius(15);
  ta_server_port.align(ta_server_name, ALIGN_BOTTOM_LEFT, 0, 15);
  ta_server_port.font_color(TFT_BLACK);
  ta_server_port = String(data.server_port);
  ta_server_port.onPressed([](GWIDGET)
                           { dd_web_api_update_interval.close(); });
  ta_server_port.onValueChanged([](GWIDGET)
                                {
                                  int port = ta_server_port.toInt();
                                  data.server_port = (port > 0 && port < 65535) ? port : 8000;
                                  #if USE_DEBUG
                                  Serial.print("[PAGE_SETTING_SERVER]Server port:");
                                  Serial.println(data.server_port);
                                  #endif
                                  // ta_server_port.text(String(data.server_port));  // ใช้ = แล้วตาย เพราะมีการวนมา onValueChanged ซ้ำๆ ใช้เป็น ta.text(..) แทน
                                });

  lb_web_api_update_interval = "Update interval : ";
  lb_web_api_update_interval.font(prasanmit_40, TFT_WHITE);
  lb_web_api_update_interval.align(lb_server_port, ALIGN_BOTTOM_LEFT, 0, 15);

  static String dd_update_interval_options[] = {"1 นาที", "2 นาที", "3 นาที", "4 นาที", "5 นาที", "10 นาที", "15 นาที"};
  dd_web_api_update_interval.width(150);
  dd_web_api_update_interval.options(dd_update_interval_options, 7);
  dd_web_api_update_interval.height_open(200);
  dd_web_api_update_interval.corner_radius(15);
  dd_web_api_update_interval.top(true);
  dd_web_api_update_interval.align(ta_server_port, ALIGN_BOTTOM_LEFT, 0, 15);

  switch (data.web_api_update_interval)
  {
  case 1:
    dd_web_api_update_interval.selected(0);
    break;
  case 2:
    dd_web_api_update_interval.selected(1);
    break;
  case 3:
    dd_web_api_update_interval.selected(2);
    break;
  case 4:
    dd_web_api_update_interval.selected(3);
    break;
  case 5:
    dd_web_api_update_interval.selected(4);
    break;
  case 10:
    dd_web_api_update_interval.selected(5);
    break;
  case 15:
    dd_web_api_update_interval.selected(6);
    break;
  default:
    dd_web_api_update_interval.selected(0);
    break;
  }
  dd_web_api_update_interval.onValueChanged([](GWIDGET)
                                            {
      data.web_api_update_interval  = dd_web_api_update_interval.selected().toInt();
      Serial.println(data.web_api_update_interval); });

  lb_disp_sw_simulation = "Show Simulation : ";
  lb_disp_sw_simulation.font(prasanmit_40, TFT_WHITE);
  lb_disp_sw_simulation.align(lb_web_api_update_interval, ALIGN_BOTTOM_LEFT, 0, 15);

  sw_disp_sw_simulation.OFF();
  sw_disp_sw_simulation.size(100, 50);
  sw_disp_sw_simulation.align(dd_web_api_update_interval, ALIGN_BOTTOM_LEFT, 0, 15);
  sw_disp_sw_simulation.onValueChanged([](GWIDGET)
                                       {
                                        if(sw_disp_sw_simulation.isON()){
                                          #if USE_DEBUG
                                          Serial.println("sw_disp_sw_simulation ON");
                                          #endif
                                          sw_simulation.show(true);
                                          lb_simulation.show(true);
                                          gsw_simulation.show(true);
                                        }
                                        else{
                                          #if USE_DEBUG
                                          Serial.println("sw_disp_sw_simulation OFF");
                                          #endif
                                              if(data.is_simulation_mode){
                                                                                             
                                                sw_simulation.show(false);
                                                gsw_simulation.show(true);
                                                gsw_simulation.color(TFT_GREEN); 
                                                lb_simulation.show(true);
                                              }else{                                           
                                                sw_simulation.show(false);
                                                lb_simulation.show(false);
                                                gsw_simulation.show(false);                                           
                                                gsw_simulation.color(TFT_RED);                                  
                                             
                                        }
                                        } });

  // data.is_disp_sw_simulation ? sw_disp_sw_simulation.ON() : sw_disp_sw_simulation.OFF();
  sw_disp_sw_simulation.event_send(EVENT_VALUE_CHANGED);

  //========================================MQTT_SERVER_PORT==========================================//
  lb_mqtt_server_name = "Mqtt Server Name : ";
  lb_mqtt_server_name.font(prasanmit_40, TFT_WHITE);
  lb_mqtt_server_name.align(lb_disp_sw_simulation, ALIGN_BOTTOM_LEFT, 0, 15);
  ta_mqtt_server_name.width(450);
  ta_mqtt_server_name.placeholder("MqttServer Name / MqttServer IP");
  ta_mqtt_server_name.cursor_type(CURSOR_NONE);
  ta_mqtt_server_name.corner_radius(15);
  ta_mqtt_server_name.font_color(TFT_MAGENTA);
  ta_mqtt_server_name.align(sw_disp_sw_simulation, ALIGN_BOTTOM_LEFT, 0, 15);
  ta_mqtt_server_name = String(data.mqtt_server_name);
  ta_mqtt_server_name = data.mqtt_server_name;
  ta_mqtt_server_name.onPressed([](GWIDGET)
                                { dd_web_api_update_interval.close(); });
  ta_mqtt_server_name.onValueChanged([](GWIDGET)
                                     {
                                        String mqtt_server_name = "";                               
                                        mqtt_server_name = ta_mqtt_server_name.text();                                           
                                        snprintf(data.mqtt_server_name, sizeof(data.mqtt_server_name), "%s", mqtt_server_name.c_str());
                                        #if USE_DEBUG
                                        Serial.print("[PAGE_SETTING_SERVER]Mqtt Server Name: ");
                                        Serial.println(data.mqtt_server_name);
                                        #endif
                                      });

  // MQTT_SERVER_PORT
  lb_mqtt_server_port = "Mqtt Server Port : ";
  lb_mqtt_server_port.font(prasanmit_40, TFT_WHITE);
  lb_mqtt_server_port.align(lb_mqtt_server_name, ALIGN_BOTTOM_LEFT, 0, 15);
  ta_mqtt_server_port.width(150);
  ta_mqtt_server_port.placeholder("Server Port");
  ta_mqtt_server_port.cursor_type(CURSOR_NONE);
  ta_mqtt_server_port.corner_radius(15);
  ta_mqtt_server_port.align(ta_mqtt_server_name, ALIGN_BOTTOM_LEFT, 0, 15);
  ta_mqtt_server_port.font_color(TFT_MAGENTA);
  ta_mqtt_server_port = String(data.mqtt_server_port);
  ta_mqtt_server_port.onPressed([](GWIDGET)
                                { dd_web_api_update_interval.close(); });
  ta_mqtt_server_port.onValueChanged([](GWIDGET)
                                     {
                                       int mqtt_port = ta_mqtt_server_port.toInt();                                  
                                       data.mqtt_server_port = (mqtt_port > 0 && mqtt_port < 65535) ? mqtt_port : 8000;
                                       #if USE_DEBUG
                                       Serial.print("[PAGE_SETTING_SERVER]Mqtt Server Port: ");
                                       Serial.println(data.mqtt_server_port);
                                       // ta_mqtt_server_port.text(String(data.mqtt_server_port));  // ใช้ = แล้วตาย เพราะมีการวนมา onValueChanged ซ้ำๆ ใช้เป็น ta.text(..) แทน
                                       /////////////////////////////////////////////////////////////////////////////////////////////////////
                                       Serial.print("[PAGE_SETTING_SERVER]Service Server name:");Serial.println(data.web_serverservice_name); 
                                       Serial.print("[PAGE_SETTING_SERVER]Service Server Port:");Serial.println(data.web_service_port); 
                                      #endif
                                       /////////////////////////////////////////////////////////////////////////////////////////////////////
                                     });
  //========================================================================================//
}

void GRAPHIC::PAGE_SETTING_SERVER::show(bool enable)
{
  rect_setting_server.show(enable);
  rect_setting_server.toForeground();
  data.state = P_STATE_ADMIN;
}

/*******************************
 *  หน้า IO Setting
 ***************************/
void GRAPHIC::PAGE_SETTING_IO::design()
{
  rect_setting_io.show(false);
  rect_setting_io.color(TFT_BLACK);
  rect_setting_io_titlebar.height(60);
  rect_setting_io_titlebar.align(ALIGN_TOP);
  rect_setting_io_titlebar.color(TFT_COLOR_HEX(0x202020));
  btn_setting_io_back = SYMBOL_ARROW_LEFT;
  btn_setting_io_back.round_design();
  btn_setting_io_back.size(50, 50);
  btn_setting_io_back.align(ALIGN_LEFT, 10);
  btn_setting_io_back.onClicked([](GWIDGET)
                                {
        rect_setting_io.show(false);
        PROJECT_DATA::save(data); });
  lb_setting_io_titlebar = "IO Setting";
  lb_setting_io_titlebar.font(prasanmit_40);
  lb_setting_io_titlebar.align(btn_setting_io_back, ALIGN_RIGHT, 20);
  //////////////////////////////////////////////////////////////////////////////
  cont_io.opa(0);
  cont_io.layout(LAYOUT_ROW_M, 5, 5, 5, 5, 5);
  cont_io.align(rect_setting_io_titlebar, ALIGN_BOTTOM, 0, 30);
  cont_output.opa(0);
  cont_output.layout(LAYOUT_COL_M, 10, 10, 10, 10, 5);
  cont_output.border(2, TFT_RED);
  cont_output.corner_radius(10);
  rect_cont_btn_io_output.size(200, 50);
  // rect_cont_btn_io_output .border(5);
  rect_cont_btn_io_output.opa(0);
  lb_cont_btn_io_output = "IO OUTPUT";
  lb_cont_btn_io_output.font(prasanmit_30, TFT_WHITE);

  cont_btn_io_output.opa(0);
  cont_btn_io_output.layout(LAYOUT_ROW_M, 5, 5, 5, 5, 10);
  cont_btn_io_output.align(rect_setting_io_titlebar, ALIGN_BOTTOM, 0, 40);
  for (int i = 0; i < 4; i++)
  {
    btn_io_output[i] = String("Y") + String(i);
    btn_io_output[i].font(EurostileNextLTPro_40, TFT_BLACK);
    btn_io_output[i].size(80, 80);
    btn_io_output[i].corner_radius(10);
    btn_io_output[i].mode(BUTTON_MODE_TOGGLE);
    btn_io_output[i].style[RELEASED].color(TFT_SILVER, TFT_SILVER);  // OFF color
    btn_io_output[i].style[PRESSED].color(TFT_SILVER, TFT_SILVER);   // OFF color
    btn_io_output[i].style[TOGGLE_RELEASED].color(TFT_RED, TFT_RED); // ON  color
    btn_io_output[i].style[TOGGLE_PRESSED].color(TFT_RED, TFT_RED);  // ON  color
    btn_io_output[i].parent(cont_btn_io_output);
    btn_io_output[i].onClicked([](GWIDGET)
                               {
            int ii = (GButton*) widget - &GRAPHIC::PAGE_SETTING_IO::btn_io_output[0];
            MRT_EXI0404::digitalWrite(ii, btn_io_output[ii].isON()); });
  }
  rect_cont_btn_io_output.width(cont_btn_io_output);

  cont_input.opa(0);
  cont_input.layout(LAYOUT_COL_M, 10, 10, 10, 10, 5);
  cont_input.border(2, TFT_YELLOW);
  cont_input.corner_radius(10);
  rect_cont_btn_io_input.size(200, 50);
  rect_cont_btn_io_input.opa(0);
  lb_cont_btn_io_input = "IO INPUT";
  lb_cont_btn_io_input.font(prasanmit_30, TFT_WHITE);

  cont_btn_io_input.opa(0);
  cont_btn_io_input.layout(LAYOUT_ROW_M, 5, 5, 5, 5, 10);
  cont_btn_io_input.align(rect_setting_io_titlebar, ALIGN_BOTTOM, 0, 40);
  for (int i = 0; i < 4; i++)
  {
    btn_io_input[i] = String("IN") + String(i + 4);
    btn_io_input[i].font(EurostileNextLTPro_40, TFT_BLACK);
    btn_io_input[i].size(80, 80);
    btn_io_input[i].corner_radius(10);
    btn_io_input[i].mode(BUTTON_MODE_TOGGLE);
    btn_io_input[i].style[RELEASED].color(TFT_SILVER, TFT_SILVER);      // OFF color
    btn_io_input[i].style[PRESSED].color(TFT_SILVER, TFT_SILVER);       // OFF color
    btn_io_input[i].style[TOGGLE_RELEASED].color(TFT_GREEN, TFT_GREEN); // ON  color
    btn_io_input[i].style[TOGGLE_PRESSED].color(TFT_GREEN, TFT_GREEN);  // ON  color
    btn_io_input[i].parent(cont_btn_io_input);
    btn_io_input[i].clickable(false);
    // btn_io_input[i].ON();
    // btn_io_input[i].onClicked([](GWIDGET){
    //   int input_pin = 4 +(GButton*) widget - &GRAPHIC::PAGE_SETTING_IO::btn_io_input[0];
    //   Serial.println(input_pin);
    // });
    


  }
  rect_cont_btn_io_input.width(cont_btn_io_input);

  static String io_output_options[] = {"Y0", "Y1", "Y2", "Y3"};
  static String io_input_options[] = {"IN4", "IN5", "IN6", "IN7"};

  lb_pin_output_for_machine = "Machine PIN : ";
  lb_pin_output_for_machine.font(prasanmit_40, TFT_WHITE);
  lb_pin_output_for_machine.align(cont_io, ALIGN_BOTTOM_LEFT, 20, 30);

  dd_pin_output_for_machine.width(80);
  dd_pin_output_for_machine.height_open(150);
  dd_pin_output_for_machine.options(io_output_options, 4);
  dd_pin_output_for_machine.font(prasanmit_40);
  dd_pin_output_for_machine.top(true);
  dd_pin_output_for_machine.align(lb_pin_output_for_machine, ALIGN_RIGHT, 30);
  dd_pin_output_for_machine.onPressed([](GWIDGET)
                                      { dd_pin_output_for_buzzer.close(); });
  dd_pin_output_for_machine.onValueChanged([](GWIDGET)
                                           { data.pin_output_for_machine = dd_pin_output_for_machine.selected_id(); });
  dd_pin_output_for_machine.selected(data.pin_output_for_machine);

  lb_pin_output_for_buzzer = "Buzzer PIN : ";
  lb_pin_output_for_buzzer.font(prasanmit_40, TFT_WHITE);
  lb_pin_output_for_buzzer.align(lb_pin_output_for_machine, ALIGN_BOTTOM_LEFT, 0, 15);

  dd_pin_output_for_buzzer.width(80);
  dd_pin_output_for_buzzer.height_open(100);
  dd_pin_output_for_buzzer.options(io_output_options, 4);
  dd_pin_output_for_buzzer.font(prasanmit_40);
  dd_pin_output_for_buzzer.top(true);
  dd_pin_output_for_buzzer.onPressed([](GWIDGET)
                                     { dd_pin_output_for_machine.close(); });
  dd_pin_output_for_buzzer.onValueChanged([](GWIDGET)
                                          { data.pin_output_for_buzzer = dd_pin_output_for_buzzer.selected_id(); });
  dd_pin_output_for_buzzer.align(dd_pin_output_for_machine, ALIGN_BOTTOM, 0, 20);
  dd_pin_output_for_buzzer.selected(data.pin_output_for_buzzer);

  //-----------------------------------------
  lb_pin_input_for_task_reset = "Task-Reset PIN : ";
  lb_pin_input_for_task_reset.font(prasanmit_40, TFT_WHITE);
  lb_pin_input_for_task_reset.align(cont_input, ALIGN_BOTTOM_LEFT, 10, 30);

  dd_pin_input_for_task_reset.width(100);
  dd_pin_input_for_task_reset.height_open(150);
  dd_pin_input_for_task_reset.options(io_input_options, 4);
  dd_pin_input_for_task_reset.font(prasanmit_40);
  dd_pin_input_for_task_reset.top(true);
  dd_pin_input_for_task_reset.align(lb_pin_input_for_task_reset, ALIGN_RIGHT, 25);
  dd_pin_input_for_task_reset.onPressed([](GWIDGET)
                                        { dd_pin_input_for_tracking_add.close(); });
  dd_pin_input_for_task_reset.onValueChanged([](GWIDGET)
                                             {
                                               data.pin_input_for_task_reset = 4 + dd_pin_input_for_task_reset.selected_id(); // ขา io input เริ่มตั้งแต่ 4..7 ในขณะที่ selected_id เริ่มที่ 0..3 เลยต้อง +4
                                             });
  dd_pin_input_for_task_reset.selected(data.pin_input_for_task_reset - 4); // ขา io input เริ่มตั้งแต่ 4..7 ในขณะที่ selected_id เริ่มที่ 0..3 เลยต้อง -4

  lb_pin_input_for_tracking_add = "Tracking Add PIN : ";
  lb_pin_input_for_tracking_add.font(prasanmit_40, TFT_WHITE);
  lb_pin_input_for_tracking_add.align(lb_pin_input_for_task_reset, ALIGN_BOTTOM_LEFT, 0, 15);

  dd_pin_input_for_tracking_add.width(100);
  dd_pin_input_for_tracking_add.height_open(100);
  dd_pin_input_for_tracking_add.options(io_input_options, 4);
  dd_pin_input_for_tracking_add.font(prasanmit_40);
  dd_pin_input_for_tracking_add.top(true);
  dd_pin_input_for_tracking_add.onPressed([](GWIDGET)
                                          { dd_pin_input_for_task_reset.close(); });
  dd_pin_input_for_tracking_add.onValueChanged([](GWIDGET)
                                               {
                                                 data.pin_input_for_tracking_add = 4 + dd_pin_input_for_tracking_add.selected_id(); // ขา io input เริ่มตั้งแต่ 4..7 ในขณะที่ selected_id เริ่มที่ 0..3 เลยต้อง +4
                                               });
  dd_pin_input_for_tracking_add.align(dd_pin_input_for_task_reset, ALIGN_BOTTOM, 0, 20);
  dd_pin_input_for_tracking_add.selected(data.pin_input_for_tracking_add - 4); // ขา io input เริ่มตั้งแต่ 4..7 ในขณะที่ selected_id เริ่มที่ 0..3 เลยต้อง -4
  lb_warning = "หมายเหตุ : ขา io ทั้ง 4 ให้ตั้งไม่ซ้ำกัน";
  lb_warning.color(TFT_RED);
  lb_warning.align(ALIGN_BOTTOM, 0, -3);
}

void GRAPHIC::PAGE_SETTING_IO::show(bool enable)
{
  rect_setting_io.show(enable);
  rect_setting_io.toForeground();
  data.state = P_STATE_ADMIN;
}

GRect rect_task_reset_alarm;
GLabel lb_task_reset(rect_task_reset_alarm);

void GRAPHIC::page_task_alarm(bool enable) // ปิดหน้า Alram เมื่อ IN4 OFF
{
  if (!rect_task_reset_alarm.isCreated())
  {
    rect_task_reset_alarm.show(false);
    rect_task_reset_alarm.size(GRAPHIC::BARCODE_SCANNER::rect_barcode_scanner);
    rect_task_reset_alarm.shadow(30, TFT_BLACK);
    rect_task_reset_alarm.shadow_offset(6, 6);
    rect_task_reset_alarm.corner_radius(40);
    rect_task_reset_alarm.color(TFT_RED);
    lb_task_reset = "Waitting AutoMode ON or X2 ON";
  }

  static bool _en; // เปิดหน้า Alram เมื่อ IN4 ON
  if (_en != enable)
  {
    if (enable)
    {
      rect_task_reset_alarm.show(enable);
      rect_task_reset_alarm.toForeground();
      GRAPHIC::BARCODE_SCANNER::show(false);
      GRAPHIC::GAUGE_TRACKING::show(false);
      GRAPHIC::MACHINE::OFF();
      GRAPHIC::ALARM::blinking(false, false);
      data.state = P_STATE_TASK_RESET;
    }
    else
    {
      rect_task_reset_alarm.show(enable);
      data.state = P_STATE_STANDBY;
    }
    _en = enable;
  }
}

void GRAPHIC::PAGE_SETTING_IO::y0(bool state)
{
  (state) ? GRAPHIC::PAGE_SETTING_IO::btn_io_output[0].ON() : GRAPHIC::PAGE_SETTING_IO::btn_io_output[0].OFF();
}

void GRAPHIC::PAGE_SETTING_IO::y1(bool state)
{
  (state) ? GRAPHIC::PAGE_SETTING_IO::btn_io_output[1].ON() : GRAPHIC::PAGE_SETTING_IO::btn_io_output[1].OFF();
}

void GRAPHIC::PAGE_SETTING_IO::y2(bool state)
{
  (state) ? GRAPHIC::PAGE_SETTING_IO::btn_io_output[2].ON() : GRAPHIC::PAGE_SETTING_IO::btn_io_output[2].OFF();
}

void GRAPHIC::PAGE_SETTING_IO::y3(bool state)
{
  (state) ? GRAPHIC::PAGE_SETTING_IO::btn_io_output[3].ON() : GRAPHIC::PAGE_SETTING_IO::btn_io_output[3].OFF();
}

void GRAPHIC::PAGE_SETTING_IO::in4(bool state)
{
  (state) ? GRAPHIC::PAGE_SETTING_IO::btn_io_input[0].OFF() : GRAPHIC::PAGE_SETTING_IO::btn_io_input[0].ON();
}

void GRAPHIC::PAGE_SETTING_IO::in5(bool state)
{
  (state) ? GRAPHIC::PAGE_SETTING_IO::btn_io_input[1].OFF() : GRAPHIC::PAGE_SETTING_IO::btn_io_input[1].ON();
}

void GRAPHIC::PAGE_SETTING_IO::in6(bool state)
{
  (state) ? GRAPHIC::PAGE_SETTING_IO::btn_io_input[2].OFF() : GRAPHIC::PAGE_SETTING_IO::btn_io_input[2].ON();
}

void GRAPHIC::PAGE_SETTING_IO::in7(bool state)
{
  (state) ? GRAPHIC::PAGE_SETTING_IO::btn_io_input[3].OFF() : GRAPHIC::PAGE_SETTING_IO::btn_io_input[3].ON();
}

//--------------------------------------------------------------
String Num2StringComma(int num)
{
  // แปลงจำนวนเป็นสตริง
  String str = String(num);

  // หาขนาดของสตริง
  int size = str.length();

  // คั่นทุก 3 หลักด้วยเครื่องหมาย ','
  for (int i = size - 3; i > 0; i -= 3)
  {
    str = str.substring(0, i) + "," + str.substring(i);
  }

  // นำผลลัพธ์มาเก็บในตัวแปร result
  return str;
}
//========================================================================================//


//---------------------------------------MQTT--------------------------------------------////
void GRAPHIC::MQTT_F::mqtt_int(){
  MQTT.unsubscribe_all();
  MQTT.disconnect();
  GRAPHIC::MQTT_F::mqtt_sub();   // จะจำไว้ และเมื่อ mqtt เชื่อมต่อสำเร็จ จะ subscribe ให้อัตโนมัติ
  GRAPHIC::MQTT_F::mqtt_subDown();   // จะจำไว้ และเมื่อ mqtt เชื่อมต่อสำเร็จ จะ subscribe ให้อัตโนมัติ  

}
void GRAPHIC::MQTT_F::mqtt_widget()
{
  // วิตเจ็ต MQTT Status
  lb_mqtt_message.font(prasanmit_20, TFT_BLACK);
  lb_mqtt_message.align(ALIGN_BOTTOM_LEFT, 10, -0);
  lb_mqtt_info.font(prasanmit_20, TFT_VIOLET);
  lb_mqtt_info.align(lb_mqtt_message, ALIGN_TOP_LEFT, 5);
  lb_mqtt_message.show(false);
  lb_mqtt_info.show(false);
#if GRAPHIC_DEBUG
  lb_mqtt_message.show(true);
  lb_mqtt_info.show(true);
#endif

  cir_mqttsub_status.align(cir_machine, ALIGN_LEFT, -10, 0);
  cir_mqttsub_status.radius(40);
  cir_mqttsub_status.color(TFT_VIOLET);
  cir_mqttsub_status.border(2, TFT_VIOLET);
  lb_mqttsub_status.font(prasanmit_30, TFT_WHITE);
}


void GRAPHIC::MQTT_F::mqtt_sub()
{ 
  // Serial.printf("[MQTT] onMessage : %s --> %s\n", topic.c_str(), message.c_str()); 
  MQTT.subscribe(mqtt.MQTT_sub_topic = ("AutoStrokeCounter/" + String(GRAPHIC::BARCODE_SCANNER::_data.tracking_no))); // จะจำไว้ และเมื่อ mqtt เชื่อมต่อสำเร็จ จะ subscribe ให้อัตโนมัติ
  #if USE_DEBUG 
  Serial.println(" [MQTT] MQTT_sub_topic:\t" + String(mqtt.MQTT_sub_topic)) ;
  #endif
}


void GRAPHIC::MQTT_F::mqtt_subDown()
{   
  MQTT.subscribe(mqtt.MQTT_sub_topic = ("AutoStrokeCounterDown/" + String(GRAPHIC::BARCODE_SCANNER::_data.tracking_no))); // จะจำไว้ และเมื่อ mqtt เชื่อมต่อสำเร็จ จะ subscribe ให้อัตโนมัติ
  #if USE_DEBUG    
  Serial.println(" [MQTT] MQTT_subDown_topic:\t" + String(mqtt.MQTT_sub_topic)) ;
  #endif
}
void GRAPHIC::MQTT_F::mqtt_pub()
{
#if USE_IOT

  if (data.is_simulation_mode)
  {
    // โหมด จำลอง UPDATE server ไม่ต้องมีอยู่จริง
    //------------------------------------------------------------------------------------//
    // create jsontoppic object//    
    mqtt.MQTT_pub_topic = ("AutoStrokeCounter/" + String(GRAPHIC::BARCODE_SCANNER::_data.tracking_no));
    #if USE_DEBUG    
    Serial.println("  [MQTT] simulation_mode");
    Serial.printf("  [MQTT] MQTT_pub_topic: %s",(mqtt.MQTT_pub_topic));   
    #endif

    // Create jsonpayload object
    StaticJsonDocument<256> jsonpay;
    JsonObject jsonpayload        = jsonpay.to<JsonObject>();
    jsonpayload["TrackingNo"]     = GRAPHIC::BARCODE_SCANNER::_data.tracking_no;
    jsonpayload["CurrentStroke"]  = data.tracking_cnt;
    jsonpayload["ProgramVersion"] = GRAPHIC::BARCODE_SCANNER::_data.Version;
    jsonpayload["MachineName"]    = GRAPHIC::BARCODE_SCANNER::_data.machine_name;
    mqtt.MQTT_pub_payload         = "";
    serializeJson(jsonpayload, mqtt.MQTT_pub_payload);
    MQTT.publish(mqtt.MQTT_pub_topic, mqtt.MQTT_pub_payload); // mqtt publish String

    //------------------------------------------------------------------------------------//
  }
  else
  {

    // โหมด UPDATE ไปยัง server จริง
    if (!MQTT.connected())
      return;
    //------------------------------------------------------------------------------------//
    // create jsontoppic object  
    #if USE_DEBUG
    Serial.println("  [MQTT] Is_work_mode:");    
    #endif  
    mqtt.MQTT_pub_topic = ("AutoStrokeCounter/" + String(GRAPHIC::BARCODE_SCANNER::_data.tracking_no));    
    // Create jsonpayload object//
    StaticJsonDocument<256> jsonpay;
    JsonObject jsonpayload = jsonpay.to<JsonObject>();

    jsonpayload["TrackingNo"] = GRAPHIC::BARCODE_SCANNER::_data.tracking_no; 
    //=============================================================================================================================//
    if( 0 != GRAPHIC::BARCODE_SCANNER::_data.tracking_cntReturn){       
        if(data.tracking_cnt >= GRAPHIC::BARCODE_SCANNER::_data.tracking_cntReturn){    //ถ้าค่าปัจจุบรร มากกว่าหรือเท่ากับค่าที่รับมาจาก data base ให้ส่งค่าปัจจุบรร
              jsonpayload["CurrentStroke"] = data.tracking_cnt; 
        }else{
              jsonpayload["CurrentStroke"] = data.tracking_cnt = GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt = GRAPHIC::BARCODE_SCANNER::_data.tracking_cntReturn;              
              GRAPHIC::GAUGE_TRACKING::lb_gauge = Num2StringComma(data.tracking_cnt);             
            }
      }else{
          jsonpayload["CurrentStroke"] = data.tracking_cnt;                   
    }
      //=============================================================================================================================// 
    jsonpayload["ProgramVersion"] = GRAPHIC::BARCODE_SCANNER::_data.Version;
    jsonpayload["MachineName"]    = GRAPHIC::BARCODE_SCANNER::_data.machine_name;
    mqtt.MQTT_pub_payload = "";
    serializeJson(jsonpayload, mqtt.MQTT_pub_payload);
    MQTT.publish(mqtt.MQTT_pub_topic, mqtt.MQTT_pub_payload); // mqtt publish String
    #if USE_DEBUG
    Serial.println("  [MQTT] MQTT_pub_topic:\t" + (mqtt.MQTT_pub_topic) + "\t" + (mqtt.MQTT_pub_payload) );
    #endif
    //------------------------------------------------------------------------------------//
  }

#endif
}

void GRAPHIC::MQTT_F::mqtt_onMessage()
{
  MQTT.onMessage([](String topic, String message)
                 {
                      String Resp_topic   = topic.c_str();
                      String Resp_message = message.c_str();
                         
                      #if USE_DEBUG
                      Serial.println(""); 
                      Serial.println("  -------------------1------------------");  
                      // Serial.printf("[MQTT] onMessage :\t%s --> \t%s\n", topic.c_str(), message.c_str());                                          
                      Serial.println("  [MQTT] onMessage :\t""\t " + Resp_topic + "\t " + Resp_message );                     
                      #endif
                      lb_mqtt_message = message;
                      lb_mqtt_info = StringX::printf("MQTT_TOPPIC: %s", topic.c_str()); // แสดงข้อความ ที่ sub เข้ามา ออกหน้าจอ Display                     

                      //***************************************************AutoStrokeCounter**************************************************//    
                      if(Resp_topic == ("AutoStrokeCounter/" + String(GRAPHIC::BARCODE_SCANNER::_data.tracking_no))){  
                      StaticJsonDocument<250> json;
                      auto error = deserializeJson(json, message);
                        if (!error)
                        {
                            #if USE_DEBUG
                            Serial.println("");
                            Serial.println("  -------------------2------------------");
                            Serial.println("  [GRAPHIC][MQTT_F][JSON][RESP-1]");   
                            #endif                  
                            if (json.containsKey("TrackingNo"))
                              snprintf(GRAPHIC::BARCODE_SCANNER::_data.tracking_no, sizeof(GRAPHIC::BARCODE_SCANNER::_data.tracking_no), "%s", json["TrackingNo"].as<const char *>());
                            if (json.containsKey("CurrentStroke"))
                              GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt  = json["CurrentStroke"].as<int>();
                            if (json.containsKey("ProgramVersion"))
                              GRAPHIC::BARCODE_SCANNER::_data.Version       = json["ProgramVersion"].as<String>();
                            if (json.containsKey("MachineName"))
                              snprintf(GRAPHIC::BARCODE_SCANNER::_data.machine_name, sizeof(GRAPHIC::BARCODE_SCANNER::_data.machine_name), "%s", json["MachineName"].as<const char *>());
                              #if USE_DEBUG                               
                              Serial.println("  --------------------------------------");
                              Serial.printf("  [Tracking NO.          ]    %s\n",GRAPHIC::BARCODE_SCANNER::_data.tracking_no);
                              Serial.printf("  [Tracking Count.       ]    %d\n",GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt);
                              Serial.printf("  [Tracking Version.     ]    %s\n",GRAPHIC::BARCODE_SCANNER::_data.Version);
                              Serial.printf("  [Tracking Machine Name.]    %s\n",GRAPHIC::BARCODE_SCANNER::_data.machine_name);  
                              Serial.println("  --------------------------------------");                 
                              #endif 

                              // cir_mqttsub_status.border(4, TFT_VIOLET);
                              // cir_mqttsub_status.color(TFT_GREEN);
                              // lb_mqttsub_status = String(GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt); // แสดง กล่องสถานะ mqtt เป็นสีเขียวเมื่อมีข้อมูล sub เข้ามา หลังจาก 200 ms กลับเป็นสีม่วง
                              // static GTimer timer;
                              // timer.delay(200, []()
                              //       {                                           
                              //         cir_mqttsub_status.border(4, TFT_GREEN);  
                              //         cir_mqttsub_status.color(TFT_VIOLET); 
                              //       }); 
                          }                  
                         
                        } //(if error1)
                          
                      //***************************************************AutoStrokeCounterDown**********************************************//        
                      if(Resp_topic == ("AutoStrokeCounterDown/" + String(GRAPHIC::BARCODE_SCANNER::_data.tracking_no))){                 
                        StaticJsonDocument<250> json;
                        auto error = deserializeJson(json, message);
                        if (!error)
                        { 
                          #if USE_DEBUG
                          Serial.println("");
                          Serial.println("  -------------------3------------------");
                          Serial.println("  [GRAPHIC][MQTT_F][JSON][RESP-2]");
                          #endif
                          if (json.containsKey("CurrentStroke"))
                              GRAPHIC::BARCODE_SCANNER::_data.tracking_cntReturn = json["CurrentStroke"].as<int>();
                              #if USE_DEBUG                              
                              Serial.println("  --------------------------------------");                             
                              Serial.printf("  [Tracking Count_Return.]  %d\n",GRAPHIC::BARCODE_SCANNER::_data.tracking_cntReturn);                
                              Serial.println("  --------------------------------------\n");
                              Serial.println("|____________________________________________________________________________________________________________|");
                              #endif

                              cir_mqttsub_status.border(4, TFT_VIOLET);
                              cir_mqttsub_status.color(TFT_GREEN);
                              lb_mqttsub_status = String(GRAPHIC::BARCODE_SCANNER::_data.tracking_cntReturn); // แสดง กล่องสถานะ mqtt เป็นสีเขียวเมื่อมีข้อมูล sub เข้ามา หลังจาก 200 ms กลับเป็นสีม่วง
                              static GTimer timer;
                              timer.delay(200, []()
                                    {                                           
                                      cir_mqttsub_status.border(4, TFT_GREEN);  
                                      cir_mqttsub_status.color(TFT_VIOLET); 
                                    });

                                  if(GRAPHIC::BARCODE_SCANNER::_data.tracking_cntReturn > GRAPHIC::BARCODE_SCANNER::_data.tracking_cnt){
                                      GRAPHIC::MQTT_F::mqtt_pub();
                                  }                              
                        }//(if error2) 
                      }
                        //***************************************************AutoStrokeCounterDown**********************************************//  
                   }); //end MQTT.onMessage                                    
}//mqtt_onMessage();

void GRAPHIC::MQTT_F::mqtt_connected()
{
#if USE_MQTT
  Serial.println("  [MQTT] GRAPHIC::MQTT_F::mqtt_connected");
  mqtt.mqttconnect_state = true;
  cir_mqtt.color(TFT_GREEN);
#endif
}

void GRAPHIC::MQTT_F::mqtt_disconnected()
{
#if USE_MQTT
  Serial.println("  [MQTT] GRAPHIC::MQTT_F::mqtt_disconnected");
  mqtt.mqttconnect_state = false; 
  cir_mqtt.color(TFT_RED);
#endif
}
////------------------------------------------------------------------------------------------////

void GRAPHIC::mqtt_check()
{
  static SoftTimer timer_check_wifi;
  timer_check_wifi.setInterval(1 * 5000, []()
  {   
    if (data.state == P_STATE_GAUGE_NORMAL_ZONE || data.state == P_STATE_GAUGE_WARNING_ZONE)
    {
      if (!mqtt.wificonnect_state)
      {
        mqtt.wifi_disconnect_cnt++;
#if USE_DEBUG_WIFI
        Serial.printf("[GRAPHIC][Time WIFI_CHECK_DISCONNECT] %d\n", mqtt.wifi_disconnect_cnt);
#endif
        if (mqtt.wifi_disconnect_cnt > 36)
        {
          lb_alarm_message = ALERT_WIFI_OVERTIME;
          rect_alarm_titlebar.show(true);
          GRAPHIC::GAUGE_TRACKING::show(false);
          cir_mqttsub_status.show(false);
          mqtt.wifi_disconnect_cnt_state = true;
        }else if(mqtt.wifi_disconnect_cnt == 24 )
        {
         WiFi.disconnect();
        }else if(mqtt.wifi_disconnect_cnt == 12 ){
         WiFi.disconnect();
        }
      }
      else
      {
        mqtt.wifi_disconnect_cnt_state = false;
        rect_alarm_titlebar.show(false);
        GRAPHIC::GAUGE_TRACKING::show(true);
        cir_mqttsub_status.show(true);
        mqtt.wifi_disconnect_cnt = 0;
#if USE_DEBUG_WIFI
        Serial.printf("[GRAPHIC][Time WIFI_CHECK_DISCONNECT] %d\n", mqtt.wifi_disconnect_cnt);
#endif
      }
      //===============================================================================================//
      if (!mqtt.mqttconnect_state)
      {
        mqtt.mqtt_disconnect_cnt++;
#if USE_DEBUG_WIFI
        Serial.printf("[GRAPHIC][Time MQTT_CHECK_DISCONNECT] %d\n", mqtt.mqtt_disconnect_cnt);
#endif
        if((mqtt.mqtt_disconnect_cnt > 36)  )
        {
          lb_alarm_message = ALERT_MQTT_OVERTIME;
          rect_alarm_titlebar.show(true);
          GRAPHIC::GAUGE_TRACKING::show(false);
          cir_mqttsub_status.show(false);
          mqtt.mqtt_disconnect_cnt_state = true;
        } 
        else if(mqtt.wifi_disconnect_cnt == 24 )
        {
         WiFi.disconnect();
        }else if(mqtt.wifi_disconnect_cnt == 12 ){
         WiFi.disconnect();
        }
      }
      else
      {
        mqtt.mqtt_disconnect_cnt_state = false;
        rect_alarm_titlebar.show(false);
        GRAPHIC::GAUGE_TRACKING::show(true);
        cir_mqttsub_status.show(true);
        mqtt.mqtt_disconnect_cnt = 0;
#if USE_DEBUG_WIFI
        Serial.printf("[GRAPHIC][Time MQTT_CHECK_DISCONNECT] %d\n", mqtt.mqtt_disconnect_cnt);
#endif
      }
    }
  }, false);
}