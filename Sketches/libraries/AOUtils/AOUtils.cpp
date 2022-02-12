//
//  AOUtils.cpp
//  
//
//  Created by Anthony Stanners on 07/01/2022.
//
#ifndef AOUTILS_CPP
#include "AOUtils.h"
#include "MemoryFree.h"

void printFreeSRAM(){
    Serial.print(F("Free SRAM = "));
    Serial.println(freeMemory());
}

#endif // AOUTILS_CPP
