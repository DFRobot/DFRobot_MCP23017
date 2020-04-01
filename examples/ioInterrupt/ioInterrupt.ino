/*!
 * @file ioInterrupt.ino
 * @brief IO interrupt, set a pin of a port group(A or B) IO to interrupt mode. When an interrupt occurs on the related port group,
 * @n pin INTA(group A) or INTB(group B) will output a High level.
 * @n INTA and INTB are used to detect if an interrupt occurs on the pin of port eGPA and eGPB respectively; connect pin INTA and INTB 
 * @n to main-controller's external interrupt 0 and 1 respectively.
 * @n Experiment phenomenon: when the signal change of pin INTA or INTB is detected by main-board, the related interrupt service
 * @n function will be executed to print out which pin was interrupted on serial port.
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @eGPAte  2019-07-18
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_MCP23017
 */

#include <DFRobot_MCP23017.h>
/*DFRobot_MCP23017 constructor
 *Parameter &wire Wire
 *Parameter addr  I2C address can be selected from 0x20~0x27; the relationship of the DIP switch(A2, A1, A0) and I2C address(0x27) 
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
DFRobot_MCP23017 mcp(Wire, /*addr =*/0x27);//constructor, change the Level of A2, A1, A0 via DIP switch to revise I2C address within 0x20~0x27.
//DFRobot_MCP23017 mcp;//use default parameter, Wire  0x27(default I2C address)

//Connect 2 buttons to IO expansion board, one to a pin of port eGPA(eg: eGPA0), the other to a pin of port eGPB(eg: eGPB0)
//Connect INTA to the external interrupt pin0 of UNO, INTB to external interrupt pin1 of UNO.

bool intFlagA = false;//INTA interrupt sign
bool intFlagB = false;//INTB interrupt sign

/*Interrupt service function, prototype void func(int index), index represents the pin which is interrupted*/
void gpa0CB(int index){
  /*pinDescription function is used to convert a pin into string description
  Parameter pin, the available parameter is shown below:
  eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7  eGPA
   0       1      2      3      4      5      6      7
  eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7  eGPB
   8       9      10     11     12     13     14     15
  */
  String description = mcp.pinDescription(index);
  Serial.print(description);Serial.println(" Interruption occurs!");
}

void gpb7CB(int index){
  String description = mcp.pinDescription(index);
  Serial.print(description);Serial.println(" Interruption occurs!");
}

