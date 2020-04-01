/*!
 * @file buttonInput.ino
 * @brief Connect a button to the IO expansion board, set a pin of the board(eg: eGPA0) to input mode to detect the button status.
 * @n Experiment phenomenon: connect a button on a pin of the IO board(eg:eGPA0), detect the level of the pin and print out
 * @n the button status on serial port.
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
/*DFRobot_MCP23017 constructor
 *Parameter&wire Wire
 *Parameter addr  I2C address can be select from 0x20~0x27; the relationship of DIP switch(A2, A1, A0) and I2C address(0x27) is
 *shown below:
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
DFRobot_MCP23017 mcp(Wire, /*addr =*/0x27);//constructor, change the Level of A2, A1, A0 via DIP switch to revise the I2C address within 0x20~0x27.
//DFRobot_MCP23017 mcp;//use default parameter, Wire  0x27(default I2C address)

//Prepare: connect a button to a digital pin of the IO expansion board(eg: eGPA0)
void setup() {
  Serial.begin(115200);
  /*wait for the chip to be initialized completely, and then exit*/
  while(mcp.begin() != 0){
    Serial.println("Initialization of the chip failed, please confirm that the chip connection is correct!");
    delay(1000);
  }
  /*pinMode function is used to set the pin mode of module
  Parameter pin, the available parameter is shown below:
  eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7  eGPA
   0       1      2      3      4      5      6      7
  eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7  eGPB
   8       9      10     11     12     13     14     15
  Parameter mode, can be set to: INPUT, OUTPUT, INPUT_PULLUP(internal 100KΩ pull-up resistor)
  */
  mcp.pinMode(/*pin = */mcp.eGPA0, /*mode = */INPUT);
  /*Set all Group GPIOA pins to input*/
  //mcp.pinMode(/*pin = */mcp.eGPA, /*mode = */INPUT);
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
  /*Read  level of Group GPIOA pins*/
  //value = mcp.digitalRead(/*pin = */mcp.eGPA);
  if(value){
      Serial.println("Button press!");
      delay(200);
  }else{
      //Serial.println("Button release!");
  }
}
