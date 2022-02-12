//
//  AODebug.h
//  
//
//  Created by Anthony Stanners on 07/01/2022.
//

#ifndef AODEBUG_H
#define AODEBUG_H


void _assert( bool condition, int flashes1, int flashes2, bool stop );
void _errorMessageAndFlash( int flashes1, int flashes2, bool stop );
void _flashLED ( int flashes1, int flashes2 );

#define prt(p) Serial.print( " " ); Serial.print( p ); Serial.print( " " );

#define DEBUG_PRINT(str)        \
    Serial.print(millis());     \
    Serial.print(": ");         \
    Serial.print(__PRETTY_FUNCTION__); \
    Serial.print(' ');          \
    Serial.print(__FILE__);     \
    Serial.print(':');          \
    Serial.print(__LINE__);     \
    Serial.print(' ');          \
    Serial.println(str);

#define STOP true


// AO FRAMEWORK ERROR CODES
// ------------------------

#define AO_ERROR_CLASS_1    1   // CDIgitalPinOut
#define AO_ERROR_METHOD_1   1   // SetPin - Invalid Pin Number
#define AO_ERROR_METHOD_2   2   // GetPin - Invalid Pin Number
#define AO_ERROR_METHOD_3   3   // TogglePin - Invalid Pin Number
#define AO_ERROR_CLASS_2    2   // CDIgitalPinIn
#define AO_ERROR_METHOD_1   1   // GetPin - Invalid Pin Number
#define AO_ERROR_CLASS_3    3   // CPWMPin
#define AO_ERROR_METHOD_1   1   // SetPin - Invalid Pin Number

#endif // AODEBUG_H
