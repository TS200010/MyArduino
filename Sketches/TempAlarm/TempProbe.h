#ifndef TEMP_PROBE_H
#define TEMP_PROBE_H

#include <Arduino.h>
#include "max6675.h" 
#include "Task.h"

#include "Globals.h"
#include "Colour.h"

enum tState { tSteady, tRising, tFalling, tAlarm }; 
const int maxSticky = 3;

// TEMPERATURE AND SENSORS
// =======================

// Probe read interval
static const int readInterval = 10000;

// Temperature range of interest
static const int tMin = 20; // Shown as Green
static const int tMax = 40; // Shown as Red - above this temp gas is certainly alight

class TempProbe /*: public TimedTask*/ {
public:
      TempProbe( int _sckPin, int _csPin, int _soPin );
//virtual void run(uint32_t now);
      void    SetTemp();
const float   GetTemp()       { return temp; };
const float   GetLastTemp()   { return lastTemp; }
const tState  GetState()      { return state; };
const colour  GetColour()     { return col; };
const float   GetEMA()        { return EMA; };
const float   GetLastEMA()    { return lastEMA; };
const int     GetSticky()     { return sticky; };

      void    ForceTemp( int i );  // Only to be used in testing

static colour GreenRedPctToColour ( const int &percent );
                           
private:
       colour TempToColour();
       int       sckPin;
       int       csPin;
       int       soPin;
       MAX6675   tempSensor;
       float     temp            = NAN;
       float     lastTemp        = NAN;
       tState    state           = tSteady;
       colour    col             = g_magenta;
       int       EMA             = NAN;
       int       lastEMA         = NAN;
       int       sticky          = 0;
const  float     alpha           = 0.3; // Used in EMA calculation
};


#endif
