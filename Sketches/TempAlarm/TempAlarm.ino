/*

 TEMPERATURE ALARM
 =================
 
*/

/*
    The circuit:
 * MAX SCK pin to digital pin 6 -> 10
 * MAX CS  pin to digital pin 5 -> 9
 * MAX SO  pin to digital pin 4 -> 8 
 * LCD RS  pin 4 to digital pin A2
 * LCD En  pin 6 to digital pin A1
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
// #define DATA_LOGGING
#define PLOTTING
#define TEST_VERBOSE
// #define SHOW_LED_STARTUP

#include "AOUtils.h"
#include "max6675.h" 
#include "math.h"
#include "LiquidCrystal.h"
#include "MemoryFree.h"
#include <SPI.h>
#include <SD.h>
#include <TimeLib.h>


// COLOURS AND COLOUR MANAGEMENT
// =============================

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
const colour g_black =      {0,   0,   0,  };

/*
class Colour {
public:
  Colour( const colour &c ){ col = c; };
private:
  colour col;
};
*/

// LED
// ===

// LED Constants and Setup
static const int redPin = 6;
static const int bluePin = 3;
static const int greenPin = 5;

void const ShowLED( colour c ){
  analogWrite( redPin,   c.r);
  analogWrite( greenPin, c.g);
  analogWrite( bluePin,  c.b);
};

const void TurnLEDOff( ){
  ShowLED( g_black );
  delay( 100 );  
}

// TEMPERATURE AND SENSORS
// =======================

// Temperature and Sensor Constants and Setup
static const int soPin  = 8;   // SO=Serial Out
static const int csPin  = 9;   // CS = chip select CS pin
static const int sckPin = A0;  // SCK = Serial Clock pin

MAX6675 tempSensor(sckPin, csPin, soPin);// create instance object of MAX6675

// Temperature State
enum tState { tFalling, tSteady, tRising };

// Temperature range of interest
static const int tMin = 23; // Shown as Green
static const int tMax = 36; // Shown as Red - above this temp gas is certainly alight

// Probe read interval
static const int readInterval = 5000;

class TempProbe{
public:
      void    SetTemp( const float& t);
const float   GetTemp()       { return temp; };
const float   GetLastTemp()   { return lastTemp; }
const tState  GetState()      { return state; };
const colour  GetColour()     { return col; };
const float   GetEMA()        { return EMA; };
const float   GetLastEMA()    { return lastEMA; };

static colour GreenRedPctToColour ( const int &percent );
                           
private:
       colour TempToColour();
       float  temp             = NAN;
       float  lastTemp         = NAN;
       tState state           = tSteady;
       colour col             = g_magenta;
       int    EMA             = NAN;
       int    lastEMA         = NAN;
const  float  alpha           = 0.3;
};

void TempProbe::SetTemp( const float &t){
  lastTemp = temp;
  temp = t;
// Calculate the "colour" of this temperature  
  TempToColour();

// Calculate new Exponential Moving Average
  lastEMA = EMA;
  if ( isnan( EMA ) ) { EMA=0; };
  EMA = alpha*temp + ( 1-alpha ) * EMA;
  
// TODO Calculate if rising or falling
  if (EMA>lastEMA) 
    { state = tRising; }
  else
  if (EMA=lastEMA) 
    { state = tSteady; }
  else 
    {state = tFalling;};
};

colour TempProbe::GreenRedPctToColour ( const int &pct ) {
  colour col;
  float pctf = pct;
// "yell" represents the percentage where the colour transition has gone from green to yellow
// We set this low so the LED turns yellow quite quickly.   
  float yell = 10;
  float yellFactor = 100 / yell;
  float redFactor = 100 / (100-yell);
  if ( pct <= yell ){
//    col.r = int( 255 * float( (1 - (100-(float)percent*2) / 100 ) )) ;
    col.r = int( 255 * (pctf*yellFactor / 100) );
    col.g = 255;
    col.b = 0;
   }
  else{
    col.r = 255;
//    col.g = int( 255 * ( - (pctf-100)*2 / 100 ));
    col.g = int( 255 * ( - (pctf-100)*redFactor / 100 ));
    col.b = 0;
  };
  return col; 
};

// Calculate this probes shade of Red To Green
colour TempProbe::TempToColour(){
  int colPct;
  if ( !isnan( temp) ){ 
    if ( temp<tMin ){ colPct = 0; 
    }
    else 
    if ( temp>tMax ) { colPct = 100; 
    }
    else { colPct = 100 - int( 100 * float(1 - float( temp-tMin ) / float( tMax-tMin )));
    };
    col = GreenRedPctToColour( colPct );
  }
  else
    col = g_blue;

#ifdef TEST_VERBOSE
Serial.print("val="); Serial.print( temp ); Serial.print(", colPct="); Serial.print(colPct); 
Serial.print(", col="); Serial.print(col.r); Serial.print(" "); Serial.print(col.g); Serial.print(" "); Serial.println(col.b);
#endif // TEST_VERBOSE

  return col;
};

// LCD CONSTANTS AND SETUP
// =======================

static const int LCDen = A1; // was 11, 12,13
static const int LCDrs = A2;
static const int LCDd4 = A3;
static const int LCDd5 = 4;
static const int LCDd6 = 7;
static const int LCDd7 = 2;
LiquidCrystal lcd(LCDrs, LCDen, LCDd4, LCDd5, LCDd6, LCDd7);

