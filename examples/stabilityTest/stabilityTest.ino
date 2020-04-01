/*!
 * @file stabilityTest.ino
 * @brief A Comprehensive demo to test the functions of IO expansion board: input, output, interrupt.
 * @n Set the pin eGPB1 on port groupB to output mode, connect a LED to the pin, name it as LEDB1.
 * @n Set the pin eGPA7 on port groupA to output mode, connect a LED to the pin, name it as LEDA7.
 * @n Set the pin eGPA0 on port groupA to input mode, connect a button to the pin, name it as ButtonA0.
 * @n Set the pin eGPA1 on port groupA to falling edge interrupt, connect a button to the pin, name it as ButtonA1. 
 * @n Connect INTA interrupted signal pin to the external interrupt 0 of main-controller. (Take UNO as example)
 * @n Set the pin eGPB7 on port groupB to rising edge interrupt, connect a button to the pin, name it as ButtonB7.
 * @n Connect INTB interrupted signal pin to the external interrupt 1 of main-controller. (Take UNO as example here)
 * @n When detected ButtonA0 is pressed, LEDB1 turn on, release LEDB1 off
 * @n When detected an interrupt occurred on port groupA, LEDA7 turn on
 * @n When detected an interrupt occurred on port groupB, LEDA7 turn off 
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
 *Parameter addr  I2C address can be selected from 0x20~0x27, the relationship of DIP switch (A2,A1,A0) and I2C address(0x27) 
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
DFRobot_MCP23017 mcp(Wire, 0x27);//constructor, change the Level of A2, A1, A0 via DIP switch to revise the I2C address within 0x20~0x27.
//DFRobot_MCP23017 mcp;//use default parameter, Wire  0x27(default I2C address)

/*Interrupt service function, prototype void func(int index), index represents the number of the pin that is interrupted*/
void gpioA1(int index){
  /*pinDescription function is used to convert a pin into string description
  arameter pin, the available parameter is shown below:
  eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7  eGPA
   0       1      2      3      4      5      6      7
  eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7  eGPB
   8       9      10     11     12     13     14     15
  */
  String description = mcp.pinDescription(/*pin = */index);
  Serial.print(description);
  Serial.println(" Interruption occurs!");
  Serial.println("LEDA7 turn on!");
  mcp.digitalWrite(/*pin = */mcp.eGPA7, /*level = */HIGH);
}
void gpioB7(int index){
  String description = mcp.pinDescription(/*pin = */index);
  Serial.print(description);
  Serial.println(" Interruption occurs!");
  Serial.println("LEDA7 turn off!");
  mcp.digitalWrite(/*pin = */mcp.eGPA7, /*level = */LOW);
}

bool intFlagA = false;//INTA interrupt sign
bool intFlagB = false;//INTB interrupt sign

void setup() {
  Serial.begin(115200);
  
  /*wait for the chip to be initialized completely, and then exit*/
  while(mcp.begin() != 0){
    Serial.println("Initialization of the chip failed, please confirm that the chip connection is correct!");
    delay(1000);
  }
  #ifdef ARDUINO_ARCH_MPYTHON 
  pinMode(P0, INPUT);//use mPython external interrupt, connect INTA to pin 0 of mPython.
  pinMode(P1, INPUT);//use mPyhtin external interrupt, connect INTB to pin 1 of mPython.
  #else
  pinMode(2, INPUT);//use UNO external interrupt 0
  pinMode(3, INPUT);//use UNO external interrupt 1
  #endif
  
  /*pinMode function is used to set pin mode of module
  Parameter pin, the available parameter is shown below:
  eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7  eGPA
   0       1      2      3      4      5      6      7
  eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7  eGPB
   8       9      10     11     12     13     14     15
  Parameter mode, can be set to INPUT, OUTPUT, INPUT_PULLUP (internal 100KΩ pull-up resistor)
  */
  mcp.pinMode(/*pin = */mcp.eGPA7, /*mode = */OUTPUT);
  mcp.pinMode(/*pin = */mcp.eGPB1, /*mode = */OUTPUT);
  mcp.pinMode(/*pin = */mcp.eGPA0, /*mode = */INPUT);

  /*pinModeInterrupt function is used to set a pin to interrupt mode. The pin will be automatically set to input mode by this function.  
  Parameter pin, the available parameter is showm below:
  eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7  eGPA
   0       1      2      3      4      5      6      7
  eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7  eGPB
   8       9      10     11     12     13     14     15
  Parameter mode, the available parameter is shown below:
  eLowLevel             eHighLevel             eRising                   eFalling                   eChangeLevel
 Low-level interrupt    High-level interrupt   Rising edge interrupt     Falling edge interrupt     Double edge interrupts 
  Parameter cb interrupt service function(with parameter)
  Prototype void func(int)
  */
  mcp.pinModeInterrupt(/*p = */mcp.eGPA1, /*mode = */mcp.eFalling, /*cb = */gpioA1);//digital pin1(eGPA1), falling edge interrupt, generate an interrupt when the status of pin 1 changes from High to Low, INTA output High level.
  mcp.pinModeInterrupt(/*p = */mcp.eGPB7, /*mode = */mcp.eRising, /*cb = */gpioB7);//digital pin15(eGPB7),  rising edge interrupt, generate an interrupt when the status of pin15 changes from Low to High, INTB output High level.
  #ifdef ARDUINO_ARCH_MPYTHON  //
  /* mPython Interrupt Pin vs Interrupt NO
   * -------------------------------------------------------------------------------------
   * |                    |  DigitalPin  |        P0~P20 can be used as external interrupt             |
   * |    mPython            |--------------------------------------------------------------|
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
  /* microbit Interrupt Pin vs Interrupt NO
   * ---------------------------------------------------------------------------------------------------------------
   * |                                                   |  DigitalPin  |    P0~P20 can be used as external interrupt            |
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
  /*digitalRead function is used to read the Level of a digital pin. The pin needs to be set to input mode before using this function. 
  Parameter pin, the available parameter is shown below:
  eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7  eGPA
   0       1      2      3      4      5      6      7
  eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7  eGPB
   8       9      10     11     12     13     14     15
  */
  uint8_t value = mcp.digitalRead(/*pin = */mcp.eGPA0);
  if(value){
      /*digitalWrite function is used to make the pin output HIGH or LOW. The pin needs to be set to output mode before using this function.
      Designate a pin on the IO expansion board; parameter pin, the available parameter is shown below:
      eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7
       0      1       2      3      4      5      6      7
      eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7
        8      9     10     11      12     13    14     15
      */
      mcp.digitalWrite(/*pin = */mcp.eGPB1, /*level = */HIGH);
      delay(200);
  }else{
      mcp.digitalWrite(/*pin = */mcp.eGPB1, /*level = */LOW);
  }
  if(intFlagA){
    intFlagA = false;
    /*pollInterrupts function is used to poll if an interrupt occurs on a port group
    Parameter group, the available parameter is shown below: (default value: eGPIOALL)：
     eGPIOA        eGPIOB         eGPIOALL
     Port group A  Port groupB    Port groupA+B
    */
    mcp.pollInterrupts(/*group = */mcp.eGPIOA);
  }
  if(intFlagB){
    intFlagB = false;
    mcp.pollInterrupts(/*group = */mcp.eGPIOB);
  }
}
