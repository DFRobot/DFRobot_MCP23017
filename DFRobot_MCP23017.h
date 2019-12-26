/*!
 * @file DFRobot_MCP23017.h
 * @brief Define the basic structure of class DFRobot_MCP23017 
 * @n This is a digital I/O expansion board with changeable IIC address. It can be controlled via IIC. 
 * @n The functions of the board are shown below:
 * @n 16-bit input/output port expander with interrupt output;
 * @n Cascadable for up to 8 devices on one bus;
 * @n 25mA sink/source capability per I/O;
 * @n Supports 100kHz, 400kHz and 1.7MHz I2C™Compatible compatible modes
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-07-16
 * @https://github.com/DFRobot/DFRobot_MCP23017
 */
#ifndef __DFROBOT_MCP23017_H
#define __DFROBOT_MCP23017_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Wire.h>

//Define DBG, change 0 to 1 open the DBG, 1 to 0 to close.  
#if 0
#define DBG(...) {Serial.print("["); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

#if 0
#define DBGI(...) {Serial.print("["); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBGI(...)
#endif

#define REG_MCP23017_IODIRA   0x00   //direction register portA, control the direction of the data I/O.
#define REG_MCP23017_IODIRB   0x01   //direction register portB, control the direction of the data I/O. 
#define REG_MCP23017_IPOLA    0x02   //Input polarity registerA, 1 = GPIO register bit will reflect the opposite logic state of the input pin;0 = GPIO register bit will reflect the same logic state of the input pin.
#define REG_MCP23017_IPOLB    0x03   //Input polarity registerB, 1 = GPIO register bit will reflect the opposite logic state of the input pin;0 = GPIO register bit will reflect the same logic state of the input pin.
#define REG_MCP23017_GPINTENA 0x04   //Interrupt on change control registorA, control the interrupt-on change feature for each pin: 1 = Enable GPIO input pin for interrupt-on-change event; 0 = Disable GPIO input pin for interrupt-on-change event.
#define REG_MCP23017_GPINTENB 0x05   //Interrupt on change control registorB, control the interrupt-on change feature for each pin: 1 = Enable GPIO input pin for interrupt-on-change event; 0 = Disable GPIO input pin for interrupt-on-change event
#define REG_MCP23017_DEFVALA  0x06   //Default compare register A for interrupt-on-change.If enabled (via GPINTEN and INTCON) to compare against the DEFVAL register, an opposite value on the associated pin will cause an interrupt to occur.
#define REG_MCP23017_DEFVALB  0x07   //Default compare register B for interrupt-on-change.If enabled (via GPINTEN and INTCON) to compare against the DEFVAL register, an opposite value on the associated pin will cause an interrupt to occur
#define REG_MCP23017_INTCONA  0x08   //Interrupt control register A, 1 = Controls how the associated pin value is compared for interrupt-on-change; 0 = Pin value is compared against the previous pin value.
#define REG_MCP23017_INTCONB  0x09   //Interrupt control register B, 1 = Controls how the associated pin value is compared for interrupt-on-change; 0 = Pin value is compared against the previous pin value.
#define REG_MCP23017_IOCONA   0x0A   //Configuration register A contains several bits for configuring the device.
#define REG_MCP23017_IOCONB   0x0B   //Configuration register B contains several bits for configuring the device.
#define REG_MCP23017_GPPUA    0x0C   //Port A pull-up resistor configuration registerA, control the pull-up resistors for the port pins.
#define REG_MCP23017_GPPUB    0x0D   //Port B pull-up resistor configuration registerB, control the pull-up resistors for the port pins.
#define REG_MCP23017_INTFA    0x0E   //Interrupt flag register A: 1 = Pin caused interrupt; 0 = Interrupt not pending.
#define REG_MCP23017_INTFB    0x0F   //Interrupt flag register B: 1 = Pin caused interrupt; 0 = Interrupt not pending.
#define REG_MCP23017_INTCAPA  0x10   //Interrupt capture register A : the INTCAP register captures the GPIO port value at the time the interrupt occurred. The register will remain unchanged until the interrupt is cleared via a read of INTCAP or GPIO.
#define REG_MCP23017_INTCAPB  0x11   //Interrupt capture register B: the INTCAP register captures the GPIO port value at the time the interrupt occurred. The register will remain unchanged until the interrupt is cleared via a read of INTCAP or GPIO
#define REG_MCP23017_GPIOA    0x12   //Port registerA reflects the value on the port.Reading from this register reads the port. Writing to this register modifies the Output Latch (OLAT) register.
#define REG_MCP23017_GPIOB    0x13   //Port registerB reflects the value on the port.Reading from this register reads the port. Writing to this register modifies the Output Latch (OLAT) register
#define REG_MCP23017_OLATA    0x14   //Oupput latch registerA provides access to the output latches. 
#define REG_MCP23017_OLATB    0x15   //Oupput latch registerB provides access to the output latches.

