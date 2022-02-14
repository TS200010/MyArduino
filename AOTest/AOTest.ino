
#include "AOConst.h"
#include "AOFwk.h"
#include "AOUtils.h"
#include "AOFwkDebug.h"
#include "pgmStrToRAM.h"

String g_strPass = "Passed CDigitalPinOut Test "; 
String g_strFail = "FAILED CDigitalPinOut Test "; 

void setup() {
  Serial.begin(9600);
  printFreeSRAM();

  Serial.println( NUM_DIGITAL_PINS );
  
//  for ( int i=0; i<=20; i++ ) {
//    uint8_t mask = digitalPinToBitMask( i );
//    Serial.print( i ); Serial.print( " " ); Serial.println( mask );
//  };
}

CDigitalPinOut *p1 = new CDigitalPinOut((digitalPin_t)0, ACTIVE_HIGH);

#define PASSED( n,m,ij ... ) {Serial.print(g_strPass); Serial.print(__PRETTY_FUNCTION__); Serial.print(":- "); Serial.print(n); \
                              Serial.print("-"); Serial.println(m); m+=1;}
#define FAILED( n,m,ij ... ) {Serial.print(g_strFail); Serial.print(__PRETTY_FUNCTION__); Serial.print(":- "); Serial.print(n); \
                              Serial.print("-"); Serial.print(m); Serial.print("-"); Serial.println(ij); m+=1; failedCnt+=1;}
#define STOP while (true) {};

void loop() {

  CDigitalPinOut *p;
  int TestNo=0, SubTestNo=0, failedCnt=0;
  uint8_t val;

// CDigitalPinOut::CDigitalPinOut (Constructor) Tests
// ------------------------------------
// Test m_pin is set correctly for range of allowed pin numbers.
  SubTestNo=1;TestNo=1;
  p = new CDigitalPinOut(DIGITAL0,  ACTIVE_HIGH); if ((uint8_t)p->m_pin==0)   PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); delete p; 
  p = new CDigitalPinOut(DIGITAL10, ACTIVE_HIGH); if ((uint8_t)p->m_pin==10)  PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); delete p; 
  p = new CDigitalPinOut(DIGITAL19, ACTIVE_HIGH); if ((uint8_t)p->m_pin==19)  PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); delete p; 

// isValid() Tests
// --------------- 
  SubTestNo=1;TestNo=2;
  p = new CDigitalPinOut((digitalPin_t)20 , ACTIVE_HIGH ); 
  if ((uint8_t)p->m_pin==NOT_A_PIN)  PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  if (!p->isValid()) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ) ;
  delete p; 
  
// Test m_activeHigh set correctly
// ------------------------------
  SubTestNo=1;TestNo=3;
  p = new CDigitalPinOut(DIGITAL0, ACTIVE_HIGH); if (p->m_activeHigh==true)  PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); delete p;
  p = new CDigitalPinOut(DIGITAL0, ACTIVE_LOW);  if (p->m_activeHigh==false) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); delete p;

// CDigitalPinOut::digitalWrite Tests
// ----------------------------------
// Test each pin can be read back correctly
// Using... bool value = (0!=(*portOutputRegister( digitalPinToPort(pin) ) & digitalPinToBitMask(pin)));
#define prt(s, p) Serial.print( s ); Serial.print( p ); Serial.print( " " );
#define DUMP val=::digitalRead(ii); prt("digitalRead Value ", val); val=(int)p->readPin(); prt("readPin Value ",val);

  SubTestNo=1; TestNo=4;
  for (int ii=0; ii<NUM_DIGITAL_PINS; ii+=1 ){
    p = new CDigitalPinOut((digitalPin_t)ii, ACTIVE_HIGH); p->writePin( NOT_ACTIVE ); DUMP; if (::digitalRead( ii )==0) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo,ii ); delete p;
    p = new CDigitalPinOut((digitalPin_t)ii, ACTIVE_HIGH); p->writePin( ACTIVE );     DUMP; if (::digitalRead( ii )==1) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo,ii ); delete p;
    p = new CDigitalPinOut((digitalPin_t)ii, ACTIVE_LOW);  p->writePin( ACTIVE );     DUMP; if (::digitalRead( ii )==0) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo,ii ); delete p;
    p = new CDigitalPinOut((digitalPin_t)ii, ACTIVE_LOW);  p->writePin( NOT_ACTIVE ); DUMP; if (::digitalRead( ii )==1) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo,ii ); delete p;
  };

// CDigitalPinOut::togglePin Tests
// -------------------------------
// Check value is toggled for ACTIVE_HIGH and _LOW pins 
  SubTestNo=1; TestNo=5;
  p = new CDigitalPinOut(DIGITAL0,  ACTIVE_HIGH); p->writePin( ACTIVE );  
  Serial.println( (uint8_t)p->readPin() ); p->togglePin(); Serial.println( (uint8_t)p->readPin() ); p->togglePin(); Serial.println( (uint8_t)p->readPin() ); p->togglePin(); Serial.println( (uint8_t)p->readPin() );
  if ((uint8_t)p->togglePin()==1) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  if ((uint8_t)p->togglePin()==0) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  delete p;
  p = new CDigitalPinOut(DIGITAL5,  ACTIVE_HIGH); p->writePin( NOT_ACTIVE );  
  Serial.println( (uint8_t)p->readPin() ); p->togglePin(); Serial.println( (uint8_t)p->readPin() ); p->togglePin(); Serial.println( (uint8_t)p->readPin() ); p->togglePin(); Serial.println( (uint8_t)p->readPin() );
  if ((uint8_t)p->togglePin()==0) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  if ((uint8_t)p->togglePin()==1) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  delete p;
  p = new CDigitalPinOut(DIGITAL10,  ACTIVE_LOW); p->writePin( ACTIVE );  
  Serial.println( (uint8_t)p->readPin() ); p->togglePin(); Serial.println( (uint8_t)p->readPin() ); p->togglePin(); Serial.println( (uint8_t)p->readPin() ); p->togglePin(); Serial.println( (uint8_t)p->readPin() );
  if ((uint8_t)p->togglePin()==1) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  if ((uint8_t)p->togglePin()==0) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  delete p;
  p = new CDigitalPinOut(DIGITAL19,  ACTIVE_LOW); p->writePin( NOT_ACTIVE );  
  Serial.println( (uint8_t)p->readPin() ); p->togglePin(); Serial.println( (uint8_t)p->readPin() ); p->togglePin(); Serial.println( (uint8_t)p->readPin() ); p->togglePin(); Serial.println( (uint8_t)p->readPin() );
  if ((uint8_t)p->togglePin()==0) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  if ((uint8_t)p->togglePin()==1) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  delete p;

 // p = new CDigitalPinOut(DIGITAL10, ACTIVE_HIGH); 
 // p = new CDigitalPinOut(DIGITAL19, ACTIVE_HIGH); if ((uint8_t)p->m_pin==19)  PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); delete p; 

// Print Summary and stop
// ----------------------  
  Serial.print("Failed = "); Serial.println( failedCnt );

  STOP



}
