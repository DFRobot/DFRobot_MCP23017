/*!
 * @file ledOutput.ino
 * @brief Set a pin of IO expansion board(eg:eGPA7) to output mode, and output High/Low.
 * @n Experiment phenomenon: the LED connected to the pin of IO board(eg:eGPA7) repeatedly lights up for 1s and turns off 1s.
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-07-18
 * @url https://github.com/DFRobot/DFRobot_MCP23017
 */

#include <DFRobot_MCP23017.h>
/*DFRobot_MCP23017 Constructor
 *Parameter &wire  Wire
 *Parameter addr  I2C address can be selected from 0x20~0x27, the relationship of DIP switch (A2,A1,A0) and I2C address (0x27)is shown below：
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
DFRobot_MCP23017 mcp(Wire, /*addr =*/0x27);//constructor, change the Level of A2, A1, A0 via DIP switch to revise I2C address within 0x20~0x27
//DFRobot_MCP23017 mcp;//use default parameter, Wire  0x27(default I2C address)

//Prepare: connect the LED to a digital pin of IO expansion board(eg:eGPA7)
void setup(void)
{
  Serial.begin(115200);
  /*wait for the chip to be initialized completely, and then exit*/
  while(mcp.begin() != 0){
    Serial.println("Initialization of the chip failed, please confirm that the chip connection is correct!");
    delay(1000);
  }
  /*pinMode function is used to set the pin mode of the module
  Parameter pin, the available parameter is shown below:
  eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7  eGPA
   0       1      2      3      4      5      6      7
  eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7  eGPB
   8       9      10     11     12     13     14     15
  Parameter mode, can be set to: INPUT, OUTPUT, INPUT_PULLUP mode (internal 100KΩ pull-up resistor)
  */
  mcp.pinMode(/*pin = */mcp.eGPA7, /*mode = */OUTPUT);
  /*Set all Group GPIOA pins to output*/
  //mcp.pinMode(/*pin = */mcp.eGPA, /*mode = */OUTPUT);
}

void loop(void)
{
  Serial.println("Pin output high level!");
  /*digitalWrite function is used to make the pin output HIGH or LOW. The pin needs to be set to output mode before using this function.
  Designate a pin on the IO expansion board; parameter pin, the available parameter is shown below:
  eGPA0  eGPA1  eGPA2  eGPA3  eGPA4  eGPA5  eGPA6  eGPA7  eGPA
   0       1      2      3      4      5      6      7
  eGPB0  eGPB1  eGPB2  eGPB3  eGPB4  eGPB5  eGPB6  eGPB7  eGPB
   8       9      10     11     12     13     14     15
  */
  mcp.digitalWrite(/*pin = */mcp.eGPA7, /*level = */HIGH);
  /*Set GPIOIA0-GPIOIA3 to low and GPIOIA4-GPIOIA7 to high*/
  //mcp.digitalWrite(/*pin = */mcp.eGPA, /*Port Value = */0xF0);
  delay(1000);
  Serial.println("Pin output low level!");
  mcp.digitalWrite(/*pin = */mcp.eGPA7, /*level = */LOW);
  /*Set GPIOIA0-GPIOIA3 to high and GPIOIA4-GPIOIA7 to low*/
  //mcp.digitalWrite(/*pin = */mcp.eGPA, /*Port Value = */0x0F);
  delay(1000);
}
