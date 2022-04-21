#ifndef TRI_COL_LED_CPP
#define TRI_COL_LED_CPP

#include <Arduino.h>

#include "Globals.h"
#include "TriColLED.h"
#include "Task.h"
#include "TempProbe.h"

TriColLED::TriColLED( int _channel,PCA9685 *_pwm ) : TimedTask( millis() ), pwmChannel(_channel ), pwmController( _pwm ) { };

TriColLED::TriColLED( uint8_t _redPin, uint8_t _greenPin, uint8_t _bluePin ) 
                    : TimedTask( millis() ), 
                      redPin( _redPin ), greenPin( _greenPin ), bluePin( _bluePin ) {
    pinMode( redPin,   OUTPUT ); 
    pinMode( greenPin, OUTPUT ); 
    pinMode( bluePin,  OUTPUT ); 
};

void TriColLED::run(uint32_t now)
{     
  int nextIncRunTime = g_LEDFlashTime;  // Assume this unless set in switch statement
  switch( state ) {
    case tSteady:
#ifdef TEST_VERBOSE 
Serial.println ("tSteady");  
Serial.println("case tSteady");
#endif  
                  ShowLED();
                  break;
                  
    case tRising: 
#ifdef TEST_VERBOSE 
Serial.println ("tRising"); 
Serial.print("case tRising ... pulseOff="); Serial.println(pulseOff);
#endif 
                  if (pulseOff) { 
                    ShowLED( colRising );
                    nextIncRunTime = g_LEDPulseInterval;
                  } else {
                    ShowLED();
                  };
                  pulseOff = !pulseOff;
                  break;

    case tFalling:
#ifdef TEST_VERBOSE      
Serial.println ("tFalling"); 
Serial.print("case tFalling ... pulseOff="); Serial.println(pulseOff);
#endif 
                  if (pulseOff) { 
                    ShowLED( colFalling );
                    nextIncRunTime = g_LEDPulseInterval;
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
//  incRunTime( nextIncRunTime );
   setRunTime( now + nextIncRunTime );
}

void const TriColLED::ShowLED( colour _col){
  float b = Brightness();
#ifdef TEST_VERBOSE 
Serial.print ("Brightness = "); Serial.println(b);
#endif 
/*  if (pwmChannel==255){
    analogWrite( redPin,   _col.r);
    analogWrite( greenPin, _col.g);
    analogWrite( bluePin,  _col.b);
 } else {
 */
    pwmController->setChannelPWM( pwmChannel,   RGBToPWM( col.r )/b );
    pwmController->setChannelPWM( pwmChannel+1, RGBToPWM( col.g )/b );
    pwmController->setChannelPWM( pwmChannel+2, RGBToPWM( col.b )/b );
 //}
};

void const TriColLED::ShowLED( ){
  ShowLED( col );
};


#endif
