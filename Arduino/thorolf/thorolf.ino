#include <SoftwareSerial.h>

#include "FastLED.h"
#define NUM_LEDS_PER_STRIP 6
CRGB leds[NUM_LEDS_PER_STRIP];

int nstrips = 6;

/********* Bluetooth settings *********/
SoftwareSerial BT(10, 11); // Bluetooth 10 RX, 11 TX.
int bytesToSend[2]; //Array that hold the values you will be sending to OF

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
const float STRAIGHT_RESISTANCE[] = {10417.16, 32818.18, 30372.60}; // resistance when straight
const float BEND_RESISTANCE[] = {20461.54, 86363.63, 86363.63}; // resistance at 90 deg

void setup()
{
  /********* LED settings *********/
  FastLED.addLeds<NEOPIXEL, 2>(leds, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 3>(leds, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 4>(leds, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 5>(leds, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 7>(leds, NUM_LEDS_PER_STRIP);

  // Set al LEDS green
  for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    leds[i] = CRGB::Cyan;
  }
  FastLED.setBrightness(0);

  Serial.begin(9600);
  BT.begin(9600);

  for (int i = 0; i < 3; i++) {
    pinMode(flexPins[i], INPUT);
  }
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  
  /********* Flexie loop *********/

  // For each flexie
  int led_angle[nflex];
  int led_brightness[nflex];

  for (int i = 0; i < nflex; i++) {

    //Serial.println("FLEXIE #" + String(i));

    // Read the ADC, and calculate voltage and resistance from it
    int flexADC = analogRead(flexPins[i]);
    float flexV = flexADC * VCC / 1023.0;
    float flexR = R_DIV * (VCC / flexV - 1.0);
    //Serial.println("Resistance for flexie #" + String(i) + ": " + String(flexR) + " ohms");

    // Use the calculated resistance to estimate the sensor's
    // bend angle:
    float angle = map(flexR, STRAIGHT_RESISTANCE[i], BEND_RESISTANCE[i], 0, 90.0);
    //Serial.println("Bend for flexie #" + String(i) + ": " + String(angle) + " degrees");

    // Map force to led number
    led_angle[i] = map(angle, 0, 90.0, 0, NUM_LEDS_PER_STRIP);
    //Serial.println(led_angle[i]);
    // Map force to led brightness
    led_brightness[i] = map(angle, 0, 90.0, 0, 164);
    //}

    /********* LED loop *********/

    //for (int i = 0; i < nflex; i++) {
    // Minimum of bending is 10.0 degrees
    if (angle > 10.0) {
      Serial.println("Touched on flexie #" + String(i));
      // Now map brightness
      Serial.println(led_brightness[i]);
      FastLED.setBrightness(led_brightness[i]);
    }
  }



  //if (led_angle[0] > 1 || led_angle[1] > 1 || led_angle[2] > 1) {
  //for (int l = 0; l < nflex; l++) {
  //int l = 2;
  //    if (led_angle[l] > 1) {
  //      for (int i = 0; i < led_angle[l]; i++) {
  //        leds[i] = CRGB::Green;
  //      }
  //      for (int i = led_angle[l]; i < NUM_LEDS_PER_STRIP; i++) {
  //        leds[i] = CRGB::Black;
  //      }
  //      FastLED.show();
  //    }

  // }


  //  for (int i = 0; i < led_angle; i++) {
  //    pixels.setPixelColor(i, pixels.Color(225, 225, 10 + (i + 1) * 10));
  //
  //  }
  //  for (int i = led_angle; i < NUMPIXELS; i++) {
  //    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  //
  //  }
  //
  //  pixel0.show();
  //  pixel1.show();
  //  pixel2.show();
  //  pixel3.show();
  //  pixel4.show();
  //  pixel5.show();

FastLED.show();

  /**************** SEND Bluetooth messages ***************/
  //  if (BT.available()) {
  //
  //    bytesToSend[0] = map(angle, 0, 90, 0, 255);
  //    bytesToSend[1] = 0;
  //
  //    for (int i = 0; i < 2 ; i++) { // loops through the Bytes Array
  //      BT.write(bytesToSend[i]); // When using Serial Write you are sending the Info as a Byte which is needed to send info to OF
  //      // This means this info won't appear in your arduino serial viewer
  //    }
  //
  //  }
  /**************** READ Bluetooth messages ***************/
  if (BT.available()) {

    // get value from OF
    char value = BT.read();
    Serial.println(value);

    if (value == '0') {
      digitalWrite(LED_BUILTIN, HIGH);
      //pixels.setBrightness(255);
    }
    if (value == '1') {
      digitalWrite(LED_BUILTIN, LOW);
      //pixels.setBrightness(20);
    }

  }

}

