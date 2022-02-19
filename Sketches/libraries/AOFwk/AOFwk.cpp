/*
 AOFwk.cpp - Part of the Library for Arduino wrapped as Objects.
 
 AOFwk is a thin layer od C++ classes that helps user create correct programs more quickly by providing a framework for catching errors and inconsistencies at compile time - along the lines of providing primitives that encourage "intentional programming".
 
 Created by Anthony J Stanners January 20th 2022. (c) 2022
     
 Released into the public domain.
 
 This version of the library has been tested with UNO. Should it be useful, contributions are welcome for other models.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef AOFWK_CPP
#define AOFWK_CPP

#include <arduino.h>

#include "AOConst.h"
#include "AOFwk.h"
#include "AOFwkDebug.h"

/* CDigitalPinOut is a class that wraps a pin behaving as a digital pin. A key abstraction is that the user does not need to track and remember if the pin is used Active High or Active Low. When a CDigitalPin object is created it is created specifying if it is active high or not. When the pin is set or read the boolean consts ACVITE and NOT_ACTIVE are used. The class takes care of getting and setting the actual value of the pin (0 or 1) correctly.
 
 Valid pins are in the range 0 to (NUM_DIGITAL_PINS - 1) + NUM_ANALOG_INPUTS.
 
 Pins 0 and 1 are permitted as digital pins even though they may also be used for Serial Input and Output. This is reflected by these pins also having the names kDigitalRX and kDigitalTX respectively. Refer to AOConst.h.
 
 Pins 3, 5, 6, 9, 10, and 11 are permitted as digital pins even though they may also be used for Pulse Width Modulated output. This is reflected by these pins also having the names kPWMn  where n is one of the digital pin numbers. Refer to AOConst.h
 
 Methods are provided to:
    Check validity of the object.
    Set the pin.
    Get the pins value.
    Get the last value that the pin was set to. Useful on architectures that do not reliably read an output pni.
*/

/* CDigitalPinOut Constructor
*/

CDigitalPinOut::CDigitalPinOut( const digitalPin_t& pin, const bool& activeHigh ){
    if ( pin > NUM_DIGITAL_PINS-1 ){
        m_pin = kInvalidDigPin;
        return;
    };
    m_pin = pin;
    m_activeHigh = activeHigh;
    ::pinMode( (uint8_t)m_pin, OUTPUT );
    m_lastWriteVal = NOT_A_DIGITAL_VALUE;
};

/* CDigitalPinOut::IsValid
 Performs a validity check on itself.
*/
const bool CDigitalPinOut::IsValid( ) {
    return m_pin != kInvalidDigPin;
};

/* CDigitalPinOut::SetPin
 Performs a validity check on itself.
*/
digitalData_t CDigitalPinOut::SetPin( const bool& activeOrNot ) {
    _assert( IsValid(), AO_ERROR_CLASS_1, AO_ERROR_METHOD_1, STOP );
    uint8_t val = ((activeOrNot && m_activeHigh) || (!activeOrNot && !m_activeHigh));
    ::digitalWrite( (uint8_t)m_pin, val );
    m_lastWriteVal = (digitalData_t)activeOrNot;
    return m_lastWriteVal;
};

/* CDigitalPinOut::GetPin
  Reads a pin - may not work reliably on all architectuires so maybe better to use GetLastWrite(). The return value is kActive or kNotActive NOT the raw 0 or 1 value of the pin.
*/
const digitalData_t CDigitalPinOut::GetPin( ) {
    _assert( IsValid(), AO_ERROR_CLASS_1, AO_ERROR_METHOD_2, STOP );
    // This will not care if a pin is set for PWM or not
    uint8_t val = (
                    (
                    /*if*/  (
                             ( *portOutputRegister( digitalPinToPort( (uint8_t)m_pin) )
                                         & digitalPinToBitMask( (uint8_t)m_pin )
                             )==0
                            )
                    /*then*/? 0
                    /*else*/: 1
                    )
                  );
    return (digitalData_t)((val && m_activeHigh) || (!val && !m_activeHigh));
};

/* CDigitalPinOut::TogglePin
 Toggles the value of the pin.
*/
digitalData_t CDigitalPinOut::TogglePin(){
    _assert( IsValid(), AO_ERROR_CLASS_1, AO_ERROR_METHOD_3, STOP );
    return SetPin( !(bool)GetPin());
};


const digitalData_t CDigitalPinOut::GetLastWrite(){
    return m_lastWriteVal;
}

CDigitalPinIn::CDigitalPinIn( const digitalPin_t& pin,
                              const pinInputMode& mode,
                              const bool& activeHigh ) {
    if (pin > NUM_DIGITAL_PINS-1){
        m_pin = kInvalidDigPin;
        return;
    }
    m_pin = pin;
    m_mode = mode;
    m_activeHigh = activeHigh;
    ::pinMode( (uint8_t)m_pin, m_mode );
};

const bool CDigitalPinIn::IsValid( ) {
    return (int)m_pin != kInvalidPin;
};

const digitalData_t CDigitalPinIn::GetPin( ){
    _assert( !IsValid(), AO_ERROR_CLASS_2, AO_ERROR_METHOD_1, STOP );
    const int val = ::digitalRead( (uint8_t)m_pin );
    return (digitalData_t)( m_activeHigh? val: !(bool)val );
};

CPWMPin::CPWMPin( const PWMPin_t& pin, const PWMData_t& initialValue ){
    m_pin = pin;
    m_lastWriteVal = initialValue;
    ::pinMode( m_pin, OUTPUT );
    ::analogWrite( m_pin, (int)initialValue);
};

const bool CPWMPin::IsValid( ) {
    return m_pin != kInvalidPin;
};

CPWMPin::SetPin( const PWMData_t& val ){
    _assert( IsValid(), AO_ERROR_CLASS_3, AO_ERROR_METHOD_1, STOP );
    ::analogWrite( m_pin, (int)val);
    m_lastWriteVal = val;
};

const PWMData_t CPWMPin::GetLastWrite( ){
    _assert( IsValid(), AO_ERROR_CLASS_3, AO_ERROR_METHOD_2, STOP );
    return m_lastWriteVal;
}

#endif // AOFWK_CPP


