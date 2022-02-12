
// using namespace ELEGOO_STARTER_UNO_R3;


// **************************************** VOID_PROGRAM
#ifdef VOID_PROGRAM
void setup() {
}

void loop() {
}
#endif

// **************************************** BLINK
#ifdef BLINK
void setup() {
  Serial.begin(9600);
  printFreeSRAM();
  pinMode( LED_BUILTIN, OUTPUT );     
}

void loop() {
  digitalWrite( LED_BUILTIN, (int)1 );   
  delay( 1000 );                
  digitalWrite( LED_BUILTIN, (int)0 );   
  delay( 1000 );                         
}
#endif // BLINK

// **************************************** BLINK_CPP
#ifdef BLINK_CPP

CDigitalPinOut g_blinkPin( kLEDBuiltIn, kActiveHigh );

void setup() {
  Serial.begin(9600);
  printFreeSRAM();
}

void loop() {
  g_blinkPin.SetPin( kActive );  
  delay( kOneSecond );                 
  g_blinkPin.SetPin( kNotActive );  
  delay( kTwoSeconds );                         
}
#endif // BLINK_CPP

// **************************************** GET CPP VERSION
#ifdef GET_CPP_VERSION
void setup() {
  Serial.begin(9600);
  while(!Serial){};
  printFreeSRAM();
  Serial.print("CPP Version = ");
  Serial.println(__cplusplus);
}
void loop() {}
#endif // GET_CPP_VERSION

// **************************************** RGB LED (Lesson 4)
#ifdef RGB_LED
// Constants User can update to explore the behaviour of the example
const uint8_t kPWMBrightnessMax = 10;            // Maximum PWM value controls maximum brightness ( 0 to kPWMMax )
const int     kTotalCycleTime = kTenSeconds;     // Number of milliseconds for the complete cycle of 
                                                 //   three colours. Note how the constants improve readability here.

// Global constants
const PWMPin_t kBlueLED  = 3;
const PWMPin_t kGreenLED = 5;
const PWMPin_t kRedLED   = 6;
const int kLEDUpdateDelay = kTotalCycleTime / (kPWMBrightnessMax + 1);

// Global variables
PWMData_t g_redLEDBrightness   = 0;
PWMData_t g_greenLEDBrightness = 0;
PWMData_t g_blueLEDBrightness  = 0;

void setup()
{
  Serial.begin( 9600 ); 
  printFreeSRAM();
  delay ( 200 );                                   // Stops brief LED flash on program loading                
  pinModeAO( kRedLED,   OUTPUT );
  pinModeAO( kGreenLED, OUTPUT );
  pinModeAO( kBlueLED,  OUTPUT );
  analogWriteAO( kRedLED, 0 ); 
  analogWriteAO( kGreenLED, 0 );
  analogWriteAO( kBlueLED,  0 );
}

void loop()
{
  for(  int i=kPWMBrightnessMax-1; i>=0; i-=1 )    // Fades out red brings green full
                                                   // Starts at -1 otherwise the same brightness will be
                                                   //   displayed twice - at start of this for loop 
                                                   //   and at the end of the last for loop
  { 
      g_redLEDBrightness = i;
      g_greenLEDBrightness = kPWMBrightnessMax - g_redLEDBrightness;
      analogWriteAO( kRedLED, g_redLEDBrightness );
      analogWriteAO( kGreenLED, g_greenLEDBrightness );
      delay( kLEDUpdateDelay );
  }
  
  for(  int i=kPWMBrightnessMax-1; i>=0; i-=1 )    // Fades out green brings blue full
  {
     g_greenLEDBrightness = i;
     g_blueLEDBrightness = kPWMBrightnessMax - g_greenLEDBrightness;
     analogWriteAO( kGreenLED, g_greenLEDBrightness );
     analogWriteAO( kBlueLED, g_blueLEDBrightness );
     delay( kLEDUpdateDelay );
   }
  
  for(  int i=kPWMBrightnessMax-1; i>=0; i-=1 )   // Fades out blue brings red full
  {
     g_blueLEDBrightness = i;
     g_redLEDBrightness = kPWMBrightnessMax - g_blueLEDBrightness;
     analogWriteAO( kBlueLED, g_blueLEDBrightness );
     analogWriteAO( kRedLED, g_redLEDBrightness );
     delay( kLEDUpdateDelay );
  }
}
#endif // RGB_LED

// **************************************** RGB LED OPTIMISED (Lesson 4)
#ifdef RGB_LED_OPTIMISED
// Constants User can update to explore the behaviour of the example
const uint8_t kPWMBrightnessMax = 10;              // Maximum PWM value controls maximum brightness ( 0 to kPWMMax )
const int     kTotalCycleTime = kTenSeconds;       // Number of milliseconds for the complete cycle of three colours. 
const int     kLEDUpdateDelay = kTotalCycleTime / (kPWMBrightnessMax + 1);     

// Global constants
const PWMPin_t kBlueLED  = kPWM3;
const PWMPin_t kGreenLED = kPWM5;
const PWMPin_t kRedLED   = kPWM6;

