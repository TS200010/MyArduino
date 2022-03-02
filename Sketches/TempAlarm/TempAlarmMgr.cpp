#ifndef TEMP_ALARM_MGR_CPP
#define TEMP_ALARM_MGR_CPP

#include <Arduino.h>
#include "max6675.h" 
#include <TimeLib.h>
#include "light_CD74HC4067.h"

#include "Globals.h"
#include "TempAlarmMgr.h"
#include "TriColLED.h"
#include "TempProbe.h"

extern LiquidCrystal lcd;
extern File dataFile;

TempAlarmMgr::TempAlarmMgr() : TimedTask ( millis() ){ }

bool TempAlarmMgr::Start() {
  // TODO Add full set of probes and LED + ambient temp Probe.
  tempProbes[0] = new TempProbe( MAXX_SCK, MAXX_CS, MAXX_SO );
 // CD74HC4067 *mux = new CD74HC4067( MUX_A0, MUX_A1, MUX_A2, MUX_A3 );
  CD74HC4067 mux( MUX_A0, MUX_A1, MUX_A2, MUX_A3 );
  tempProbes[0]->SetMux( &mux, 0 );
  triColLEDs[0] = new TriColLED( LED_RED, LED_GRN, LED_BLU );

  Task *tasks[ ] = { this, triColLEDs[0] };
  TaskScheduler scheduler (tasks, NUM_TASKS(tasks) );
  scheduler.runTasks();  // Never returns
}

void TempAlarmMgr::run( uint32_t now ) {
  SDWriteCnt = 0;
  
// TODO Set all LEDs in here  
  triColLEDs[0]->SetColour( tempProbes[0]->GetColour() );
  triColLEDs[0]->SetSteady( );
    
// Read the temperature(s) and save
  tempProbes[0]->SetTemp();

// Set Associated LED to reflect reading
  triColLEDs[0]->SetState( tempProbes[0]->GetState() );  

// TODO Calculate if state is steady and temp above ambient
// Gas appears to be left ON

// All functional code is finished. From here its debug, display and logging only
// ==============================================================================

#ifdef TEST_VERBOSE
  Serial.print("lastEMA="); Serial.print(tempProbes[0]->GetLastEMA()); Serial.print("  EMA="); Serial.print(tempProbes[0]->GetEMA()); 
  Serial.print("  State="); Serial.print(tempProbes[0]->GetState()); Serial.print("  Sticky=");Serial.println(tempProbes[0]->GetSticky());
#endif // TEST_VERBOSE

#ifdef TEST_VERBOSE
  { colour col = tempProbes[0]->GetColour();
    Serial.print("SetTemp Colour = ");
    Serial.print(col.r); Serial.print(" ");
    Serial.print(col.g); Serial.print(" ");
    Serial.println(col.b);
  };
#endif

#ifdef PLOTTING
// Write to serial port so we can plot them
  Serial.print(tempProbes[0]->GetEMA()); 
  Serial.print(" ");
  Serial.print(tempProbes[0]->GetTemp());
  Serial.println(" ");
#endif // PLOTTING

#ifdef USE_LCD
  // Show values on LCD
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  // Display on LCD
  lcd.setCursor(0, 1);
  if (!isnan( tempProbes[0]->GetEMA() )) { 
    lcd.print( tempProbes[0]->GetEMA() ); 
  } else { 
    lcd.print( "NoVal"); 
  };
  lcd.print( "   " );

  if (!isnan( tempProbes[0]->GetTemp() ) ) {
    lcd.print( tempProbes[0]->GetTemp() );
  } else { 
    lcd.print( "NoVal"); 
  };
  lcd.print( "   " );
#endif  // USE_LCD

#ifdef DATA_LOGGING
  // Print to file 
  if ( SDWriteCnt == 12) {
    SDWriteCnt = 0;
    lcd.setCursor(0,0);
    int mins = minute();
    dataFile.print( minute() );
    dataFile.print(", "); 
    if (!isnan( tempProbes[0]->GetEMA() )) { 
      dataFile.print( tempProbes[0]->GetEMA() ); 
      dataFile.print(", ");  
    } else { 
      dataFile.print( "NoVal, " );
    };
    if (!isnan( tempProbes[0]->GetTemp() ) ) {
      dataFile.println( tempProbes[0]->GetTemp() );
    } else { 
      dataFile.println( "NoVal" ); 
    };
    dataFile.flush();
  } else {
    SDWriteCnt = SDWriteCnt + 1;
  };
#endif // DATA_LOGGING

  incRunTime( readInterval );  
};

#endif
