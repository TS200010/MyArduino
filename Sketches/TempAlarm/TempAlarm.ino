/*

 TEMPERATURE ALARM
 =================
 
*/

/*
 * Initial Circuit Before adding MUX
 * =================================
 * LCD D7   pin 14 to digital pin 2
 * LED Blue        to digital pin 3
 * LCD D6   pin 13 to digital pin 4
 * LED Green       to digital pin 5
 * LED Red         to digital pin 6
 * LCD D5   pin 12 to Digital pin 7
 * MAX SO          to digital pin 8 
 * MAX CS          to digital pin 9
 * SD  CS          to digital pin 10
 * SD MOSI         to digital pin 11
 * SD MISO         to digital pin 12
 * SD SCK          to digital pin 13

 * MAX SCK         to digital pin A0
 * LCD En   pin 6  to digital pin A1
 * LCD RS   pin 4  to digital pin A2
 * LCD D4   pin 11 to digital pin A3

 * LCD R/W pin 5 to ground
 * LCD VSS pin 1 to ground
 * LCD VCC pin 2 to 5V
 * 10K potentiometer VR1:
 * VR1 ends to +5V and ground
 * LCD VO  pin 3 to VR1 wiper
 
 * After adding Multiplexor
 * ========================

 * See constants in globals.h
 
*/


#include "AOUtils.h"
#include "math.h"
#include "LiquidCrystal.h"
#include "MemoryFree.h"
#include <SPI.h>
#include <SD.h>
#include <TimeLib.h>
#include "Task.h"
#include "TaskScheduler.h"

#include "Globals.h"
#include "Colour.h"
#include "TriColLED.h"
#include "TempProbe.h"
#include "TempAlarmMgr.h"
#include "TempAlarmTests.h"


// Global variables
File dataFile;
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
volatile bool userButtonInterrupt = false;
unsigned long buttonTime = 0;
unsigned long lastButtonTime = 0;

void UserButtonISR(){
  Serial.println("ISR Entered");
  delay (100000 );
  buttonTime = millis();
  if (buttonTime-lastButtonTime > g_debounceTime ){
    // TODO ISR Code Here
     Serial.println("Interrupt Created");
    userButtonInterrupt = true;
    lastButtonTime = buttonTime;
  };
};

#ifdef DATA_LOGGING
void const StartDataLogging(){
  if (!SD.begin(SD_CS)) { Serial.println("SD init failed"); };
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) { Serial.println("dataFile OK"); };  
}
#endif // DATA_LOGGING

#ifdef USE_LCD
void const StartLCD(){
  lcd.print( "Hello world" );
  delay(1000);
// set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("Temp Alarm(v1.3)");
}
#endif // USE_LCD

void const ShowStartUpLEDs(){
  PCA9685 pwmController;
  pwmController.resetDevices();       // Resets all PCA9685 devices on i2c line
  pwmController.init();               // Initializes module using default totem-pole driver mode, and default disabled phase balancer
  pwmController.setPWMFrequency(100); // Set PWM freq to 100Hz (default is 200Hz, supports 24Hz to 1526Hz)
  pwmController.setChannelPWM( 0, 64<<4); // Set PWM to 128/255, shifted into 4096-land
  delay( 100 );

  #ifdef TEST_VERBOSE  
    Serial.print("PCA9685 PWM Controller should display 2048 -> ");
    Serial.println(pwmController.getChannelPWM(0)); // Should output 2048, which is 128 << 4
  #endif

// TriColLED tcl{ LED1_CHAN, &pwmController };
  TriColLED tcl[ g_numLEDs ]= { 
    TriColLED( LED1_CHAN, &pwmController ),
    TriColLED( LED2_CHAN, &pwmController ),
    TriColLED( LED3_CHAN, &pwmController ),
    TriColLED( LED4_CHAN, &pwmController ),
    TriColLED( LED5_CHAN, &pwmController ) 
  };

  // Cycle through the colours so we know the LED(s) are working correctly
  colour rainbow [6] = {g_red, g_green, g_blue, g_yellow, g_cyan, g_magenta }; 
  for (int i=0; i<=5; i++) {
    for (int j=0; j<g_numLEDs; j++ ){
      tcl[j].SetColour( rainbow[i] );
      tcl[j].ShowLED( );
    };
    delay (1000 );
  };  
 // tcl[0].SetColour( g_black );
  delay(1000);
 /*
  // ... and also display Green to Red and back
  for (int i=0; i<=99; i++ ){
    tcl.SetColour( TempProbe::GreenRedPctToColour( i ) );
    tcl.ShowLED(  ); 
    delay( 20 );   
  };
  for (int i=99; i>=0; i-- ){
    tcl.SetColour( TempProbe::GreenRedPctToColour( i ) ); 
     tcl.ShowLED( ); 
    delay( 20 );   
  };    
  tcl.SetColour( g_black );
  */
};  // End ShowStartUpLEDs()


// SETUP
// =====
void setup() { 
  Serial.begin(9600);
  Serial.println(F("TempAlarm Started"));
  Serial.print(F("Free RAM = ")); 
  Serial.println(freeMemory());

#ifdef USE_LCD
  StartLCD();
#endif // USE_LCD

#ifdef DATA_LOGGING
  StartDataLogging();
#endif // DATA_LOGGING

#ifdef SHOW_LED_STARTUP
  ShowStartUpLEDs();
#endif // SHOW_LED_STARTUP

// TESTS ALL PLACED HERE
// =====================
    
#ifdef TEST_GREEN_RED_PCT_COLOUR
  Test_GreenRedPctToColour();
#endif

#ifdef TEST_TEMP_COLOURS
  Test_TempColours();
#endif

#ifdef TEST_MESSAGE_QUEUE
  Test_MessageQueue();
#endif
  
}; // End Setup


// Main Loop
// =========
void loop()
{
  // This is where it all happens...
  TempAlarmMgr *pTempAlarmMgr = new TempAlarmMgr( );
  pTempAlarmMgr->Start(); // Never Returns

};

 