void setup(){
  Serial.begin( 9600 ); 
  printFreeSRAM();
  delay ( 200 );                                   // Stops brief LED flash on program loading                
  pinModeAO( kRedLED,   OUTPUT );
  pinModeAO( kGreenLED, OUTPUT );
  pinModeAO( kBlueLED,  OUTPUT );
  analogWriteAO( kRedLED, kPWMMin ); 
  analogWriteAO( kGreenLED, kPWMMin );
  analogWriteAO( kBlueLED,  kPWMMin );
}

// Function Specification to prevent IDE throwing compile errors
void crossFade( const PWMPin_t& fadeFromPin, const PWMPin_t& fadeToPin);

void loop(){
 crossFade( kRedLED, kGreenLED );
 crossFade( kGreenLED, kBlueLED );
 crossFade( kBlueLED, kRedLED );
}

void crossFade( const PWMPin_t& fadeFromPin, const PWMPin_t& fadeToPin) {
  for(  PWMData_t i=kPWMBrightnessMax-1; i>=0; i-=1 ) {// Fades out "from" brings "to" full
                                                       // Starts at -1 otherwise the same brightness will be
                                                       //   displayed twice - at start of this for loop 
                                                       //   and at the end of the last for loop
      PWMData_t fadeFromLEDBrightness = i;
      PWMData_t fadeToLEDBrightness = kPWMBrightnessMax - fadeFromLEDBrightness;
      analogWriteAO( fadeFromPin, fadeFromLEDBrightness );
      analogWriteAO( fadeToPin,   fadeToLEDBrightness );
      delay( kLEDUpdateDelay );
  }
}
#endif // RGB_LED_OPTIMISED


// **************************************** RGB LED OPTIMISED (Lesson 4) CPP
#ifdef RGB_LED_CPP

// Constants User can update to explore the behaviour of the example
const uint8_t kPWMBrightnessMax = 10;            // Maximum PWM value controls maximum brightness ( 0 to kPWMMax )
const int kTotalCycleTime = kTenSeconds;         // Number of milliseconds for the complete cycle of 
                                                  //   three colours. Note how the constants improve readability here.
const int kLEDUpdateDelay = kTotalCycleTime / (kPWMBrightnessMax + 1); 


CPWMPin  g_BlueLED(  kPWM3, kActiveHigh );
CPWMPin  g_GreenLED( kPWM5, kActiveHigh );
CPWMPin  g_RedLED(   kPWM6, kActiveHigh );

void setup(){
  Serial.begin(9600);
  printFreeSRAM();
  delay ( 200 );                            // Stops brief LED flash on program loading                
}

// Function Specification to prevent IDE throwing compile errors
void crossFade( const CPWMPin& fadeFromPin, const CPWMPin& fadeToPin);  

void loop(){
  crossFade( g_RedLED, g_GreenLED );
  crossFade( g_GreenLED, g_BlueLED );
  crossFade( g_BlueLED, g_RedLED );
 }

void crossFade( const CPWMPin& fadeFromPin, const CPWMPin& fadeToPin) {
  for(  PWMData_t i=kPWMBrightnessMax-1; i>=0; i-=1 ) {// Fades out "from" brings "to" full
                                                 // Starts at -1 otherwise the same brightness will be
                                                 //   displayed twice - at start of this for loop 
                                                 //   and at the end of the previous for loop
      PWMData_t fadeFromLEDBrightness = i;
      PWMData_t fadeToLEDBrightness = kPWMBrightnessMax - fadeFromLEDBrightness;
      fadeFromPin.SetPin( fadeFromLEDBrightness );
      fadeToPin.SetPin(   fadeToLEDBrightness   );
      delay( kLEDUpdateDelay );
  }
}
#endif // RGB_LED_CPP

// **************************************** DIGITAL INPUTS (Lesson 5) 
#ifdef DIGITAL_INPUTS
int ledPin = 5;
int buttonApin = 9;
int buttonBpin = 8;

//byte leds = 0;

void setup() 
{
  pinMode(ledPin, OUTPUT);
  pinMode(buttonApin, INPUT_PULLUP);  
  pinMode(buttonBpin, INPUT_PULLUP);  
}

void loop() 
{
  if (digitalRead(buttonApin) == LOW)
  {
    digitalWrite(ledPin, HIGH);
  }
  if (digitalRead(buttonBpin) == LOW)
  {
    digitalWrite(ledPin, LOW);
  }
}
#endif // DIGITAL_INPUTS

// **************************************** DIGITAL INPUTS (Lesson 5)  CPP Version
#ifdef DIGITAL_INPUTS_CPP
CDigitalPinOut g_LEDPin(    kDigital5, kActiveHigh);
CDigitalPinIn  g_buttonApin(kDigital9, kInputPullup, kActiveLow);
CDigitalPinIn  g_buttonBpin(kDigital8, kInputPullup, kActiveLow);

void setup() 
{
  Serial.begin(9600);
  printFreeSRAM();               
}

void loop() 
{
  if (g_buttonApin.GetPin() == kActive) {
    g_LEDPin.SetPin( kActive );
  }
  if (g_buttonBpin.GetPin() == kActive) {
    g_LEDPin.SetPin( kNotActive );
  }
}

#endif // DIGITAL_INPUTS_CPP

