#ifndef GLOBALS_H
#define GLOBALS_H

#include <LiquidCrystal.h>
#include <SD.h>
#include <MemoryFree.h>

#include "TempProbe.h"


#define DATA_LOGGING
#define PLOTTING
//#define TEST_VERBOSE
#define SHOW_LED_STARTUP
#define USE_LCD
//#define TEST_TEMP_COLOURS
#define TEST_MESSAGE_QUEUE
//#define TEST_GREEN_RED_PCT_COLOUR

// Task Scheduler will break if you change these
static const int g_numLEDs          = 5;
static const int g_numSensors       = 5; // Does not include Ambient Temp Sensor

// Probe read interval
static const int g_readInterval     = 2000;

// LED Timings
static const int g_LEDFlashTime     = 1000;
static const int g_LEDPulseInterval = 200;

// Temperature range of interest
static const int g_tMin             = 25; // Shown as Green - below this temp gas is certainly off
static const int g_tMax             = 35; // Shown as Red - above this temp gas is certainly alight

// ISR Constants
static const int g_debounceTime     = 250; 

// BUTTON PINS
// ===========
static const uint8_t USER_BUTTON_PIN = 2;

// LED PINS
// ======== 
static const int LED_BRIGHTNESS_PIN = A0;

// LED CHANNELS
// ============
static const int LED1_CHAN = 0;
static const int LED2_CHAN = 3;
static const int LED3_CHAN = 6;
static const int LED4_CHAN = 9;
static const int LED5_CHAN = 12;
static const int NO_LED_CHAN = 255;

// LCD PINS
// ========
static const int LCD_EN   = 22;  // EN = Enable (ie CS)
static const int LCD_RS   = 23;  // RS = Register Select
static const int LCD_D4   = 24;  // D4 = DATA 4
static const int LCD_D5   = 25;  // D5 = DATA 5
static const int LCD_D6   = 27;  // D6 = DATA 6
static const int LCD_D7   = 26;  // D7 = DATA 7

// SD (DATA LOGGING) PINS
// ======================
// (Note SD uses SDI Interface pins)
static const int SD_CS    = 53;  // CS   = Chip Select
static const int SD_SCK   = 52;  // SCK  = Serial Clock
static const int SD_MOSI  = 51;  // MOSI = Master Out Slave In
static const int SD_MISO  = 30;  // MOSI = Master In Slave Out

// SENSOR PINS
// ===========
static const int MAXX_SO   = 29;  // SO  = Serial Out PURPLE
static const int MAXX_CS   = 33;  // CS  = chip select WHITE
static const int MAXX_SCK  = 31;  // SCK = Serial Clock pin YELLOW

// MULTIPLEXOR PINS
// ================
static const int MUX_A0    = 34;  // A0 = Address Line 0
static const int MUX_A1    = 32;  // A1 = Address Line 1
static const int MUX_A2    = 30;  // A2 = Address Line 2
static const int MUX_A3    = 28;  // A3 = Address Line 3



#endif
