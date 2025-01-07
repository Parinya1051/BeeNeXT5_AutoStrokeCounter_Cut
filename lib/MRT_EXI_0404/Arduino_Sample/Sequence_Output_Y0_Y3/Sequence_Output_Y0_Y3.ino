/*
  Project     Sequence_Output_Y0_Y3
  Description Step sequence output relay Y0-Y3

  Programmer  Ukrit  Tantasutanon

  Hardware
    EXI-0404  MCP23008 , 4 Input / 4 Output expansion I/O
              Address 0 : A2-A0 = 000 => 0x20 (default)
              Address 1 : A2-A0 = 001 => 0x21
              ...              
              Address 7 : A2-A0 = 111 => 0x27

  Library
    Adafruit MCP23008 library  
********************************************************************
  MICRO RESEARCH TECHNOLOGY CO.,LTD. , https://micro-research.co.th
********************************************************************/

#include <Wire.h>
#include "Adafruit_MCP23008.h"

Adafruit_MCP23008 mcp;

char Output_State = 0x00;

void setup() {  
  // Use default address 0
  mcp.begin(0x20);              

  // Define output pin 
  mcp.pinMode(0, OUTPUT);
  mcp.pinMode(1, OUTPUT);
  mcp.pinMode(2, OUTPUT);
  mcp.pinMode(3, OUTPUT);
}

void loop() {
  // Check data boundary and initial value
  if ((Output_State & 0x0F)==0)
  { Output_State=1;
  }else {
    Output_State<<=1; 
  }

  // Update data to output
  mcp.writeGPIO(Output_State);
  delay(1000);
}