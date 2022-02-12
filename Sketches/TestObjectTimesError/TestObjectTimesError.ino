
// Program to demonstrate a timing anomaly that I can't explain.
// (1) In Run 1, A member function is run in a loop 1000 times and measured. (t1)
// (2) The program es edited so that the member function test loop is run twice
// (3) In Run 2, each loop is timed (t2 and t3)
// Results: t3=t2 and t2>>t1
// How does adding code AFTER the first loop affect the timing of the first loop.


#pragma GCC optimize ("O0")

typedef uint8_t digitalPin_t;
typedef uint8_t digitalData_t;

class CDigitalPinOutT {
  public:
    CDigitalPinOutT( digitalPin_t pin );
    digitalData_t readPin( );
//  private:
    digitalPin_t _pin;
};

CDigitalPinOutT::CDigitalPinOutT( digitalPin_t pin ) {
  _pin = pin;
  ::pinMode( (uint8_t)_pin, OUTPUT );
};

digitalData_t CDigitalPinOutT::readPin( ) {
 //  
 uint8_t mask = digitalPinToBitMask((uint8_t)_pin );
 uint8_t port = digitalPinToPort((uint8_t)_pin );
 return (digitalData_t)(((*portOutputRegister( port ) & mask) == 0) ? 0 : 1);
 // Note calling digitalRead directly exhibits the same anomaly
 // return ::digitalRead( _pin );
};


unsigned long start, end, delta;
int val;
const int times = 1000;
uint8_t testPinT = 12;

CDigitalPinOutT *pTest = new CDigitalPinOutT((digitalPin_t)testPinT);

// Note same anomaly observed with pTest on the stack
// CDigitalPinOutT pTest( (digitalPin_t)testPinT );

#define RUN_TEST( fn ) \
  { unsigned long start, end, delta; \
    Serial.println( "" ); \
    Serial.flush(); \
    start = micros();  \
    for (int ii = 0; ii < times; ii += 1) { \
      val += (uint8_t)fn; \
    }; \
    end = micros(); \
    Serial.print("Value = "); \
    Serial.println( val ); \
    delta = end - start; \
    Serial.println( start ); \
    Serial.println( end ); \
    Serial.println( delta ); \
    Serial.print( #fn ); \
    Serial.print( " called " ); \
    Serial.print( times ); \
    Serial.print( " times: " ); \
    Serial.print( delta ); \
    Serial.println( "ms" ); \
    Serial.flush(); \
  }
  
 void setup() {
  Serial.begin( 9600 );
  while (!Serial){};
  Serial.println( " " );
  Serial.flush(); 
}

void loop() {
 
//  RUN_TEST( 1 );  // To time the scaffolding of the test. 
                  // . this shows (almost) all the time is spent 
                  // . executing the function under test.

// To demonstrate the perceived anomaly:-
// Run once with both these lines and then again with one commented out.
// When BOTH lines are present, the times of EACH run is slow 
// . ( 3108ms for 1000 loop ).
// When ONE line is run it runs twice as fast ( 1644ms for 1000 loop).
  Serial.println( pTest->_pin );
  RUN_TEST( pTest->readPin() ); 
  RUN_TEST( pTest->readPin() );


  while (true) {}; // ... and stop

}
