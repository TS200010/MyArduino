//www.elegoo.com
//2016.12.8

// Define Pins
#define BLUE 3
#define GREEN 5
#define RED 6

void setup()
{
pinMode(RED, OUTPUT);
pinMode(GREEN, OUTPUT);
pinMode(BLUE, OUTPUT);
//digitalWrite(RED, HIGH);
digitalWrite(GREEN, LOW);
digitalWrite(BLUE, LOW);
}

// define variables
int redValue;
int greenValue;
int blueValue;

// main loop
void loop()
{
#define delayTime 50 // fading time between colors

redValue = 255; // choose a value between 1 and 255 to change the color.
greenValue = 0;

// this turns RED and GREEN off
analogWrite(RED, 0);
analogWrite(GREEN, 0);
delay(200);

for(int i = 0; i < 255; i += 1) // fades out red bring green full when i=255
{
redValue -= 1;
greenValue += 1;
analogWrite(RED, redValue);
analogWrite(GREEN, greenValue);
delay(delayTime);
}

redValue = 0;
greenValue = 255;

for(int i = 0; i < 255; i += 1) // fades out green bring red full when i=255
{
greenValue -= 1;
redValue += 1;
analogWrite(GREEN, greenValue);
analogWrite(RED, redValue);
delay(delayTime);
}

redValue = 0;
greenValue = 0;
blueValue = 255;


/*for(int i = 0; i < 255; i += 1) // fades out blue bring red full when i=255
{
blueValue -= 1;
redValue += 1;
analogWrite(BLUE, blueValue);
analogWrite(RED, redValue);
delay(delayTime);
}
*/
}
