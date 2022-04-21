#ifndef TRI_COL_LED_H
#define TRI_COL_LED_H

#include <stdint.h>
#include "Task.h"
#include <PCA9685.h>

#include "Globals.h"
#include "Colour.h"

static colour colRising  = g_cyan;
static colour colFalling = g_magenta;
static colour colAlarm1  = g_red;
static colour colAlarm2  = g_green;

class TriColLED : public TimedTask {
  public:
    TriColLED( int _channel, PCA9685 *_pwm );
    TriColLED( uint8_t _redPin, uint8_t _greenPin, uint8_t _bluePin );
    virtual void   run( uint32_t now );
    void           SetColour( colour _col) { col = _col; };
    void           SetState( tState _state ) { state = _state; };
    void           SetAlarm()  { state = tAlarm;   pulseOff = true; };
//    const void     TurnLEDOff(); 
    void const     ShowLED();  // Will immediately change LED colour without waiting
  
  private:
    float          Brightness() { return float(analogRead( LED_BRIGHTNESS_PIN )-200)/10+1; }; 
                     // Turns analog value to a percentage. analogRead() returns 0-1023. +1 ensures it is never 0
                     // Magic numbers are a crude attempt to make perceived brightness track pot position
    void const     ShowLED( colour );
    uint16_t const RGBToPWM( int _rgb ) { return _rgb*16; }; // Times 16 converts 0-255 value to 0-4096 value
    
    uint8_t        redPin;
    uint8_t        greenPin;
    uint8_t        bluePin;
    colour         col = g_blue;
    bool           pulseOff = true;
    int            pwmChannel = NO_LED_CHAN;
    PCA9685        *pwmController;
//    uint8_t      nextIncRunTime;

// TODO - why is this in the LED Class - should get it from the TempProbe
    tState         state;  

};

#endif
