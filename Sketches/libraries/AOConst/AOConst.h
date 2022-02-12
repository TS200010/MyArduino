
/*
 AOConst.h - Part of the Library for Arduino wrapped as Objects.
 
 It is a thin layer that helps user create correct programs more quickly by providing a framework
 for catching inconsistencies at compile time.
 
 Created by Anthony J Stanners January 20th 2020
     
 Released into the public domain.
 
 This version of the library has been tested with UNO. Should it be useful, contribution are welcome for other models.
*/

/*
 This file declares the key constants and classes representing Arduino data and values.
 
 static const is used instead of #define to allow the compile to perform certain checks.
 
 The convention foo_t is used to represent a type foo.
 
 The convention kFooVar is used to represent a static const named FooVar. (This is akin to #define's being in uppercase).
 
 */

#ifndef AOCONST_H
#define AOCONST_H

#include "Arduino.h"


#define NOT_A_DIGITAL_VALUE (digitalData_t)255



// ALL PINS
// --------
static const uint8_t kInvalidPin = NUM_DIGITAL_PINS+1;

// ANALOG PINS
// -----------
/*
 #ifdef _debug
enum class analogPin_t : uint8_t {};
#else
typedef uint8_t analogPin_t;
#endif
 */

typedef uint8_t analogPin_t;
static const analogPin_t kAnalog0 = (analogPin_t)A0;
static const analogPin_t kAnalog1 = (analogPin_t)A1;
static const analogPin_t kAnalog2 = (analogPin_t)A2;
static const analogPin_t kAnalog3 = (analogPin_t)A3;
static const analogPin_t kAnalog4 = (analogPin_t)A4;
static const analogPin_t kAnalog5 = (analogPin_t)A5;
static const analogPin_t kAnalog6 = (analogPin_t)A6;
static const analogPin_t kAnalog7 = (analogPin_t)A7;


// DIGITAL
// -------
/*
 #ifdef _debug
enum class digitalPin_t : uint8_t {};
#else
typedef uint8_t digitalPin_t;
#endif
 */

typedef uint8_t digitalPin_t;
static const digitalPin_t kDigitalRX = (digitalPin_t)0;
static const digitalPin_t kDigitalTX = (digitalPin_t)1;
static const digitalPin_t kDigital2  = (digitalPin_t)2;
static const digitalPin_t kDigital3  = (digitalPin_t)3;
static const digitalPin_t kDigital4  = (digitalPin_t)4;
static const digitalPin_t kDigital5  = (digitalPin_t)5;
static const digitalPin_t kDigital6  = (digitalPin_t)6;
static const digitalPin_t kDigital7  = (digitalPin_t)7;
static const digitalPin_t kDigital8  = (digitalPin_t)8;
static const digitalPin_t kDigital9  = (digitalPin_t)9;
static const digitalPin_t kDigital10 = (digitalPin_t)10;
static const digitalPin_t kDigital11 = (digitalPin_t)11;
static const digitalPin_t kDigital12 = (digitalPin_t)12;
static const digitalPin_t kDigital13 = (digitalPin_t)13;
// THe following are the Analog pins that can also be used as Digital
static const digitalPin_t kDigital14 = (digitalPin_t)14;
static const digitalPin_t kDigital15 = (digitalPin_t)15;
static const digitalPin_t kDigital16 = (digitalPin_t)16;
static const digitalPin_t kDigital17 = (digitalPin_t)17;
static const digitalPin_t kDigital18 = (digitalPin_t)18;
static const digitalPin_t kDigital19 = (digitalPin_t)19;

enum pinInputMode { kInput=INPUT, kInputPullup=INPUT_PULLUP };

/*
#ifdef _debug
 enum class digitalData_t : bool {};
#else
typedef bool digitalData_t;
#endif
*/

//enum class pinUsedAs { notUsed=0, digitalPin, analogPin, PWMPin, interruptPin, seriaIOPin, SPIPin };

//pinUsedAs pinInUseAs[ 20 ];

typedef bool digitalData_t;
//static const digitalData_t ON           = (digitalData_t) 1;
//static const digitalData_t OFF          = (digitalData_t) 0;
static const bool          kActiveHigh  = true;
static const bool          kActiveLow   = false;
static const bool          kActive      = true;
static const bool          kNotActive   = false;

// PULSE WIDTH MODULATED (PWM) PINS
// --------------------------
/*
#ifdef _debug
enum class pulseWidthModulatedPin_t : uint8_t {};
#else
typedef uint8_t pulseWidthModulatedPin_t;
#endif
*/

typedef uint8_t PWMPin_t;
static const PWMPin_t kPWM3  = (PWMPin_t) 3;
static const PWMPin_t kPWM5  = (PWMPin_t) 5;
static const PWMPin_t kPWM6  = (PWMPin_t) 6;
static const PWMPin_t kPWM9  = (PWMPin_t) 9;
static const PWMPin_t kPWM10 = (PWMPin_t) 10;
static const PWMPin_t kPWM11 = (PWMPin_t) 11;
// typedef PWMPin_t PWMPin_t;

// SERIAL PERIPHERAL INTERFACE (SPI) PINS
// --------------------------------------
typedef uint8_t serialInterfacePin_t;
static const serialInterfacePin_t kSlaveSelect       = SS;
static const serialInterfacePin_t kMasterOutSlaveIn  = MOSI;
static const serialInterfacePin_t kMasterInSlaveOut  = MISO;
static const serialInterfacePin_t kSerialClock       = SCK;
static const serialInterfacePin_t kSerialData        = SDA;

// BUILT-IN LED PIN
// ----------------
//const static uint8_t LED_BUILTIN                      = 13;  Defined in pins_arduino.h
static const digitalPin_t   kLEDBuiltIn                = (digitalPin_t) LED_BUILTIN;

// TIMES FOR TIMERS AND DELAYS
// ---------------------------
typedef uint16_t timeInterval_t;
static const timeInterval_t kOneMilliSec          = 1;
static const timeInterval_t kTenMilliSecs         = 10;
static const timeInterval_t kFiftyMilliSecs       = 50;
static const timeInterval_t kOneHundredMilliSecs  = 50;
static const timeInterval_t kOneSecond            = 1000;
static const timeInterval_t kTwoSeconds           = 2000;
static const timeInterval_t kThreeSeconds         = 3000;
static const timeInterval_t kFiveSeconds          = 5000;
static const timeInterval_t kTenSeconds           = 10000;
static const timeInterval_t kOneMinute            = 60 * kOneSecond;


// PULSE WIDTH MODULATION (PWM) LEVELS
// -----------------------------------
//enum class PWMData_t : int {};
typedef int PWMData_t;
static const PWMData_t kPWMMax = (PWMData_t)255;
static const PWMData_t kPWMMin = (PWMData_t)0;


// Procedural Versions of Pin Commands with type checking (Arduino Objects)
// -----------------------------------------------------------------------
void pinModeAO( PWMPin_t pin, uint8_t mode );
void pinModeAO( digitalPin_t pin, uint8_t mode );
//void analogWriteAO( analogPin_t pin, int val );
void analogWriteAO( PWMPin_t pin, PWMData_t val );
void digitalWriteAO( digitalPin_t pin, digitalData_t val );

#endif // AOCONST_H
