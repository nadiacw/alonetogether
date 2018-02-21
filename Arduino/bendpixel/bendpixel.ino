#include <SoftwareSerial.h>

#include <Adafruit_NeoPixel.h>

#include <avr/power.h>

//#define PIN            3
#define NUMPIXELS      6

int nstrips = 6;
int pixelPins[] = {0, 1, 2, 3, 4, 5};

/********* LED settings *********/
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel0 = Adafruit_NeoPixel(NUMPIXELS, pixelPins[0], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel1 = Adafruit_NeoPixel(NUMPIXELS, pixelPins[1], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel2 = Adafruit_NeoPixel(NUMPIXELS, pixelPins[2], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel3 = Adafruit_NeoPixel(NUMPIXELS, pixelPins[3], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel4 = Adafruit_NeoPixel(NUMPIXELS, pixelPins[4], NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel5 = Adafruit_NeoPixel(NUMPIXELS, pixelPins[5], NEO_GRB + NEO_KHZ800);
int delayval = 3000; // delay for half a second

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
const float STRAIGHT_RESISTANCE = 10348.21; // resistance when straight
const float BEND_RESISTANCE = 20461.54; // resistance at 90 deg

void setup()
{
  Serial.begin(9600);
  BT.begin(9600);

  for (int i = 0; i < 3; i++) {
    pinMode(flexPins[i], INPUT);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  pixel0.begin();
  pixel1.begin();
  pixel2.begin();
  pixel3.begin();
  pixel4.begin();
  pixel5.begin();
}

void loop()
{
  /********* Flexie loop *********/
  
  // For each flexie
  int led_angle;
  for (int i = 0; i < nflex; i++) {

    // Read the ADC, and calculate voltage and resistance from it
    int flexADC = analogRead(flexPins[i]);
    float flexV = flexADC * VCC / 1023.0;
    float flexR = R_DIV * (VCC / flexV - 1.0);
    //Serial.println("Resistance: " + String(flexR) + " ohms");

    // Use the calculated resistance to estimate the sensor's
    // bend angle:
    float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                      0, 90.0);
    //Serial.println("Bend: " + String(angle) + " degrees");
    //Serial.println();

    led_angle = map(angle, 0, 90.0, 0, NUMPIXELS);
  }
  /********* LED loop *********/
  for (int i = 0; i < led_angle; i++) {
    pixels.setPixelColor(i, pixels.Color(225, 225, 10 + (i + 1) * 10));

  }
  for (int i = led_angle; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));

  }

  pixel0.show();
  pixel1.show();
  pixel2.show();
  pixel3.show();
  pixel4.show();
  pixel5.show();


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