void setup() {
  Serial.begin(115200);
  #ifdef ARDUINO_ARCH_MPYTHON 
  pinMode(P0, INPUT);//use mPython external interrupt, connect INTA to pin 0 of mPython.
  pinMode(P1, INPUT);//use mPython external interrupt, connect INTB to pin 1 of mPython.
  #else
  pinMode(2, INPUT);//use UNO external interrupt 0
  pinMode(3, INPUT);//use UNO external interrupt 1
  #endif

  /*wait for the chip to be initialized completely, and then exit*/
  while(mcp.begin() != 0){
    Serial.println("Initialization of the chip failed, please confirm that the chip connection is correct!");
    delay(1000);
  }
  /*pinModeInterrupt function is used to set pin to interrupt mode, and the pin will be automatically set to input mode.
  Parameter pin, the available parameter is showm below:
  eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7  eGPA
   0       1      2      3      4      5      6      7
  eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7  eGPB
   8       9      10     11     12     13     14     15
  Parameter mode, the available parameter is shown below:
  eLowLevel              eHighLevel              eRising                eFalling                  eChangeLevel
  Low-level interrupt    High-level interrupt    Rising edge interrupt  Falling edge interrupt    Double edge interrupts 
  Parameter cb interrupt service function(with parameter)
  Prototype void func(int)
  */
  mcp.pinModeInterrupt(/*pin = */mcp.eGPA0, /*mode = */mcp.eHighLevel, /*cb = */gpa0CB);//digital pin 0(eGPA0), interrupt in High level. Generate an interrupt when pin 0 is in High level state.INTA output High level.
  mcp.pinModeInterrupt(/*pin = */mcp.eGPB7, /*mode = */mcp.eChangeLevel, /*cb = */gpb7CB);//digital pin 15(eGPB7), double edge interrupts. Generate an interrupt when the status of Pin 15 changes. INTB output High level.

  #ifdef ARDUINO_ARCH_MPYTHON // 
  /* mPython Interrupt Pin vs Interrupt NO
   * -------------------------------------------------------------------------------------
   * |                    |  DigitalPin  |        P0~P20 can be used as external interrupt|
   * |    mPython           |--------------------------------------------------------------|
   * |                    | Interrupt No |  use digitalPinToInterrupt(Pn) to query interrupt number     |
   * |-----------------------------------------------------------------------------------|
   */
  attachInterrupt(digitalPinToInterrupt(P0)/*query Interrupt NO of P0*/,notifyA,RISING);//Enable the external interrupt of mPython P0; rising edge trigger; connect INTA to P0
  attachInterrupt(digitalPinToInterrupt(P1)/*query Interrupt NO of P1*/,notifyB,RISING);//Enable the external interrupt of mPython P1; rising edge trigger; connect INTB to P1
  #else
  /* Main-board of AVR series    Interrupt Pin vs Interrupt NO
   * ---------------------------------------------------------------------------------------
   * |                                        |  DigitalPin  | 2  | 3  |                   |
   * |    Uno, Nano, Mini, other 328-based    |--------------------------------------------|
   * |                                        | Interrupt No | 0  | 1  |                   |
   * |-------------------------------------------------------------------------------------|
   * |                                        |    Pin       | 2  | 3  | 21 | 20 | 19 | 18 |
   * |               Mega2560                 |--------------------------------------------|
   * |                                        | Interrupt No | 0  | 1  | 2  | 3  | 4  | 5  |
   * |-------------------------------------------------------------------------------------|
   * |                                        |    Pin       | 3  | 2  | 0  | 1  | 7  |    |
   * |    Leonardo, other 32u4-based          |--------------------------------------------|
   * |                                        | Interrupt No | 0  | 1  | 2  | 3  | 4  |    |
   * |--------------------------------------------------------------------------------------
   */
  /* microbit  Interrupt Pin vs Interrupt NO
   * ---------------------------------------------------------------------------------------------------------------
   * |                                                   |  DigitalPin  |    P0~P20 can be used as external interrupt           |
   * |                  microbit                         |---------------------------------------------------------|
   * |(when used as external interrupt, do not need to set it to input mode via pinMode)  | Interrupt No | Interrupt NO is pin value, for instance, the Interrupt NO of P0 is 0, P1 is 1. |
   * |-------------------------------------------------------------------------------------------------------------|
   */
  attachInterrupt(/*Interrupt NO*/0,notifyA,RISING);//Enable external interrupt 0, connect INTA to the main-controller's digital pin: UNO(2),Mega2560(2),Leonardo(3),microbit(P0)
  attachInterrupt(/*Interrupt NO*/1,notifyB,RISING);//Enable external interrupt 1, connect INTB to the main-controller's digital pin: UNO(3),Mega2560(3),Leonardo(2),microbit(P1)
  #endif
}
/*Interrupt service function*/
void notifyA(){
  intFlagA = true;
}
void notifyB(){
  intFlagB = true;
}

void loop() {
  if(intFlagA){
    intFlagA = false;
    /*pollInterrupts function is used to poll if an interrupt occurs on a port group 
    parameter group, the available parameter is shown below: (default value: eGPIOALL)：
     eGPIOA        eGPIOB         eGPIOALL
     Port groupA  Port groupB   Port groupA+B
    */
    mcp.pollInterrupts(/*group = */mcp.eGPIOA);
  }
  if(intFlagB){
    intFlagB = false;
    mcp.pollInterrupts(/*group = */mcp.eGPIOB);
  }
}
