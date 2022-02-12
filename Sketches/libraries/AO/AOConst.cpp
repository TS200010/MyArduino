xx
#ifndef AOCONST_CPP
#define AOCONST_CPP

#include "AOConst.h"

// Procedural Versions of Pin Commands with type checking (Arduino Objects)
// -----------------------------------------------------------------------
void pinModeAO(      PWMPin_t pin, uint8_t mode )           { ::pinMode( (uint8_t)pin, mode); };
//void pinModeAO(      digitalPin_t pin, uint8_t mode )       { ::pinMode( (uint8_t)pin, mode); };
void analogWriteAO(  analogPin_t pin, int val )             { ::analogWrite( (uint8_t)pin, (int)val); };
//void analogWriteAO(  PWMPin_t pin, PWMData_t val )          { ::analogWrite( (uint8_t)pin, (int)val); };
void digitalWriteAO( digitalPin_t pin, digitalData_t val )  { ::digitalWrite( (uint8_t)pin, (uint8_t)val); };

#endif // AOCONST_CPP
