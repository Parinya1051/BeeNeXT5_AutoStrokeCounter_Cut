#include <Adafruit_MCP23008.h>
#include "02_project.h"


// [MRT_EXI0404]
// I2C IO Expander  0-3 OUTPUT, 4-7 INPUT
// https://uhandbook.com/blog/EXI-0404-arduino-sample.php
// Download link
// https://u.pcloud.link/publink/show?code=kZDhvTVZhgOBwYM4XPFL6JVtDTbMcLKwlL5y


Adafruit_MCP23008 mcp;

uint8_t Output_State = 0x00;

void MRT_EXI0404::init(){
  mcp.begin(0x20);   // บน Wire  

  // Define output pin 
  mcp.pinMode(0, OUTPUT);
  mcp.pinMode(1, OUTPUT);
  mcp.pinMode(2, OUTPUT);
  mcp.pinMode(3, OUTPUT);

  // จำลองทดสอบ
  // static SoftTimer timer_io_test;
  // timer_io_test.setInterval(1000,[](){
  //   static int8_t i=-1;
  //   if(++i >= 4) i=0;
  //   MRT_EXI0404::reset();
  //   MRT_EXI0404::digitalWrite(i, HIGH);
  //   // MRT_EXI0404::digitalWrite(Y3, HIGH);
  // });
}

void MRT_EXI0404::reset(){
  Output_State = 0x00;
  mcp.writeGPIO(Output_State);

#if USE_GRAPHIC
  GRAPHIC::PAGE_SETTING_IO::y0(LOW);
  GRAPHIC::PAGE_SETTING_IO::y1(LOW);
  GRAPHIC::PAGE_SETTING_IO::y2(LOW);
  GRAPHIC::PAGE_SETTING_IO::y3(LOW);
#endif

}

void MRT_EXI0404::printBinary() {
  for (int i = 7; i >= 0; --i)   Serial.print(bitRead(Output_State, i));
  Serial.println(); // ขึ้นบรรทัดใหม่
}

// pin 0-3  : OUTPUT
void MRT_EXI0404::digitalWrite(uint8_t pin , bool state){
  if (pin >= 4)  return;

  bitWrite(Output_State, pin, state);
  mcp.writeGPIO(Output_State);

#if USE_GRAPHIC
  switch(pin){
    case 0:   GRAPHIC::PAGE_SETTING_IO::y0(state); break;
    case 1:   GRAPHIC::PAGE_SETTING_IO::y1(state); break;
    case 2:   GRAPHIC::PAGE_SETTING_IO::y2(state); break;
    case 3:   GRAPHIC::PAGE_SETTING_IO::y3(state); break;
  }
#endif

}

// pin 4-7  : INPUT
int8_t MRT_EXI0404::digitalRead(uint8_t pin) {
  if( pin < 4) return -1;
  uint8_t Input_State = mcp.readGPIO() & 0xF0;

  int8_t state = bitRead(Input_State, pin);  // 1 or 0

#if USE_GRAPHIC
  switch(pin){
    case 4:   GRAPHIC::PAGE_SETTING_IO::in4(state); break;
    case 5:   GRAPHIC::PAGE_SETTING_IO::in5(state); break;
    case 6:   GRAPHIC::PAGE_SETTING_IO::in6(state); break;
    case 7:   GRAPHIC::PAGE_SETTING_IO::in7(state); break;
  }
#endif

  return state;
}