typedef void(*MCP23017_INT_CB)(int index);

class DFRobot_MCP23017{
public:
  #define ERR_OK             0      //ok
  #define ERR_PIN           -1      //error in pin number 
  #define ERR_DATA_READ     -2      //failed to read data bus
  #define ERR_ADDR          -3      //error in I2C address
  
  typedef enum{
      eGPIOA = 1,  /**< GPIO Group A*/
      eGPIOB = 2,  /**< GPIO Group B*/
      eGPIOALL = 3 /**< GPIO Group A+B*/
  }eGPIOGrout_t;
  
  typedef enum{
      eGPA0 = 0,  /**< PortA, digital pin GPA0*/
      eGPA1,  /**< PortA, digital pin GPA1*/
      eGPA2,  /**< PortA, digital pin GPA2*/
      eGPA3,  /**< PortA, digital pin GPA3*/
      eGPA4,  /**< PortA, digital pin GPA4*/
      eGPA5,  /**< PortA, digital pin GPA5*/
      eGPA6,  /**< PortA, digital pin GPA6*/
      eGPA7,  /**< PortA, digital pin GPA7*/
      eGPB0,  /**< PortB, digital pin GPB0*/
      eGPB1,  /**< PortB, digital pin GPB1*/
      eGPB2,  /**< PortB, digital pin GPB2*/
      eGPB3,  /**< PortB, digital pin GPB3*/
      eGPB4,  /**< PortB, digital pin GPB4*/
      eGPB5,  /**< PortB, digital pin GPB5*/
      eGPB6,  /**< PortB, digital pin GPB6*/
      eGPB7,  /**< PortB, digital pin GPB7*/
      eGPIOTotal
  }ePin_t;
  
  typedef enum{
      eLowLevel = 0,   /**< configure pin interrupt, low-level interrupt */
      eHighLevel,  /**< configure pin interrupt, high-level interrupt */
      eRising,  /**< configure pin interrupt, rising edge interrupt */
      eFalling,  /**< configure pin interrupt, falling edge interrupt */
      eChangeLevel     /**< configure pin interrupt, double edge interrupt*/
  }eInterruptMode_t;

  
  typedef struct {
    ePin_t pin;  /**< digital pin, range 0~15 */
    const char * description;/**< digital pin string description, GPIOA0~GPIOB7 */
  } __attribute__ ((packed)) sPinDescription_t;
  
  typedef struct {
    uint8_t   RESERVE: 1; /**< offset = 0*/
    uint8_t   INTPOL: 1;
    uint8_t   ODR: 1;
    uint8_t   HAEN: 1;
    uint8_t   DISSLW: 1; /**< offset = 4*/
    uint8_t   SEQOP: 1;  /**< offset = 5*/
    uint8_t   MIRROR: 1; /**< offset = 6*/
    uint8_t   BANK: 1;   /**< offset = 7*/
  } __attribute__ ((packed)) sIOCON_t;
  
  typedef struct {
    eInterruptMode_t mode;
    MCP23017_INT_CB cb;
  } __attribute__ ((packed)) sModeCB_t;
  
  
public:
  /**
   * @brief Constructor
   * @param pWire I2C bus pointer object. When calling the function, you may transfer a parameter into it. Defaule as Wire
   * @param addr 8 bits I2C address, range 0x20~0x27. Change A2A1A0 via DIP switch to revise IIC address. When calling the function, 
   * @n the I2C address can be designated. (default: 0x27)
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
  DFRobot_MCP23017(TwoWire &wire = Wire, uint8_t addr = 0x27);
  ~DFRobot_MCP23017();

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
   * @param cb Interrupt service function, needs to be defined and transferred parameter by users. Prototype: void func(int)
   */
  void pinModeInterrupt(ePin_t pin, eInterruptMode_t mode,  MCP23017_INT_CB cb);

