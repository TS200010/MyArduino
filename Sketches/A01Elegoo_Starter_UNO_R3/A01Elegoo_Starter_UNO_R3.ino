/*
 * This file is a compendium of the examples in the Elegoo UNO R3 Prokect Kit for the 
 * intermediate programmer.
 * 
 * Whilst the examples as provided with the kit are great and go a long way to teaching a
 * beginner how to make an Arduino work, the examples are not so good for the intermediate
 * user. This file takes all the examples and combines them into one file where they can 
 * easily be individually compiled and run but also show how an intermediate user might 
 * actually write the code.
 * 
 * First we define some meaningful constants for the Arduino Architecture.
 * Don't concern yourself with how these type definitions work. Just use them as in the examples. 
 * You will benefit from compile time checking of your use of the pins for digital, analog etc. 
 *  
 */

//#define _debug

#include "AOConst.h"
#include "AOFwk.h"
#include "AOUtils.h"
//#include "AODebug.h"

/*
 *  DEBUGGING FACILITIES
 *  --------------------
 *  
 *  To remove debugging facilities, comment out the line below with '//'
 */


/* 
 * THE EXAMPLES
 * ------------
 *
 * To complie and run a partilular example, remove the comment symbol "//" in front or the relavent #define
 * Only One example will work at a time!
*/

// #define VOID_PROGRAM         // Empty program will do nothing but clear the Arduino memory.
// #define BLINK                // Default Manufacturing Test Application
// #define BLINK_CPP            // --ditt0-- uning classes
// #define GET_CPP_VERSION      // Prints C++ version to Serial Monitor
// #define RGB_LED              // Lesson 4
// #define RGB_LED_OPTIMISED    // ... with optimised code 
// #define RGB_LED_CPP          // ... C++ version
// #define DIGITAL_INPUTS       // Lesson 5
#define DIGITAL_INPUTS_CPP   // ... C++ version

