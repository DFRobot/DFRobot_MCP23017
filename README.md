# DFRobot_MCP23017
This is a 16-bit digital IO expansion board that communicates with main-controller via IIC to read and set Level value of the pins. <br>
The board supports 8 IIC addresses. One main-controller board can be connected with at most 8 modules in parallel to expand 128 IO ports. <br>

## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary

1. Set pin mode: input, output, pull-up input(internal 100KΩ pull-up resistor); <br>
2. Read and set pin Level value; <br>
3. Support 5 interruption modes: high-level interrupt, low-level interrupt, rising edge interrupt, falling edge interrupt, double edge interrupts; <br>
4. Support 2-way interrupt signal output: when an interrupt occurs on a pin of portA, pin INTA output High level, when an interrupt on a pin of PortB, INTB output High level; <br>
5. Polled interrupt: detect if an interrupt occurs on the pins via polled interrupt function, and run the relevant interrupt service function; <br>

## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.

## Methods

```C++
/**
 * @brief Init function
 * @return Return 0 if initialization succeeds, otherwise return non-zero. 
 */
int begin(void);

/**
 * @brief Set the pin mode to  input, output or pull-up input (internal 100KΩ pull-up resistor)
 * @param pin Pin number, it could be all enumeration values (eGPA0-eGPB7/ 0-15) included in ePin_t. 
 * @param mode Mode, it can be set to Input, Output, Pull-up Input (internal 100KΩ pull-up resistor)
 * @return Return 0 if the setting is successful, otherwise return non-zero. 
 */
int pinMode(ePin_t pin, uint8_t mode);

/**
 * @brief Write digtial pin. The pin needs to be set to output mode before writing. 
 * @param pin Pin number, it could be all enumeration values (eGPA0-eGPB7/ 0-15) inlcuded in ePin_t.
 * @param level High level 1 or Low level 0
 * @return Return 0 if the writing is successful, otherwise return non-zero. 
 */
int digitalWrite(ePin_t pin, uint8_t level);

/**
 * @brief Read digital pin. The pin needs to be set to input mode before reading. 
 * @param pin Pin number, it could be all enumeration values (eGPA0-eGPB7/ 0-15) included in ePin_t.
 * @return Return High or Low
 */
int digitalRead(ePin_t pin);

/**
 * @brief Set a pin to interrupt mode 
 * @param pin Pin number, it could be all enumeration values (eGPA0-eGPB7/ 0-15) included in ePin_t.
 * @param mode Interrupt mode: all enumeration values included in eInterruptMode_t.
 * @param cb Interrupt service function, needs to be defined and transferred parameter by users. 
 * @n Prototype: void func(int)
 */
void pinModeInterrupt(ePin_t pin, eInterruptMode_t mode,  MCP23017_INT_CB cb);

/**
 * @brief Poll if an interrupt occurs on a port group. 
 * @param group Port group, it could be all enumeration values included in eGPIOGrout_t, 
 * @n GPIO Group A(eGPIOA), GPIO Group B(eGPIOB) Group A+B (eGPIOALL).
 * @n When setting to eGPIOA，poll if an interrupt occurs on the port group A. 
 * @n When setting to eGPIOB, poll if an interrupt occurs on the port group B. 
 * @n When setting to eGPIOALL, poll if an interrupt occurs on the port group A+B
 * @n None, poll if an interrupt occurs on the all ports of group A and B by default. 
 */
void pollInterrupts(eGPIOGrout_t group=eGPIOALL);

/**
 * @brief Convert pin into string description 
 * @param pin Pin number, it could be all enumeration values (eGPA0-eGPB7/ 0-15) inlcuded in ePin_t.
 * @return Return pin description string 
 * @n Such as "GPIOA0" "GPIOA1" "GPIOA2" "GPIOA3" "GPIOA4" "GPIOA5" "GPIOA6" "GPIOA7"
 * @n   "GPIOB0" "GPIOB1" "GPIOB2" "GPIOB3" "GPIOB4" "GPIOB5" "GPIOB6" "GPIOB7"
 */
String pinDescription(ePin_t pin);

/**
 * @brief Convert pin into string description 
 * @param pin Pin number, range 0~15
 * @return Return pin description string
 * @n Such as "GPIOA0" "GPIOA1" "GPIOA2" "GPIOA3" "GPIOA4" "GPIOA5" "GPIOA6" "GPIOA7"
 * @n   "GPIOB0" "GPIOB1" "GPIOB2" "GPIOB3" "GPIOB4" "GPIOB5" "GPIOB6" "GPIOB7"
 */
String pinDescription(int pin);
```

## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino Uno        |      √       |              |             | 
Mega2560        |      √       |              |             | 
Leonardo        |      √       |              |             | 
ESP32         |      √       |              |             | 
micro:bit        |      √       |              |             | 

## History

- Data 2019-7-18
- Version V1.0

## Credits

Written by(xue.peng@dfrobot.com), 2019. (Welcome to our [website](https://www.dfrobot.com/))





