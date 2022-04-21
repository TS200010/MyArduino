#ifndef TEMP_ALARM_TESTS_CPP
#define TEMP_ALARM_TESTS_CPP

#include "Globals.h"
#include "TempProbe.h"
#include "TriColLED.h"
#include "MessageQueue.h"

#ifdef TEST_GREEN_RED_PCT_COLOUR
void Test_GreenRedPctToColour(){
  for (int i=0; i<=99; i++){
    colour c = TempProbe::GreenRedPctToColour( i );
    Serial.print( c.r );Serial.print( "   " );
    Serial.print( c.g );Serial.print( "   " );
    Serial.print( c.b );Serial.println( "   " );
   };
}   
#endif

#ifdef TEST_TEMP_COLOURS
void Test_TempColours(){
  PCA9685 pwmController;
  pwmController.resetDevices();       // Resets all PCA9685 devices on i2c line
  pwmController.init();               // Initializes module using default totem-pole driver mode, and default disabled phase balancer
  pwmController.setPWMFrequency(100); // Set PWM freq to 100Hz (default is 200Hz, supports 24Hz to 1526Hz)
  pwmController.setChannelPWM(0, 64 << 4); // Set PWM to 128/255, shifted into 4096-land
  TriColLED tcl( LED1_CHAN, &pwmController );
  for (int i=g_tMin-2; i<=g_tMax+2; i++ ){
    TempProbe p( MAXX_SCK, MAXX_CS, MAXX_SO );
    p.ForceTemp( i );
    tcl.SetColour( p.GetColour() );
    tcl.ShowLED( );
    delay(500);
    tcl.SetColour( g_black );  
  };
};
#endif

#ifdef TEST_MESSAGE_QUEUE
#define P Serial.print
#define PL Serial.println
void Test_MessageQueue(){
  CMessageQueue Q;
  message msg;
  int memStart = freeMemory();
  int memEnd;
  for (int i=1; i<=3; i++){
    Q.AddElem( i );
    memEnd = freeMemory();
    P("Heap Start: "); P(memStart); P(" Heap End: "); PL(memEnd);
  };
  P("Queue Size: "); PL( Q.SizeOf() );
  Q.PrintQ();
  msg = Q.RemoveElem();
  P("Message removed: "); PL( msg );
  P("Queue Size: "); PL( Q.SizeOf() );
  Q.PrintQ();
  msg = Q.RemoveElem();
  P("Message removed: "); PL( msg );
  P("Queue Size: "); PL( Q.SizeOf() );
  Q.PrintQ();
  msg = Q.RemoveElem();
  P("Message removed: "); PL( msg );
  P("Queue Size: "); PL( Q.SizeOf() );
  Q.PrintQ();
  msg = Q.RemoveElem();
  P("Message removed: "); PL( msg );
  
  memEnd = freeMemory();
  P("Heap Start: "); P(memStart); P(" Heap End: "); PL(memEnd);
};
#endif


#endif //TEMP_ALARM_TESTS_CPP
