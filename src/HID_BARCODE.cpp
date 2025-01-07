#include <CH9350_USBKEY.h>
#include "02_project.h"

USB9350_KeyMouse HID; // Create an Instance for Keyboard/ Mouse Decoder

enum
{
  BARCODE_STATE_STANDBY,
  BARCODE_STATE_READING
};

barcode_cb_t _func_cb = NULL;
String _barcode = "";
int _x, _y, _z, _Button;

static SoftTimer timer_hid;

void HID_BARCODE::init(barcode_cb_t func)
{
  _func_cb = func;
  // Create Option Serialport Config
  // static int rs232_baudrate_ar[] = {9600, 19200, 38400, 57600, 115200};
  // static int rs232_data_ar[] = {SERIAL_7N1, SERIAL_7N2, SERIAL_7O1, SERIAL_7O2, SERIAL_7E1, SERIAL_7E2, SERIAL_8N1, SERIAL_8N2, SERIAL_8O1, SERIAL_8O2, SERIAL_8E1, SERIAL_8E2};
  // Serial_Barcode.begin((rs232_baudrate_ar[data.rs232_bauddrate]), rs232_data_ar[data.rs232_databit], HID_BARCODE_RX_17, HID_BARCODE_TX_18); // Parinya
  Serial1.begin(115200, SERIAL_8N1, HID_BARCODE_RX_17, HID_BARCODE_TX_18);//Old Config HID //Parinya
  HID.begin(Serial1);

  timer_hid.setInterval(0, []()
                        {
    static uint8_t _barcode_state = BARCODE_STATE_STANDBY;
    int key = HID.GetKey();
    switch(_barcode_state){
      case BARCODE_STATE_STANDBY:
        if((key>=0x20) and (key<=0x7e)){
          _barcode = (char) key;
          _barcode_state = BARCODE_STATE_READING;
        }
        break;
      case BARCODE_STATE_READING:
        if((key>=0x20) and (key<=0x7e)){
          _barcode += (char) key;
        }else if( key == 0x0D){
          _func_cb(_barcode);
          _barcode = "";
        }
        break;
    } });

  Serial.println("[HID] inited");
}
