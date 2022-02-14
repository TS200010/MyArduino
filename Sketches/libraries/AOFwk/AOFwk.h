#ifndef AOFWK_H
#define AOFWK_H

#include "AOConst.h"

// DIGITAL PIN CLASSES
// -------------------

#define AOFWK_TEST
#ifdef AOFWK_TEST
#define MAKE_PUBLIC_FOR_TESTING public
#else
#define MAKE_PUBLIC_FOR_TESTING private
#endif

// inline PWMPin_t operator= (int rhs){ return (PWMPin_t)rhs; };

// inline bool operator==(const digitalPin_t& lhs, const uint8_t& rhs){ return (int)lhs==(int)rhs; };
// inline bool operator!=(const digitalPin_t& lhs, const uint8_t& rhs){ !(lhs==rhs); };

class CPin{
MAKE_PUBLIC_FOR_TESTING:
    virtual const bool IsValid() = 0;
};

class CDigitalPinOut : public CPin {
  public:
                        CDigitalPinOut( const digitalPin_t& pin, const bool& activeHigh ) ;
    const bool          IsValid();
    digitalData_t       SetPin( const bool& activeOrNot );
    const digitalData_t GetPin();
    digitalData_t       TogglePin();
    const digitalData_t GetLastWrite();
  MAKE_PUBLIC_FOR_TESTING:
    digitalData_t       m_lastWriteVal;
    bool                m_activeHigh;
    digitalPin_t        m_pin;
};

class CDigitalPinIn : public CPin {
  public:
                        CDigitalPinIn( const digitalPin_t& pin, const pinInputMode& mode, const bool& activeHigh );
    const bool          IsValid();
    const digitalData_t GetPin();
  MAKE_PUBLIC_FOR_TESTING:
    digitalPin_t        m_pin;
    bool                m_activeHigh;
    pinInputMode        m_mode;
};

class CPWMPin : public CPin {
  public:
                        CPWMPin( const PWMPin_t& pin, const PWMData_t& init);
    const bool          IsValid();
                        SetPin(  const PWMData_t& val );
    const PWMData_t     GetLastWrite();
  MAKE_PUBLIC_FOR_TESTING:
    PWMPin_t            m_pin;
    PWMData_t           m_lastWriteVal;
};

#endif AOFWK_H
