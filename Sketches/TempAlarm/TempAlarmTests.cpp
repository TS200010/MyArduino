#ifndef TEMP_ALARM_TESTS_CPP
#define TEMP_ALARM_TESTS_CPP

#include "Globals.h"
#include "TempProbe.h"
#include "TriColLED.h"

void Test_GreenRedPctToColour(){
  for (int i=0; i<=99; i++){
    colour c = TempProbe::GreenRedPctToColour( i );
    Serial.print( c.r );Serial.print( "   " );
    Serial.print( c.g );Serial.print( "   " );
    Serial.print( c.b );Serial.println( "   " );
   };
}   

void Test_TempColours(){
  TriColLED tcl( LED_RED, LED_GRN, LED_BLU );
  for (int i=tMin-2; i<=tMax+2; i++ ){
    TempProbe p( MAXX_SCK, MAXX_CS, MAXX_SO );
    p.ForceTemp( i );
    tcl.SetColour( p.GetColour() );
    tcl.ShowLED( );
    delay(500);
    tcl.TurnLEDOff();  
  };
};

#endif
