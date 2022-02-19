/*
 MAX6675 and LCD1692 Example


    The circuit:
 * MAX SCK pin to digital pin 6 -> 10
 * MAX CS  pin to digital pin 5 -> 9
 * MAX SO  pin to digital pin 4 -> 8 
 * LCD RS  pin 4 to digital pin 12
 * LCD En  pin 6 to digital pin 11
 * LCD D4  pin 10 to digital pin 5
 * LCD D5  pin 11 to digital pin 4
 * LCD D6  pin 12 to digital pin 3
 * LCD D7  pin 13 to digital pin 2
 * LCD R/W pin 5 to ground
 * LCD VSS pin 1 to ground
 * LCD VCC pin 2 to 5V
 * 10K potentiometer VR1:
 * VR1 ends to +5V and ground
 * LCD VO  pin 3 to VR1 wiper

*/

#include "max6675.h" 
#include "math.h"
#include <LiquidCrystal.h>
#include "AOFwk.h"

const int soPin  = 8;   // SO=Serial Out
const int csPin  = 9;   // CS = chip select CS pin
const int sckPin = 10;  // SCK = Serial Clock pin
float EMA=0;
float alpha=0.1;

enum tState {tRising, tFalling, tAmbient, tSteady};

class temperature{
private: 
  float val;
  tState state;
public:
  SetTemp( const float& t) { val = t; };
  SetState ( const tState& s) {state = s;};
  float GetTemp(){ return val; };
  tState GetState(){ return state; };
};


MAX6675 tSensor(sckPin, csPin, soPin);// create instance object of MAX6675

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Constants User can update to explore the behaviour of the example
const uint8_t kPWMBrightnessMax = 10;           // Maximum PWM value controls maximum brightness ( 0 to kPWMMax )
const int kTotalCycleTime = kTenSeconds;        // Number of milliseconds for the complete cycle of 
                                                //   three colours. Note how the constants improve readability here.
const int kLEDUpdateDelay = kTotalCycleTime / (kPWMBrightnessMax + 1); 

CPWMPin  g_BlueLED(  kPWM3, kActiveHigh );
CPWMPin  g_GreenLED( kPWM5, kActiveHigh );
CPWMPin  g_RedLED(   kPWM6, kActiveHigh );

void setup() {          
  Serial.begin(9600);// initialize serial monitor with 9600 baud
  Serial.println("MAX6675"); 
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD
  lcd.print("EMA     Temp v1");
}

void loop() {
   int startms = millis();
//   float t=Module.readCelsius();
   temperature t; 
   t.SetTemp(tSensor.readCelsius());
   EMA = alpha*t.GetTemp() + (1-alpha) * EMA;
   if (isnan(EMA)) {EMA=0;};   
   //Serial.print("EMA="); 
   Serial.print(EMA); 
   Serial.print(" ");
   //Serial.print(" Temp="); 
   Serial.print(t.GetTemp());
   Serial.print(" ");
   Serial.println("");
   delay(500-(millis()-startms));
   

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);
  if (!isnan(EMA)) { lcd.print( EMA ); } else {lcd.print( "NoVal");};
  lcd.print( "   " );
  if (!isnan(t.GetTemp())) { lcd.print( t.GetTemp() ); } else {lcd.print( "NoVal");};
  lcd.print( "   " );
}
