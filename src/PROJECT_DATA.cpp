#include "02_project.h"

#define FLASHMEM_KEY "DATA"

void PROJECT_DATA::init(project_data_t &data)
{  
  PROJECT_DATA::load(data); // ถ้ามีบน flashMem จะดึงลงมา
}

void PROJECT_DATA::load(project_data_t &data)
{
  if (BlynkGO.flashMem_exists(FLASHMEM_KEY))
  {
    Serial.println("[DATA] โหลดจาก flashMem");
    BlynkGO.flashMem_Object(FLASHMEM_KEY, (void *)&data, sizeof(data));
    Serial.print("[DATA] MagicNo = " + String(data.magicnumber)); Serial.println("\tSizeData = " + String(sizeof(data)));
    if (data.magicnumber != sizeof(data))
    {     
      Serial.println("[PROJECT DATA] data size changed... load default");
      PROJECT_DATA::loadDefault(data);
      PROJECT_DATA::save(data);
    }
    else
    {
      #if USE_GRAPHIC
            GRAPHIC::APP_LOCKSCREEN::passcode(data.passcode);
      #endif
            Serial.println("[DATA] magicnumber = sizeof(data)");
            Serial.println("[DATA] None");
    }
  }
  else
  {
    Serial.println("[DATA] โหลดจาก default แล้วบันทึก (เริ่มต้น)");
    PROJECT_DATA::loadDefault(data);
    PROJECT_DATA::save(data);
  }
}

void PROJECT_DATA::save(project_data_t &data)
{
  // static SoftTimer timer1;
  // timer1.delay(800, [](){   // ถ่วงเวลาเพื่อให้ หากมีภาค GRAPHIC มีการปรับเปลี่ยนแปลง ให้แสดงเสร็จก่อน แล้วสักครุ่ค่อยมามีการบันทึกลง flashMem ที่จะไม่ไปชน แล้วทำให้จอเต้นกระพริบได้
  
  if (BlynkGO.flashMem_exists(FLASHMEM_KEY))
      BlynkGO.flashMem_erase(FLASHMEM_KEY);
      BlynkGO.flashMem(FLASHMEM_KEY, (void *)&data, sizeof(project_data_t));
      Serial.print("[DATA] บันทึกลง flashMem1:\t"); Serial.println("Size of Project data :" + String(sizeof(project_data_t)));
  // });
}

void PROJECT_DATA::loadDefault(project_data_t &data)
{
  Serial.println("[DATA] โหลดจาก default to data."); 
  data.magicnumber                    = sizeof(data);
  data.state                          = P_STATE_SCANNER_STEP1;
  data.prev_state                     = P_STATE_SCANNER_STEP1;
  data.is_simulation_mode             = 0;
  data.is_disp_sw_simulation          = 0;
  
  snprintf(data.machine_name, sizeof(data.machine_name), "%s", DEFAULT_MACHINE_NAME);
  snprintf(data.tracking_no, sizeof(data.tracking_no), "%s", DEFAULT_TRACKING_NUMBER);

  data.passcode                       = DEFAULT_PASSCODE;

  data.tracking_max                   = DEFAULT_TRACKING_MAX;
  data.tracking_warning_zone          = DEFAULT_TRACKING_WARNING_ZONE;
  data.tracking_normal_zone_color     = DEFAULT_TRACKING_NORMAL_ZONE_COLOR;
  data.tracking_warning_zone_color    = DEFAULT_TRACKING_WARNING_ZONE_COLOR;

  snprintf(data.server_name, sizeof(data.server_name), "%s", DEFAULT_SERVER_NAME);
  data.server_port = DEFAULT_SERVER_PORT;  //80
  data.web_api_update_interval = DEFAULT_WEB_API_UPDATE_INTERVAL;

  snprintf(data.mqtt_server_name, sizeof(data.mqtt_server_name), "%s", DEFAULT_MQTT_SERVER_NAME);
  data.mqtt_server_port = DEFAULT_MQTT_SERVER_PORT; //1883

  snprintf(data.web_serverservice_name, sizeof(data.web_serverservice_name), "%s", DEFAULT_WEB_SERVERSERVICE);
  data.web_nodered_port = DEFAULT_NODERED_PORT; //1884
  data.web_service_port = DEFAULT_SERVICE_PORT; //80
  data.web_api_update_interval         = 5; 
  //DATA_INPUT_PIN
  data.pin_input_for_tracking_add      = DEFAULT_PIN_INPUT_FOR_TRACKING_ADD;
  data.pin_input_for_task_reset        = DEFAULT_PIN_INPUT_FOR_TASK_RESET;
  data.pin_input_for_in6               = DEFAULT_PIN_INPUT_FOR_IN6;
  data.pin_input_for_in7               = DEFAULT_PIN_INPUT_FOR_IN7;
  //DATA_OUTPUT_PIN
  data.pin_output_for_buzzer           = DEFAULT_PIN_OUTPUT_FOR_BUZZER;
  data.pin_output_for_machine          = DEFAULT_PIN_OUTPUT_FOR_MACHINE;
  data.pin_output_for_y2               = DEFAULT_PIN_OUTPUT_FOR_Y2;
  data.pin_output_for_y3               = DEFAULT_PIN_OUTPUT_FOR_Y3;
  //DATA_RS232_//RS485
  data.rs232_bauddrate                 = DEFAULT_BAUDRATE_RS232;
  data.rs232_databit                   = DEFAULT_CONFIG_RS232;
  data.rs485_bauddrate                 = DEFAULT_BAUDRATE_RS485;
  data.rs485_databit                   = DEFAULT_CONFIG_RS485;
  
}

String PROJECT_DATA::toJsonString(project_data_t &data)
{
}
