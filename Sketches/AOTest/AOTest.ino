
#include "AOConst.h"
#include "AOFwk.h"
#include "AOUtils.h"
//#include "AOFwkDebug.h"
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


// Macros used to facilitate tests and reporting

#define PASSED( n,m,ij ... ) {Serial.print(g_strPass); Serial.print(__PRETTY_FUNCTION__); Serial.print(":- "); Serial.print(n); \
                              Serial.print("-"); Serial.println(m); m+=1;}
#define FAILED( n,m,ij ... ) {Serial.print(g_strFail); Serial.print(__PRETTY_FUNCTION__); Serial.print(":- "); Serial.print(n); \
                              Serial.print("-"); Serial.print(m); Serial.print("-"); Serial.println(ij); m+=1; failedCnt+=1;}
#define STOP while (true) {};
#define prt(s, p) Serial.print( s ); Serial.print( p ); Serial.print( " " );
// #define DUMP val=::digitalRead(ii); prt("digitalRead Value ", val); val=(int)p->getPin(); prt("GetPin Value ",val);
#define DUMP 

// CDigitalPinOut *p1 = new CDigitalPinOut((digitalPin_t)0, kActiveHigh);

void loop() {

int failedCnt=0;

// New Scope for CDigitalPinOut Tests
{
  CDigitalPinOut *p;
  int TestNo=0, SubTestNo=0;
  uint8_t val;

// CDigitalPinOut::CDigitalPinOut (Constructor) Tests
// --------------------------------------------------
// Test m_pin is set correctly for range of allowed pin numbers.
  SubTestNo=1;TestNo=1;
  p = new CDigitalPinOut(kDigital2,  kActiveHigh); 
  if (p->m_pin==2)   PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo );
  if (p->m_lastWriteVal==NOT_A_DIGITAL_VALUE ) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo );
  delete p; 

  p = new CDigitalPinOut(kDigital10, kActiveHigh); 
  if (p->m_pin==10)  PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  if (p->m_lastWriteVal==NOT_A_DIGITAL_VALUE ) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo );
  delete p; 

  p = new CDigitalPinOut(kDigital19, kActiveHigh); 
  if (p->m_pin==19)  PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  if (p->m_lastWriteVal==NOT_A_DIGITAL_VALUE ) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo );
  delete p; 

// isValid() Tests
// --------------- 
  SubTestNo=1;TestNo=2;
  p = new CDigitalPinOut((digitalPin_t)20 , kActiveHigh ); 
  if (p->m_pin==kInvalidPin)  PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  if (!p->IsValid()) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ) ;
  delete p; 
  
// Test m_activeHigh set correctly
// ------------------------------
  SubTestNo=1;TestNo=3;
  p = new CDigitalPinOut(kDigital2, kActiveHigh); if (p->m_activeHigh==true)  PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); delete p;
  p = new CDigitalPinOut(kDigital2, kActiveLow);  if (p->m_activeHigh==false) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); delete p;

// CDigitalPinOut::SetPin Tests
// ----------------------------------
// Test each pin can beset and read back correctly.
// Note we cannot test pins 0 and 1 as they are being used for Serial IO.
// Using... bool value = (0!=(*portOutputRegister( digitalPinToPort(pin) ) & digitalPinToBitMask(pin)));
  SubTestNo=1; TestNo=4;
  for (int ii=2; ii<NUM_DIGITAL_PINS; ii+=1 ){
    p = new CDigitalPinOut((digitalPin_t)ii, kActiveHigh); p->SetPin( kNotActive ); DUMP; if (::digitalRead( ii )==0) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo,ii ); delete p;
    p = new CDigitalPinOut((digitalPin_t)ii, kActiveHigh); p->SetPin( kActive );    DUMP; if (::digitalRead( ii )==1) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo,ii ); delete p;
    p = new CDigitalPinOut((digitalPin_t)ii, kActiveLow);  p->SetPin( kActive );    DUMP; if (::digitalRead( ii )==0) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo,ii ); delete p;
    p = new CDigitalPinOut((digitalPin_t)ii, kActiveLow);  p->SetPin( kNotActive ); DUMP; if (::digitalRead( ii )==1) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo,ii ); delete p;
  };

