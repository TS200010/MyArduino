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


#include "AOUtils.h"
#include "max6675.h" 
#include "math.h"
#include "LiquidCrystal.h"
#include "MemoryFree.h"
#include <SPI.h>
#include <SD.h>

// LED Constants and Setup
static const int redPin = 6;
static const int bluePin = 3;
static const int greenPin = 5;

// Temperature and Sensor Constants and Setup
static const int soPin  = 8;   // SO=Serial Out
static const int csPin  = 9;   // CS = chip select CS pin
static const int sckPin = A0;  // SCK = Serial Clock pin

float EMA=0;
float alpha=0.3;

enum tState {tRising, tFalling, tAmbient, tSteady};

class temperature{
public:
  SetTemp( const float& t) { val = t; };
  SetState ( const tState& s) {state = s;};
  float GetTemp(){ return val; };
  tState GetState(){ return state; };
private: 
  float val;
  tState state;
};

// Colours and Colour Management

struct colour {
public:
int r, g, b;   
};

// See https://www.rapidtables.com/web/color/RGB_Color.html

const colour g_red =        {255, 0,   0   };
const colour g_green =      {0,   255, 0   };
const colour g_blue =       {0,   0,   255 };
const colour g_yellow =     {255, 255, 0   };
const colour g_cyan =       {0,   255, 255 };
const colour g_magenta =    {255, 0,   255 };

colour rainbow [6] = {g_red, g_green, g_blue, g_yellow, g_cyan, g_magenta }; 

const void midColour( const colour& col1, const colour& col2, const uint8_t& level) {
  }; 

  
MAX6675 tempSensor(sckPin, csPin, soPin);// create instance object of MAX6675

// LCD Constants and Setup

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
static const int LCDen = A1; // was 11, 12,13
static const int LCDrs = A2;
static const int LCDd4 = A3;
static const int LCDd5 = 4;
static const int LCDd6 = 7;
static const int LCDd7 = 2;
LiquidCrystal lcd(LCDrs, LCDen, LCDd4, LCDd5, LCDd6, LCDd7);


File dataFile;

void setup() { 

// Say Hello 
   lcd.print( "Hello world" );

   Serial.begin(9600);// initialize serial monitor with 9600 baud

   Serial.print("Initializing SD card...");
   if (!SD.begin(10)) {
      Serial.println("SD initialization failed!");
   };
   Serial.println("SD initialization done.");

  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    Serial.println("dataFile OK");
  };

// Say Hello 
   lcd.print( "Hello world" );

   Serial.begin(9600);// initialize serial monitor with 9600 baud

   Serial.print("Initializing SD card...");
   if (!SD.begin(10)) {
      Serial.println("SD initialization failed!");
   };
   Serial.println("SD initialization done.");

  //dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    Serial.println("dataFile OK");
  };

// Display basic colours on the LCD    
  for (int i=0; i<=5; i++) {
    analogWrite( redPin,   rainbow[i].r);
    analogWrite( greenPin, rainbow[i].g);
    analogWrite( bluePin,  rainbow[i].b);
    delay (1000 );  
  };
          
  Serial.println("MAX6675"); 
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD
  lcd.print("EMA     Temp (v1.1)");
}

void loop() {
  int startms = millis();
   temperature t; 
   t.SetTemp(tempSensor.readCelsius());
   if (isnan(EMA)) {EMA=0;};
   EMA = alpha*t.GetTemp() + (1-alpha) * EMA;
   //Serial.print("EMA="); 
   Serial.print(EMA); 
   Serial.print(" ");
   //Serial.print(" Temp="); 
   Serial.print(t.GetTemp());
   Serial.print(" ");
   Serial.println("");

   // set the cursor to column 0, line 1
   // (note: line 1 is the second row, since counting begins with 0):
   lcd.setCursor(0, 1);
   // print the number of seconds since reset:
   //lcd.print(millis() / 1000);
   if (!isnan(EMA)) { 
     lcd.print( EMA ); 
     dataFile.print( EMA ); 
     dataFile.print(", "); } 
   else { 
     lcd.print( "NoVal"); 
     dataFile.print( "NoVal, " );
   };
   lcd.print( "   " );
   float temp = t.GetTemp();
   if (!isnan( temp ) ) {
     lcd.print( temp ); 
     dataFile.println( temp ); } 
   else { 
     lcd.print( "NoVal"); 
     dataFile.println( "NoVal" ); 
   };
   lcd.print( "   " );
   dataFile.flush();
  
   int del = 500-(millis()-startms);
   delay (del);

 
}
