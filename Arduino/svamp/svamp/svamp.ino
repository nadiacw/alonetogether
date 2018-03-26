#include <SoftwareSerial.h>
#include "FastLED.h"
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

#ifdef __AVR__
#include <avr/power.h>
#endif

boolean btMessage = false;
SoftwareSerial BT(10, 11); // Bluetooth 10 RX, 11 TX.

unsigned long previousMillisRead = 0;
unsigned long previousMillisUpdate = 0;
unsigned long previousBluetooth = 0;

//Frequencies (in ms) for:
//1. reading new events (BT or FSR is touched)
//2. speed of "animations"
unsigned long readingFrequency = 1000;
unsigned long updateFrequency = 20;
unsigned long bluetoothFrequency = 500;

/********* ANALOG READ TO BRIGHTNESS VALUE SETUP *********/
int fsrPin0 = 0;
int fsrPin1 = 1;
int fsrPin2 = 2;
int fsrReading0;
int fsrReading1;
int fsrReading2;

void setup(void) {
  BT.begin(9600);
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, 5>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 7>(leds, NUM_LEDS);

  // Set all LEDS green
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
  }
  FastLED.setBrightness(100);
}

/**** UPDATES ARRAY ACCORDING TO READING FROM BT/FSR ****/
void setBrightness(int index, int value) {
  int bright = map(value, 0, 1200, 20, 150);
  leds[index] += CRGB(0, 0, bright);
}

/*********** BLUETOOTH SEND/RECIEVE-FUNCTIONS ***********/

void bluetoothRead() {
  if (BT.available()) {
    char value = BT.read();

    if (value == '1') {
      btMessage = true;
    }
  }
}

void bluetoothSend(char bo) {
  if (BT.available()) {
    BT.write(bo);
  }
}

void loop() {
  unsigned long currentReadingMillis = millis();
  unsigned long currentUpdateMillis = millis();
  unsigned long currentBluetoothMillis = millis();
  fsrReading0 = analogRead(fsrPin0);
  fsrReading1 = analogRead(fsrPin1);
  fsrReading2 = analogRead(fsrPin2);

  bluetoothRead();
  
  if((fsrReading0 > 100) || (fsrReading1 > 100) || (fsrReading2 > 100)) {
    bluetoothSend(1);
  } else {
    bluetoothSend(0);
  }


  /* GET NEW BRIGHTNESS AND SET BRIGHTNESS ACCORDING TO UPDATE-PERIOD */
  if (currentUpdateMillis - previousMillisUpdate >= updateFrequency) {
    for (int i = 0; i < 3; i++) {
      leds[i].fadeLightBy(2);
    }

    FastLED.show();

    previousMillisUpdate = currentUpdateMillis;
  }

  /*********** GET NEW READINGS EVERY READING-PERIOD **************/
  if (currentReadingMillis - previousMillisRead >= readingFrequency) {
    fsrReading0 = analogRead(fsrPin0);
    fsrReading1 = analogRead(fsrPin1);
    fsrReading2 = analogRead(fsrPin2);

    if (btMessage == true) {
      btMessage = false;
     
        if (currentBluetoothMillis - previousBluetooth >= bluetoothFrequency) {
          leds[0] = CHSV(random8(),255,255);
        } else {
          leds[0] = CRGB::Blue;
        }
        previousBluetooth = currentBluetoothMillis;
    } else {

      if (fsrReading0 > 10) {
        setBrightness(0, fsrReading0);
      } 

      if (fsrReading1 > 10) {
        setBrightness(0, fsrReading1);
      } 

      if (fsrReading2 > 10) {
        setBrightness(0, fsrReading2);
      } 
    }
    previousMillisRead = currentReadingMillis;
  }
}
