#ifndef TRI_COL_LED_CPP
#define TRI_COL_LED_CPP

#include <Arduino.h>

#include "Globals.h"
#include "TriColLED.h"
#include "Task.h"
#include "TempProbe.h"

TriColLED::TriColLED( uint8_t _redPin, uint8_t _greenPin, uint8_t _bluePin ) 
                    : TimedTask( millis() ), 
                      redPin( _redPin ), greenPin( _greenPin ), bluePin( _bluePin ) {
  pinMode( redPin,   OUTPUT ); 
  pinMode( greenPin, OUTPUT ); 
  pinMode( bluePin,  OUTPUT ); 
 };

void TriColLED::run(uint32_t now)
{     
  int nextIncRunTime = LEDFlashTime;  // Assume this unless set in switch statement
  switch( state ) {
    case tSteady: Serial.println ("tSteady");  
    Serial.println("case tSteady");
                  ShowLED();
                  break;
                  
    case tRising: Serial.println ("tRising"); 
    Serial.print("case tRising ... pulseOff="); Serial.println(pulseOff);
                  if (pulseOff) { 
                    ShowLED( colRising );
                    nextIncRunTime = LEDPulseInterval;
                  } else {
                    ShowLED();
                  };
                  pulseOff = !pulseOff;
                  break;

    case tFalling: Serial.println ("tFalling"); 
    Serial.print("case tFalling ... pulseOff="); Serial.println(pulseOff);
                  if (pulseOff) { 
                    ShowLED( colFalling );
                    nextIncRunTime = LEDPulseInterval;
                  } else {
                    ShowLED();
                  };
                  pulseOff = !pulseOff;
                  break;
   
    case tAlarm:   if (pulseOff) { 
                    ShowLED( colAlarm1 );
                  } else {
                    ShowLED( colAlarm2);
                  };
                  pulseOff = !pulseOff;
                  break;                                          
 };
  incRunTime( nextIncRunTime );
}

void const TriColLED::ShowLED( colour _col){
  analogWrite( redPin,   _col.r);
  analogWrite( greenPin, _col.g);
  analogWrite( bluePin,  _col.b);
};

void const TriColLED::ShowLED( ){
  ShowLED( col );
};

const void TriColLED::TurnLEDOff( ){
  colour colsave = col;
  col = g_black;
  ShowLED( );
  col = colsave;  
//  delay( 100 );  
}

#endif
