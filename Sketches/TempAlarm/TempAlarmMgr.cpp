#ifndef TEMP_ALARM_MGR_CPP
#define TEMP_ALARM_MGR_CPP

#include <Arduino.h>
#include "max6675.h" 
#include <TimeLib.h>
#include "light_CD74HC4067.h"
#include <PCA9685.h>

#include "Globals.h"
#include "TempAlarmMgr.h"
#include "TriColLED.h"
#include "TempProbe.h"

extern LiquidCrystal lcd;
extern File dataFile;
extern bool userButtonInterrupt;
extern void UserButtonISR();

TempAlarmMgr::TempAlarmMgr() : TimedTask ( millis() ){ }

bool TempAlarmMgr::Start() {

  // Create the VIEW (triColLEDs, LED Brightness, LCD, and User Button Interrupt Handler)
  // ===============
  PCA9685 pwmController;
  pwmController.resetDevices();       // Resets all PCA9685 devices on i2c line
  pwmController.init();               // Initializes module using default totem-pole driver mode, and default disabled phase balancer
  pwmController.setPWMFrequency(100); // Set PWM freq to 100Hz (default is 200Hz, supports 24Hz to 1526Hz)
  for (int i=0; i<g_numLEDs; i++){
    triColLEDs[i] = new TriColLED( i*3, &pwmController ); // LED has three colours so in groups of three
  };

  // LED Brightness
  pinMode( LED_BRIGHTNESS_PIN, INPUT );

  // LCD
  LCDWnds[ 0 ] = new CLCDWnd( 0,0, &lcd );
  LCDWnds[ 1 ] = new CLCDWnd( 6,0, &lcd );
  LCDWnds[ 2 ] = new CLCDWnd( 12,0, &lcd );
  LCDWnds[ 3 ] = new CLCDWnd( 0,1, &lcd );
  LCDWnds[ 4 ] = new CLCDWnd( 12,1, &lcd );
  LCDWnds[ 5 ] = new CLCDWnd( 6,1, &lcd ); // Ambient Temperature Sensor

  // User Button Interrupt Handler
  pinMode( USER_BUTTON_PIN, INPUT_PULLUP );
  attachInterrupt( digitalPinToInterrupt( USER_BUTTON_PIN ), UserButtonISR, CHANGE );

  // Create the MODEL (TempProbes and Data Logging)
  // ================
  CD74HC4067 mux( MUX_A0, MUX_A1, MUX_A2, MUX_A3 );
  for (int i=0; i<g_numSensors; i++){
    tempProbes[i] = new TempProbe( MAXX_SCK, MAXX_CS, MAXX_SO );
    tempProbes[i]->SetMux( &mux, i*2 );   // Channels 0, 2, 4, 6, 8, 10 are used
  };
  ambTempProbe = new TempProbe( MAXX_SCK, MAXX_CS, MAXX_SO );
  ambTempProbe->SetMux( &mux, g_numSensors*2 ); // 


// TODO WHAT IS THIS DOING HERE ************************************
  SDWriteCnt = 0;
// TODO WHAT IS THIS DOING HERE ************************************


  // Kick off all the tasks
  // ======================
  Task *tasks[ ] = { this, triColLEDs[0], triColLEDs[1], triColLEDs[2], triColLEDs[3], triColLEDs[4] };
  TaskScheduler scheduler (tasks, NUM_TASKS(tasks) );
  scheduler.runTasks();  // Never returns
}

void TempAlarmMgr::run( uint32_t now ) {

// This is the CONTROLLER
// ======================
// The contoller is a message loop processor. Each time it is called, it processes the oldest message from its queue

// Has the User Button been pressed?
if ( userButtonInterrupt ) {
  userButtonInterrupt = false;       // Clear Interrupt
  Serial.println("Interrupt Processed");
  triColLEDs[0]->SetColour(g_red);
}

// Read the temperature(s) and save
for (int i=0; i<g_numSensors; i++ ) {
  tempProbes[i]->SetTemp( );
};
ambTempProbe->SetTemp();

// TODO Every minute store the temperature and state in a circular buffer.
// time is unsigned long 

// TODO Calculate if state is steady and temp above ambient
// Gas appears to be left ON
for (int i=0; i<g_numSensors; i++ ) {
  tempProbes[i]->HasGasBeenLeftOn( );
};

// Set Associated LED to reflect reading
for (int i=0; i<g_numLEDs; i++ ) {
  triColLEDs[i]->SetState( tempProbes[i]->GetState() );  
  triColLEDs[i]->SetColour( tempProbes[i]->GetColour() );  
};


// All functional code is finished. From here its debug, display and logging only
// ==============================================================================

#ifdef TEST_VERBOSE
for (int i=0; i<2; i++ ) {
  Serial.print("lastEMA="); Serial.print(tempProbes[i]->GetLastEMA()); Serial.print("  EMA="); Serial.print(tempProbes[i]->GetEMA()); 
  Serial.print("  State="); Serial.print(tempProbes[i]->GetState()); Serial.print("  Sticky=");Serial.println(tempProbes[i]->GetSticky());
  { colour col = tempProbes[0]->GetColour();
    Serial.print("SetTemp Colour = ");
    Serial.print(col.r); Serial.print(" ");
    Serial.print(col.g); Serial.print(" ");
    Serial.println(col.b);
  };
};
#endif

#ifdef PLOTTING
for (int i=0; i<2; i++ ) {
// Write to serial port so we can plot them
  Serial.print(tempProbes[i]->GetEMA()); 
  Serial.print(" ");
  Serial.print(tempProbes[i]->GetTemp());
  Serial.println(" ");
};
#endif // PLOTTING

#ifdef USE_LCD
  lcd.clear();
  for (int i=0; i<g_numSensors; i++ ){
    LCDWnds[i]->print( tempProbes[i]->GetTemp() );
  };
  LCDWnds[ g_numSensors ]->print( ambTempProbe->GetTemp() );
#endif  // USE_LCD

#ifdef DATA_LOGGING

#define LogTemp( tp ) \
  if (!isnan( tp->GetEMA() )) { \
      dataFile.print( tp->GetEMA() ); \
      dataFile.print(", ");  \
    } else { \
      dataFile.print( "NoVal, " );\
    };\
    if (!isnan( tp->GetTemp() ) ) {\
      dataFile.print( tp->GetTemp() );\
      dataFile.print(", ");  \
    } else { \
      dataFile.print( "NoVal," ); \
    };\

  // Print to file 
  if ( SDWriteCnt == 12) {
    SDWriteCnt = 0;
    lcd.setCursor(0,0);
    int mins = minute();
    dataFile.print( minute() );
    dataFile.print(", "); 
    for (int i=0; i<g_numSensors; i++ ) {
      LogTemp( tempProbes[i] )
    };
    LogTemp( ambTempProbe );
    dataFile.println("");
    dataFile.flush();
  } else {
    SDWriteCnt = SDWriteCnt + 1;
  };
#endif // DATA_LOGGING

  incRunTime( g_readInterval );  
};

#endif