// CDigitalPinOut::TogglePin Tests
// -------------------------------
// Check value is toggled for kActiveHigh and kActiveLow  
  SubTestNo=1; TestNo=5;
  p = new CDigitalPinOut(kDigital2,  kActiveHigh); p->SetPin( kActive );  
  Serial.println( p->GetPin() ); p->TogglePin(); Serial.println( p->GetPin() ); p->TogglePin(); Serial.println( p->GetPin() ); p->TogglePin(); Serial.println( p->GetPin() );
  if (p->TogglePin()==1) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  if (p->TogglePin()==0) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  delete p;
  p = new CDigitalPinOut(kDigital5,  kActiveHigh); p->SetPin( kNotActive );  
  Serial.println( p->GetPin() ); p->TogglePin(); Serial.println( p->GetPin() ); p->TogglePin(); Serial.println( p->GetPin() ); p->TogglePin(); Serial.println( p->GetPin() );
  if (p->TogglePin()==0) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  if (p->TogglePin()==1) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  delete p;
   p = new CDigitalPinOut(kDigital10,  kActiveHigh); p->SetPin( kNotActive );  
  Serial.println( p->GetPin() ); p->TogglePin(); Serial.println( p->GetPin() ); p->TogglePin(); Serial.println( p->GetPin() ); p->TogglePin(); Serial.println( p->GetPin() );
  if (p->TogglePin()==0) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  if (p->TogglePin()==1) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  delete p;
  p = new CDigitalPinOut(kDigital12,  kActiveLow); p->SetPin( kActive );  
  Serial.println( p->GetPin() ); p->TogglePin(); Serial.println( p->GetPin() ); p->TogglePin(); Serial.println( p->GetPin() ); p->TogglePin(); Serial.println( p->GetPin() );
  if (p->TogglePin()==1) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  if (p->TogglePin()==0) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  delete p;
  p = new CDigitalPinOut(kDigital19,  kActiveLow); p->SetPin( kNotActive );  
  Serial.println( p->GetPin() ); p->TogglePin(); Serial.println( p->GetPin() ); p->TogglePin(); Serial.println( p->GetPin() ); p->TogglePin(); Serial.println( p->GetPin() );
  if (p->TogglePin()==0) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  if (p->TogglePin()==1) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  delete p;

// GetLastWrite tests
// ------------------
// 
  SubTestNo=1;TestNo=6;
  p = new CDigitalPinOut(kDigital2,  kActiveHigh); p->SetPin( kActive);
  if (p->GetLastWrite()==kActive ) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo );
  ; p->SetPin( kNotActive);
  if (p->GetLastWrite()==kNotActive ) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo );
  delete p; 

  p = new CDigitalPinOut(kDigital19,  kActiveLow); p->SetPin( kActive);
  if (p->GetLastWrite()==kActive ) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo );
  ; p->SetPin( kNotActive);
  if (p->GetLastWrite()==kNotActive ) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo );
  delete p; 

}; // End of CDigitalPinOut test Scope

// New Scope for CDigitalPinIn Tests
{

// CDigitalPinIn Tests
// -------------------

  CDigitalPinIn *p;
  int TestNo=0, SubTestNo=0;
  uint8_t val;

// CDigitalPinOut::CDigitalPinOut (Constructor) Tests
// --------------------------------------------------
// Test m_pin, m_activeHigh and m_mode are set correctly for a range of allowed pin numbers.

  SubTestNo=1;TestNo=10;
  p = new CDigitalPinIn(kDigital2,  kInputPullup, kActiveHigh); 
  if (p->m_pin==2) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo );
  if( p->m_activeHigh==kActiveHigh ) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo );
  if( p->m_mode==kInputPullup ) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo );
  delete p; 

  p = new CDigitalPinIn(kDigital10, kInput, kActiveHigh); 
  if (p->m_pin==10)  PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  if( p->m_activeHigh==kActiveHigh ) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo );
  if( p->m_mode==kInput ) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo );
  delete p; 
  
  p = new CDigitalPinIn(kDigital19, kInputPullup, kActiveLow); 
  if (p->m_pin==19)  PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo ); 
  if( p->m_activeHigh==kActiveLow ) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo );
  if( p->m_mode==kInputPullup ) PASSED( TestNo,SubTestNo ) else FAILED( TestNo,SubTestNo );
  delete p; 


};

// Print Summary and stop
// ----------------------  
  Serial.print("Failed = "); Serial.println( failedCnt );

  STOP



}
