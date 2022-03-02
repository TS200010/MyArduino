#ifndef GLOBALS_H
#define GLOBALS_H

#include <LiquidCrystal.h>
#include <SD.h>
#include "TempProbe.h"

#define DATA_LOGGING
#define PLOTTING
#define TEST_VERBOSE
#define SHOW_LED_STARTUP
#define USE_LCD
// #define TEST_TEMP_COLOURS

// Task Scheduler will break if you change this
static const int g_numSensorsAndLEDs = 5;

// LED PINS
// ======== 
static const int LED_RED  = 6;
static const int LED_GRN  = 5;
static const int LED_BLU  = 3;

// LCD PINS
// ========
static const int LCD_EN   = A1;  // EN = Enable (ie CS)
static const int LCD_RS   = A2;  // RS = Register Select
static const int LCD_D4   = A3;  // D4 = DATA 4
static const int LCD_D5   = 4;   // D5 = DATA 5
static const int LCD_D6   = 7;   // D6 = DATA 6
static const int LCD_D7   = 2;   // D7 = Data 7

// DATA LOGGING PINS
// =================
static const int SD_CS    = 10; // CS   = Chip Select
static const int SD_MOSI  = 11; // MOSI = Master Out Slave In
static const int SD_MISO  = 12; // MOSI = Master In Slave Out
static const int SD_SCK   = 0 ; // SCK  = Serial Clock

// SENSOR PINS
// ===========
static const int MAXX_SO   = 8;   // SO  = Serial Out
static const int MAXX_CS   = 9;   // CS  = chip select 
static const int MAXX_SCK  = A0;  // SCK = Serial Clock pin

// MULTIPLEXOR PINS
// ================
static const int MUX_A0 = A4;
static const int MUX_A1 = A5;
static const int MUX_A2 = 13;
static const int MUX_A3 = 9;


#endif