  /**
   * @brief Poll if an interrupt occurs on a port group.
   * @param group Port group, it could be all enumeration values included in eGPIOGrout_t,  GPIO GroupA(eGPIOA), 
   * @n GPIO GroupB(eGPIOB) GroupA+B (eGPIOALL).
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
   * @n 如"GPIOA0" "GPIOA1" "GPIOA2" "GPIOA3" "GPIOA4" "GPIOA5" "GPIOA6" "GPIOA7"
   * @n   "GPIOB0" "GPIOB1" "GPIOB2" "GPIOB3" "GPIOB4" "GPIOB5" "GPIOB6" "GPIOB7"
   */
  String pinDescription(ePin_t pin);

  /**
   * @brief Convert pin into string description 
   * @param pin  Pin number, range 0~15
   * @return Return pin description string
   * @n 如"GPIOA0" "GPIOA1" "GPIOA2" "GPIOA3" "GPIOA4" "GPIOA5" "GPIOA6" "GPIOA7"
   * @n   "GPIOB0" "GPIOB1" "GPIOB2" "GPIOB3" "GPIOB4" "GPIOB5" "GPIOB6" "GPIOB7"
   */
  String pinDescription(int pin);

protected:
  /**
   * @brief Set a pin to input mode
   * @param reg Direction register REG_MCP23017_IODIRA or MCP23017_IODIRB
   * @param Move index to the left 
   * @return Return 0 if the setting is successful, otherwise return non-zero.
   */
  int setInput(uint8_t reg, uint8_t index);
  /**
   * @brief Set a pin to output mode
   * @param reg Direction register REG_MCP23017_IODIRA  or MCP23017_IODIRB
   * @param Move index to the left
   * @return Return 0 if the setting is successful, otherwise return non-zero.
   */
  int setOutput(uint8_t reg, uint8_t index);
  /**
   * @brief Set a pin to pull-up mode 
   * @param reg Pull-up register  MCP23017_GPPUA or MCP23017_GPPUB
   * @param Move index to the left
   * @return Return 0 if the setting is successful, otherwise return non-zero.
   */
  int setPullUp(uint8_t reg, uint8_t index);
  /**
   * @brief Set a pin to double edge interrupt 
   * @param index Pin number 
   * @return Return 0 if the setting is successful, otherwise return non-zero.
   */
  int setInterruptModeChangeLevel(uint8_t index);
  /**
   * @brief Set a pin to high-level interrupt 
   * @param index Pin number 
   * @return Return 0 if the setting is successful, otherwise return non-zero.
   */
  int setInterruptModeHighLevel(uint8_t index);
  /**
   * @brief Set a pin to low-level interrupt 
   * @param index  Pin number 
   * @return Return 0 if the setting is successful, otherwise return non-zero.
   */
  int setInterruptModeLowLevel(uint8_t index);
  /**
   * @brief Set a designated(index) bit  of a 8bit data to 0 or 1 
   * @param val 8 bit date
   * @param pin index 
   * @param level  data bit 0 or 1
   * @return Return the revised 8bit data 
   */
  uint8_t updateBit(uint8_t val, uint8_t pin, uint8_t level);
  /**
   * @brief Configure INTA and INTB interrupt signal pin. When an interrupt ocurrs on a pin of portA, INTA output High level. 
   * When an interrupt occurs on a pin of PortB, INTB output High level. 
   */
  void interruptConfig();
  /**
   * @brief I2C address detection
   * @param addr I2C address
   * @return Return o if IIC address is set correctly, otherwise return non-zero. 
   */
  int i2cdetect(uint8_t addr);
  /**
   * @brief Write register function
   * @param reg  register address 8bits
   * @param pBuf Storage cache of the data to be written into 
   * @param size Length of the data to be written into 
  */
  void writeReg(uint8_t reg, const void* pBuf, size_t size);
  /**
   * @brief Read register function
   * @param reg  register address 8bits
   * @param pBuf Storage cache of the data to be read
   * @param size Length of the data to be read
   * @return Return the actually read length, fails to read if return 0.
   */
  uint8_t readReg(uint8_t reg, void* pBuf, size_t size);

private:
  sModeCB_t _cbs[eGPIOTotal];
  static sPinDescription_t _pinDescriptions[eGPIOTotal];
  TwoWire *_pWire;
  uint8_t _addr;
};
#endif
