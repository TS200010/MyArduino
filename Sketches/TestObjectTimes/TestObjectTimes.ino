#include "AOFwk.h"
#include "AOConst.h"

 class CDigitalPinOutT {
  public:
    CDigitalPinOutT( digitalPin_t pin, bool activeHigh );
    digitalData_t readPin( );
    digitalData_t readPin1( );
    digitalData_t readPin2( );
    digitalData_t readPin3( );
    digitalData_t readPin4( );
    digitalPin_t _pin;
    bool _activeHigh;
};

CDigitalPinOutT::CDigitalPinOutT( digitalPin_t pin, bool pActiveHigh ) {
  _pin = pin;
  _activeHigh = pActiveHigh;
  ::pinMode( (uint8_t) _pin, OUTPUT );
};

digitalData_t CDigitalPinOutT::readPin( ) {
  uint8_t mask = digitalPinToBitMask((uint8_t)_pin );
  uint8_t port = digitalPinToPort((uint8_t)_pin );
  return (digitalData_t)(((*portOutputRegister( port ) & mask) == 0) ? 0 : 1);
};

digitalData_t CDigitalPinOutT::readPin1( ) {
    return (digitalData_t)(
                            (
                              /*if*/ (
                                        ( *portOutputRegister( digitalPinToPort((uint8_t)_pin) )
                                          & digitalPinToBitMask((uint8_t)_pin )
                                        )==0
                                      )
                               /*then*/? LOW
                               /*else*/: HIGH
                             )
                          );
};

digitalData_t CDigitalPinOutT::readPin2( ) {
  uint8_t mask = digitalPinToBitMask((uint8_t)_pin );
  return (digitalData_t)0;
};


digitalData_t CDigitalPinOutT::readPin3( ) {
  uint8_t port = digitalPinToPort((uint8_t)_pin );
  return (digitalData_t)0;
};

digitalData_t CDigitalPinOutT::readPin4( ) {
  uint8_t mask = digitalPinToBitMask((uint8_t)_pin );
  uint8_t port = digitalPinToPort((uint8_t)_pin );
};

int digitalReadT(uint8_t pin)
{
  uint8_t mask = digitalPinToBitMask((uint8_t)pin );
  uint8_t port = digitalPinToPort((uint8_t)pin );
  return (((*portOutputRegister( port ) & mask) == 0) ? 0 : 1);
};


int digitalReadT1(uint8_t pin)
{
  return 0;
};

int digitalReadT2(uint8_t pin)
{
  uint8_t mask = digitalPinToBitMask(pin);
  return 0;
};

int digitalReadT3(uint8_t pin)
{
  uint8_t port = digitalPinToPort(pin);
  return 0;
};

int digitalReadT4(uint8_t pin)
{
  uint8_t mask = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  return 0;
};

void setup() {
  Serial.begin(9600);
  Serial.println( " " );
  Serial.println( " " );
}


unsigned long start, end, delta;
int val;
const int times = 1000;
uint8_t testPin = 0;
uint8_t testPinAO = 1;
uint8_t testPinT = 2;

CDigitalPinOutT *pTest = new CDigitalPinOutT((digitalPin_t)testPinT, ACTIVE_HIGH);
CDigitalPinOut *pAO = new CDigitalPinOut((digitalPin_t)testPinAO, ACTIVE_HIGH);

#define RUN_TEST( fn ) \
  start = micros(); \
  for (int ii = 0; ii < times; ii += 1) { \
    val = (uint8_t)fn; \
  }; \
  end = micros();\
  delta = end - start; \
  Serial.print( #fn ); \
  Serial.print( " called " ); \
  Serial.print( times ); \
  Serial.print( " times: " ); \
  Serial.print( delta ); \
  Serial.println( "ms" ); \
  
void loop() {


// #pragma optimize( "", off )
  // Main Test
RUN_TEST( pAO->readPin() ); 
RUN_TEST( pAO->readPin() );

/*  pinMode( testPin, OUTPUT );
  RUN_TEST( digitalRead( testPin ) );
*/
// Run test on locally programmed AO
 //RUN_TEST( pTest->readPin() );
/*  RUN_TEST( pTest->readPin1() );
  RUN_TEST( pTest->readPin2() );
  RUN_TEST( pTest->readPin3() );
  RUN_TEST( pTest->readPin4() );
*/

// Run Tests on locally programmed read functions
//  RUN_TEST( digitalReadT( testPin ) );
/*  RUN_TEST( digitalReadT1( testPin ) );
  RUN_TEST( digitalReadT2( testPin ) );
  RUN_TEST( digitalReadT3( testPin ) );
  RUN_TEST( digitalReadT4( testPin ) );
*/

  while (true) {};

}
