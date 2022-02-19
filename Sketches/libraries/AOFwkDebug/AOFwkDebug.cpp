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
    Serial.flush();
    do {_flashLED (flashes1, flashes2);} while ( stop );
}

/*
 * Flashes the LED "flashes" times and loops the program if "stop" is true.
 */
void _flashLED ( int flashes1, int flashes2){
 
    Serial.print( "Here 1 ");    Serial.print( "\n" );    Serial.flush();
    
    pinMode( LED_BUILTIN, OUTPUT );
    
    Serial.print( "Here 2");    Serial.print( "\n" );    Serial.flush();

    for ( int i=1; i<=flashes1; i++ ){
       
        Serial.print( "Here 3");    Serial.print( "\n" );    Serial.flush();

        digitalWrite( LED_BUILTIN, 1 );
        
        Serial.print( "Here 4");    Serial.print( "\n" );    Serial.flush();
        
        delay( 1000 );
        
        Serial.print( "Here 5");    Serial.print( "\n" );    Serial.flush();
        
        digitalWrite( LED_BUILTIN, 0 );
 //       delay( 50 );
    };
//delay( 1000 );
    for (int i=1; i<=flashes2; i++ ){
        digitalWrite( LED_BUILTIN, 1 );
 //       delay( 250 );
        digitalWrite( LED_BUILTIN, 0 );
  //      delay( 50 );
    };
    delay ( 2000 );
}
