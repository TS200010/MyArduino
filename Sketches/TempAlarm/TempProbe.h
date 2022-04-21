#ifndef TEMP_PROBE_H
#define TEMP_PROBE_H

#include <Arduino.h>
#include "max6675.h" 
#include "light_CD74HC4067.h"
#include "Task.h"

#include "Globals.h"
#include "Colour.h"

enum tState { tSteady, tRising, tFalling, tAlarm }; 
const int maxSticky = 3;
const int noMux = 255;

// TEMPERATURE AND SENSORS
// =======================


class TempProbe /*: public TimedTask*/ {
public:
      TempProbe( int _sckPin, int _csPin, int _soPin );

//virtual void run(uint32_t now);
      void    SetTemp();
      void    SetMux( CD74HC4067 *_mux, uint8_t _muxChannel ){ mux = _mux; muxChannel = _muxChannel; };
const float   GetTemp()         { return temp; };
const float   GetLastTemp()     { return lastTemp; }
const tState  GetState()        { return state; };
const colour  GetColour()       { return col; };
const float   GetEMA()          { return EMA; };
const float   GetLastEMA()      { return lastEMA; };
const int     GetSticky()       { return sticky; };
      void    HasGasBeenLeftOn();

      void    ForceTemp( int i );  // Only to be used in testing

static colour GreenRedPctToColour ( const int &percent );
                           
private:
       colour TempToColour();
       int        sckPin;
       int        csPin;
       int        soPin;
       uint8_t    muxChannel = noMux;

       MAX6675    tempSensor;
       CD74HC4067 *mux;
       float      temp            = NAN;
       float      lastTemp        = NAN;
       tState     state           = tSteady;
       colour     col             = g_magenta;
       float      EMA             = NAN;
       float      lastEMA         = NAN;
       int        sticky          = 0;
const  float      alpha           = 0.3; // Used in EMA calculation
};


#endif