// DATA LOGGING CONSTANTS, GLOBALS and SETUP
// =========================================

static const int SD_CS   = 10;
static const int SD_SCK  = 13;
static const int SD_MOSI = 11;
static const int SD_MISO = 12;

int SDWriteCnt = 0;  // To track SD versus LCD writes in loop()

// Declare globals
File dataFile;
TempProbe probe;
  
// SETUP
// =====

void setup() { 
  Serial.begin(9600);// initialize serial monitor with 9600 baud
// Say Hello
  lcd.print( "Hello world" );

// Setup SD if Logging is enabled
#ifdef DATA_LOGGING
  if (!SD.begin(SD_CS)) { Serial.println("SD init failed"); };
  dataFile = SD.open("datalog.txt", FILE_WRITE);

   if (dataFile) {
     Serial.println("dataFile OK");
   };
#endif // DATA_LOGGING
  
// Cycle through the colours so we know the LED(s) are working correctly
#ifdef SHOW_LED_STARTUP
  colour rainbow [6] = {g_red, g_green, g_blue, g_yellow, g_cyan, g_magenta }; 
  for (int i=0; i<=5; i++) {
    ShowLED( rainbow[i]);
    delay (500 );
  };  
  TurnLEDOff();
  
// ... and also display Green to Red and back
  for (int i=0; i<=99; i++ ){
    ShowLED( GreenRedPctToColour( i ) ); 
    delay( 100 );   
  };
  for (int i=99; i>=0; i-- ){
    ShowLED( GreenRedPctToColour( i ) ); 
    delay( 100 );   
  };    
  TurnLEDOff();
#endif SHOW_LED_STARTUP

// set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("    Temp (v1.2)");

// SETUP TESTS
// ===========
    
// Test GreenRedPctToColour
#ifdef TEST_VERBOSE
  for (int i=0; i<=99; i++){
    colour c = TempProbe::GreenRedPctToColour( i );
    Serial.print( c.r );Serial.print( "   " );
    Serial.print( c.g );Serial.print( "   " );
    Serial.print( c.b );Serial.println( "   " );
   };
#endif // TEST_VERBOSE
  
}; // End Setup

// LOOP
// ====

void loop() {

  int startms = millis(); // Save start ms so we can accurately time each loop

#ifdef TEST_VERBOSE1
for (int i=tMin-2; i<=tMax+2; i++ ){
  probe.SetTemp( i );
  ShowLED( probe.GetColour() );
  delay(500);
  TurnLEDOff();  
};
#endif // TEST_VERBOSE1

#ifdef TEST_VERBOSE
Serial.print("lastEMA="); Serial.print(probe.GetLastEMA()); Serial.print("  EMA="); Serial.print(probe.GetEMA()); 
Serial.print("  State="); Serial.println(probe.GetState());
#endif // TEST_VERBOSE
    
// Read the temperature(s) and save
  probe.SetTemp(tempSensor.readCelsius());
  colour col = probe.GetColour();
  
#ifdef TEST_VERBOSE
Serial.print("SetTemp Colour = ");
Serial.println(col.r);
Serial.println(col.g);
Serial.println(col.b);
#endif

  ShowLED( probe.GetColour() );

  
// TODO Calculate if state is steady and temp above ambient
// Gas appears to be left ON

    
// Write to serial port so we can plot them
#ifdef PLOTTING
  //Serial.print("EMA="); 
  Serial.print(probe.GetEMA()); 
  Serial.print(" ");
  //Serial.print(" Temp="); 
  Serial.print(probe.GetTemp());
  Serial.print(" ");
  Serial.println("");
#endif // PLOTTING

// Show values on LCD
// set the cursor to column 0, line 1
// (note: line 1 is the second row, since counting begins with 0):
// Display on LCD
  lcd.setCursor(0, 1);
  if (!isnan( probe.GetEMA() )) { 
    lcd.print( probe.GetEMA() ); 
  } else { 
    lcd.print( "NoVal"); 
  };
  lcd.print( "   " );

  if (!isnan( probe.GetTemp() ) ) {
    lcd.print( probe.GetTemp() );
  } else { 
    lcd.print( "NoVal"); 
  };
  lcd.print( "   " );

#ifdef DATA_LOGGING
// Print to file 
  if ( SDWriteCnt == 12) {
    SDWriteCnt = 0;
    lcd.setCursor(0,0);
    int mins = minute();
    dataFile.print( minute() );
    dataFile.print(", "); 
    if (!isnan( probe.GetEMA() )) { 
      dataFile.print( probe.GetEMA() ); 
      dataFile.print(", ");  
    } else { 
      dataFile.print( "NoVal, " );
    };
    if (!isnan( probe.GetTemp() ) ) {
      dataFile.println( probe.GetTemp() );
    } else { 
      dataFile.println( "NoVal" ); 
    };
    dataFile.flush();
  } else {
    SDWriteCnt = SDWriteCnt + 1;
  };
#endif // Data_LOGGING
  int endms = millis();
  int del = readInterval-(endms-startms);
  Serial.print("startms=");Serial.print(startms); Serial.print("  endms="); Serial.print(endms);
  Serial.print("  del="); Serial.print(del);
  if (del<readInterval && del>0) { 
    delay (del);
  }; // During tesing this condition can be invalid as test takes too long - even go -ve
  Serial.println(" ... back from delay()");
};
