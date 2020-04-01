/*!
 * @file pollInterrupt.ino
 * @brief Set a pin of IO expansion board to interrupt mode, poll if there is an interrupt occurring on the pins of the port.
 * @n Experiment phenomenon: poll if there is an interrupt occurring on the port group(A, B, A+B), if there is, serial print the pin
 * @n which is interrupted.
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-07-18
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_MCP23017
 */
#include <DFRobot_MCP23017.h>
/*DFRobot_MCP23017 Constructor 
 *Parameter &wire  Wire
 *Parameter addr  I2C address can be selected from 0x20~0x27, the relationship of DIP switch (A2,A1,A0) and I2C address (0x27) 
 *is shown below: 
  * 0  0  1  0  | 0  A2 A1 A0
    0  0  1  0  | 0  1  1  1    0x27
    0  0  1  0  | 0  1  1  0    0x26
    0  0  1  0  | 0  1  0  1    0x25
    0  0  1  0  | 0  1  0  0    0x24
    0  0  1  0  | 0  0  1  1    0x23
    0  0  1  0  | 0  0  1  0    0x22
    0  0  1  0  | 0  0  0  1    0x21
    0  0  1  0  | 0  0  0  0    0x20
 */
DFRobot_MCP23017 mcp(Wire, 0x27);//constructor, change the Level of A2, A1, A0 via DIP switch to revise I2C address within 0x20~0x27.
//DFRobot_MCP23017 mcp;//use default parameter, Wire  0x27(default I2C address)

/*Interrupt service function, prototype void func(int index), index: the number of the pin that is interrupted*/
void func(int index){
  /*pinDescription function is used to convert a pin into string description
  Parameter pin, the available parameter is shown below:
  eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7  eGPA
   0       1      2      3      4      5      6      7
  eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7  eGPB
   8       9      10     11     12     13     14     15
  */
  String description = mcp.pinDescription(/*pin = */index);
  Serial.print(description);
  Serial.println(" Interruption occurs!");
}

void setup() {
  Serial.begin(115200);
  
  /*wait for the chip to be initialized completely, and then exit*/
  while(mcp.begin() != 0){
    Serial.println("Initialization of the chip failed, please confirm that the chip connection is correct!");
    delay(1000);
  }

  /*pinModeInterrupt function is used to set a pin to interrupt mode. The pin will be automatically set to input mode by the function.
  Parameter pin, the available parameter is shown below: 
  eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7  eGPA
   0       1      2      3      4      5      6      7
  eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7  eGPB
   8       9      10     11     12     13     14     15
  Parameter mode, the available is shown below:  
  eLowLevel             eHighLevel             eRising                  eFalling                 eChangeLevel
  Low-level interrupt   High-level interrupt   Rising-edge interrupt    Falling-edge interrupt   Double-edge interrupts 
  Parameter cb Interrupt service function(with parameter)
  Prototype void func(int)
  */
  mcp.pinModeInterrupt(/*p = */mcp.eGPA0, /*mode = */mcp.eChangeLevel, /*cb = */func);//digital pin0(eGPA0), double edge interrupt, generate an interrupt when the status of pin0 changes, INTA output High level.
  mcp.pinModeInterrupt(/*p = */mcp.eGPA1, /*mode = */mcp.eFalling, /*cb = */func);//digital pin1(eGPA1), falling edge interrupt, generate an interrupt when the status of pin 1 changes from High to Low, INTA output High level.
  mcp.pinModeInterrupt(/*p = */mcp.eGPB7, /*mode = */mcp.eRising, /*cb = */func);//digital pin15(eGPB7), rising edge interrupt, generate an interrupt when the status of pin15 changes from Low to High, INTB output High level.
  mcp.pinModeInterrupt(/*p = */mcp.eGPB6, /*mode = */mcp.eHighLevel, /*cb = */func);//digital pin14(eGPB6), high level interrupt, generate an interrupt when the pin 14 is in high level, INTB output High level. 
}

void loop() {
  /*pollInterrupts function is used to poll if an interrupt occurs on a port group
  Parameter group, the available parameter is shown below: (default: eGPIOALL)
   eGPIOA         eGPIOB             eGPIOALL
   Port Group A   Port Group B       Port Group A+B
  */
  mcp.pollInterrupts();
 // delay(1000);
}
