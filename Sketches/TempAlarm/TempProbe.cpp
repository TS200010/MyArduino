#ifndef TEMP_PROBE_CPP
#define TEMP_PROBE_CPP

#include "Globals.h"
#include "TempProbe.h"
#include "Colour.h"

TempProbe::TempProbe( int _sckPin, int _csPin, int _soPin ) 
                         : sckPin( _sckPin ), csPin( _csPin ), soPin( _soPin ), tempSensor( _sckPin, _csPin, _soPin ) { };
                                                   
void TempProbe::SetTemp( ){
  lastTemp = temp;
  
  if ( muxChannel != noMux ){
    mux->channel( muxChannel );
  }

  temp = tempSensor.readCelsius();
  

  
// Calculate the "colour" of this temperature  
  TempToColour();

// Calculate new Exponential Moving Average
  lastEMA = EMA;
  if ( isnan( EMA ) ) { EMA=0; };
  EMA = alpha*temp + ( 1-alpha ) * EMA;
  
// Calculate if temperature is rising, falling or steady
// Once state changes hold it there for 10 cycles
  if (sticky>0) {
    sticky -= 1;  
  } else {
    if (EMA>lastEMA) 
      { state = tRising; }
    else
    if (EMA<lastEMA) 
      { state = tFalling; }
    else 
      { state = tSteady;};
    sticky = maxSticky;
  };  


// TODO Determine if we need to raise an alarm
    
};

void TempProbe::ForceTemp( int _temp ){
  temp = _temp;
  TempToColour();
}

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

// Calculates this probes shade of Red To Green
colour TempProbe::TempToColour(){
  int colPct;
  if ( !isnan( temp) ){ 
    if ( temp<tMin ){ colPct = 0; 
    }
    else 
    if ( temp>tMax ){ colPct = 100;
    }
    else { colPct = 100 - int( 100 * float(1 - float( temp-tMin ) / float( tMax-tMin )));
    };
    col = GreenRedPctToColour( colPct );
  }
  else
    col = g_blue;

#ifdef TEST_VERBOSE
Serial.flush(); Serial.println("");
Serial.print("Temp="); Serial.print( temp ); Serial.print(", colPct="); Serial.print(colPct); 
Serial.print(", col="); Serial.print(col.r); Serial.print(" "); Serial.print(col.g); Serial.print(" "); Serial.println(col.b);
#endif // TEST_VERBOSE

  return col;
};

#endif
