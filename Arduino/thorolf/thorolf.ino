#include <SoftwareSerial.h>

#include "FastLED.h"
#define NUM_LEDS 6
CRGB leds[NUM_LEDS];

int nstrips = 6;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

long previousMillis = 0;
long previousMillis1 = 0;
long interval = 15;

int k = 5;
int kmax = 164;
int kmin = 5;
bool fadeIn = true;
float minA = 15.0;
int blue = 0;

/********* LED modes *********/
// Mode 0: resting
// Mode 1: touchable
// Mode 2: incoming contact!
int ledMode = 0;

/********* Bluetooth settings *********/
SoftwareSerial BT(10, 11); // Bluetooth 10 RX, 11 TX.
int byteToSend; //Array that hold the values you will be sending to OF

/********* Flexie settings *********/
//const int FLEX_PIN = A5; // Pin connected to voltage divider output
int nflex = 3;
const int flexPins[] = {A5, A4, A3};
// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 47500.0; // Measured resistance of 3.3k resistor
// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.
const float STRAIGHT_RESISTANCE[] = {33487.50, 10904.45, 39272.32}; // resistance when straight
const float BEND_RESISTANCE[] = {58829.32, 30875.00, 79042.96}; // resistance at 90 deg

// For each flexie
float angle[3];
int led_brightness[3];

void setup()
{
  /********* LED settings *********/
  FastLED.addLeds<NEOPIXEL, 2>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 3>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 4>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 5>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 7>(leds, NUM_LEDS);

  // Set all LEDS green
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
  }
  FastLED.setBrightness(10);

  Serial.begin(9600);
  BT.begin(9600);

  for (int i = 0; i < 3; i++) {
    pinMode(flexPins[i], INPUT);
  }
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;
  }
  /********* Flexie loop *********/

  for (int i = 0; i < nflex; i++) {

    //Serial.println("FLEXIE #" + String(i));

    // Read the ADC, and calculate voltage and resistance from it
    int flexADC = analogRead(flexPins[i]);
    float flexV = flexADC * VCC / 1023.0;
    float flexR = R_DIV * (VCC / flexV - 1.0);
    //Serial.println("Resistance for flexie #" + String(i) + ": " + String(flexR) + " ohms");

    // Use the calculated resistance to estimate the sensor's
    // bend angle:
    angle[i] = map(flexR, STRAIGHT_RESISTANCE[i], BEND_RESISTANCE[i], 0, 90.0);
    //Serial.println("Bend for flexie #" + String(i) + ": " + String(angle[i]) + " degrees");

    // Map force to led number
    //led_angle[i] = map(angle, 0, 90.0, 0, NUM_LEDS);
    //Serial.println(led_angle[i]);
    // Map force to led brightness
    led_brightness[i] = map(angle[i], 0, 200.0, 0, 255);
    //Serial.println(led_brightness[i]);
    if (ledMode == 1) {
      // Touched
      //fill_solid(leds, NUM_LEDS, CHSV(led_brightness[i],255,255));
      blue = led_brightness[i];
    }

    /********* LED loop when touched! *********/
    // If breathing
    //    if (ledMode == 0) {
    //      // Minimum of bending is 5.0 degrees
    //      if (angle > 5.0) {
    //        ledMode = 1; // Flexie is being touched
    //        Serial.println("Touched on flexie #" + String(i));
    //        // Now map brightness
    //        Serial.println(led_brightness[i]);
    //        if (angle > 90.0) {
    //          FastLED.setBrightness(164);
    //        }
    //        else {
    //          FastLED.setBrightness(led_brightness[i]);
    //        }
    //      }
    //    }

  }
  if (angle[0] > minA || angle[1] > minA || angle[2] > minA) {
    // Touching at least one flexie
    //Serial.println("Touched a flexie");
    ledMode = 1;
  }
  else {
    // Touching none
    if (ledMode == 1) {
      ledMode = 0;
    }
  }


  /********* LED loop *********/
  if (ledMode == 0) {
    // Breathe
    fill_solid(leds, NUM_LEDS, CRGB::White);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;
      if (k < kmax) {
        // fade in or out
        if (fadeIn) {
          k++;
        }
        else {
          k--;
        }
      }
      if (k == kmax) {
        k--;
        fadeIn = false;
      }
      if (k == kmin) {
        k++;
        fadeIn = true;
      }
    }
    FastLED.setBrightness(k);
    FastLED.show();
  }
  else if (ledMode == 1) {
    // Touched
    //fill_solid(leds, NUM_LEDS, CRGB::Red);
    //sinelon();
    customrainbow();
  }
  else if (ledMode == 2) {
    // bpm();
    sinelon();
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis1 > 5000) {
      previousMillis1 = currentMillis;
      ledMode = 0;
    }
  }



  FastLED.show();

  /**************** SEND Bluetooth messages ***************/
  if (BT.available()) {
      byteToSend = ledMode;
      // Plant has been touched, so send a message
      BT.write(byteToSend);
      
      

    // Values to send must be mapped from 0 to 255 (byte)
    //byteToSend = ledMode;

    //for (int i = 0; i < 2 ; i++) { // loops through the Bytes Array
      //BT.write(byteToSend); // When using Serial Write you are sending the Info as a Byte which is needed to send info to OF
      // This means this info won't appear in your arduino serial viewer
    //}

  }
  /**************** READ Bluetooth messages ***************/
  if (BT.available()) {

    // get value from OF
    char value = BT.read();
    Serial.println(value);

    if (value == '0') {
      digitalWrite(LED_BUILTIN, LOW);
      //ledMode = 0;
    }
    if (value == '1') {
      digitalWrite(LED_BUILTIN, HIGH);
      ledMode = 2;
    }

  }

}
/**************** EXTRA FUNCTIONS ***************/

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(20);
}

void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16( i + 7, 0, NUM_LEDS - 1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(50, 0, NUM_LEDS - 1 );
  leds[pos] += CRGB::Blue;
}
void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 60;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void customrainbow()
{
  //Serial.println(blue);
  fill_solid(leds, NUM_LEDS, CHSV(140 + (0.1 * blue), 255, 255));
}

