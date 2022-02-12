//
//  AODebug.cpp
//  
//
//  Created by Anthony Stanners on 07/01/2022.
//

#include <Arduino.h>
#include "AOConst.h"
#include "AOFwkDebug.h"

/*
 * Calls _errorMessageAndFlash if "expression" is false.
 */
void _assert( bool condition, int flashes1, int flashes2, bool stop ){
    if ( !condition ){
      _errorMessageAndFlash( flashes1, flashes2, stop );
    };
}

/*
 * Sends a message to the Serial monitor and then flashes the onboard LED "flashes" times
 * repeatedly. The program loops in this condition.
 */
void _errorMessageAndFlash( int flashes1, int flashes2, bool stop ){
//    Serial.begin( 9600 );
//    while ( !Serial ){ };         // Wait until Serial is ready - Only for Leonardo?
    Serial.print( "Error Code: ");
    prt( flashes1 );
    prt( flashes2 );
    Serial.print( "\n" );
    do {_flashLED (flashes1, flashes2);} while ( stop );
}

/*
 * Flashes the LED "flashes" times and loops the program if "stop" is true.
 */
void _flashLED ( int flashes1, int flashes2){
    pinMode( LED_BUILTIN, OUTPUT );
    for ( int i=1; i<=flashes1; i++ ){
        digitalWrite( LED_BUILTIN, 1 );
        delay( 1000 );
        digitalWrite( LED_BUILTIN, 0 );
        delay( 50 );
    };
    delay( 1000 );
    for (int i=1; i<=flashes2; i++ ){
        digitalWrite( LED_BUILTIN, 1 );
        delay( 250 );
        digitalWrite( LED_BUILTIN, 0 );
        delay( 50 );
    };
    delay ( 2000 );
}
