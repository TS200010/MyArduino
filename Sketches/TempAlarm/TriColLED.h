#ifndef TRI_COL_LED_H
#define TRI_COL_LED_H

#include <stdint.h>
#include "Task.h"

#include "Globals.h"
#include "Colour.h"

static const int LEDFlashTime     = 5000;
static const int LEDPulseInterval = 200;

static colour colRising  = g_cyan;
static colour colFalling = g_magenta;
static colour colAlarm1  = g_red;
static colour colAlarm2  = g_green;

class TriColLED : public TimedTask {
  public:
    TriColLED( uint8_t _redPin, uint8_t _greenPin, uint8_t _bluePin);
    virtual void run(uint32_t now);
    void         SetColour( colour _col) { col = _col; };
    void         SetState( tState _state ) { state = _state; };
    void         SetSteady() { state = tSteady;  pulseOff = true; };
    void         SetRising() { state = tRising;  pulseOff = true; };
    void         SetFalling(){ state = tFalling; pulseOff = true; };
    void         SetAlarm()  { state = tAlarm;   pulseOff = true; }; 
    const void   TurnLEDOff(); 
    void const   ShowLED();  // Will immediately change LED colour without waiting
    
  private:
    void const   ShowLED( colour );
    tState       state;
    uint8_t      redPin;
    uint8_t      greenPin;
    uint8_t      bluePin;
    colour       col;
    bool         pulseOff = true;
//    uint8_t      nextIncRunTime;
};

#endif
