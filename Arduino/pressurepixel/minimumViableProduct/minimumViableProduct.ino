#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN0            5
#define PIN1            6
#define PIN2            7

#define NUMPIXELS      1

int fsrPin0 = 0;
int fsrPin1 = 1;
int fsrPin2 = 2;
int fsrReading0;
int fsrReading1;
int fsrReading2;
int brightness0;
int brightness1;
int brightness2;
int lightMode = 0;
char btSendSignal = '1';

SoftwareSerial BT(10, 11); // Bluetooth 10 RX, 11 TX.

unsigned long previousMillis = 0;
unsigned long previousBtMillis = 0;
unsigned long previousBtRecieve = 0;
unsigned long interval = 1;
unsigned long bluetoothInterval = 3000;
unsigned long bluetoothRecieveInterval = 2000;

Adafruit_NeoPixel pixel0 = Adafruit_NeoPixel(1, PIN0, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel1 = Adafruit_NeoPixel(1, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel2 = Adafruit_NeoPixel(1, PIN2, NEO_GRB + NEO_KHZ800);

void setup(void) {
  BT.begin(9600);
  Serial.begin(9600);
  pixel0.begin();
  pixel1.begin();
  pixel2.begin();
}

boolean bluetoothRead() {
  if (BT.available()) {

    char value = BT.read();

    if (value == '1') {
      return true;
    } else {
      return false;
    }
  }
}

void bluetoothSend(char bo) {
  if (BT.available()) {
    BT.write(bo); // When using Serial Write you are sending the Info as a Byte which is needed to send info to OF
  }
}

void loop() {
  unsigned long currentMillis = millis();

  if (bluetoothRead()) {
    lightMode = 1;
    brightness0 = 255;
    brightness1 = 255;
    brightness2 = 255;
  }

  fsrReading0 = analogRead(fsrPin0);
  fsrReading1 = analogRead(fsrPin1);
  fsrReading2 = analogRead(fsrPin2);

  int map0 = map(fsrReading0, 0, 1000, 20, 255);

  if (map0 > 20) {
    brightness0 = map0;
    if (currentMillis - previousBtMillis >= bluetoothInterval) {
      previousBtMillis = currentMillis;
      bluetoothSend(btSendSignal);
    }
  }

  int map1 = map(fsrReading1, 0, 1000, 20, 255);

  if (map1 > 20) {
    brightness1 = map1;
    if (currentMillis - previousBtMillis >= bluetoothInterval) {
      previousBtMillis = currentMillis;
      bluetoothSend(btSendSignal);
    }
  }

  int map2 = map(fsrReading2, 0, 1000, 20, 255);

  if (map2 > 20) {
    brightness2 = map2;
    if (currentMillis - previousBtMillis >= bluetoothInterval) {
      previousBtMillis = currentMillis;
      bluetoothSend(btSendSignal);
    }
  }

  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  pixel0.setPixelColor(0, pixel0.Color(0, 150, 0)); // Moderately bright green color.
  pixel1.setPixelColor(0, pixel1.Color(0, 150, 0)); // Moderately bright green color.
  pixel2.setPixelColor(0, pixel2.Color(0, 150, 0)); // Moderately bright green color.

  if (lightMode == 0) {
    if (currentMillis - previousMillis >= interval) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;

      if (brightness0 > 20) {
        brightness0 = pixel0.getBrightness() - 1;
        pixel0.setBrightness(brightness0);
      } else {
        pixel0.setBrightness(map0);
      }

      if (brightness1 > 20) {
        brightness1 = pixel1.getBrightness() - 1;
        pixel1.setBrightness(brightness1);
      } else {
        pixel1.setBrightness(map1);
      }

      if (brightness2 > 20) {
        brightness2 = pixel2.getBrightness() - 1;
        pixel2.setBrightness(brightness2);
      } else {
        pixel2.setBrightness(map2);
      }
      pixel0.show(); // This sends the updated pixel color to the hardware.
      pixel1.show();
      pixel2.show();
    }
  } else if (lightMode == 1) {
    previousMillis = currentMillis;

    if (currentMillis - previousMillis >= interval) {

      if (brightness0 >= 20) {
        Serial.println(brightness0);
        brightness0 = pixel0.getBrightness() - 1;
        pixel0.setBrightness(brightness0);
        pixel1.setBrightness(brightness0);
        pixel2.setBrightness(brightness0);
      } else {
        Serial.println("hello");
        pixel0.setBrightness(map0);
        pixel1.setBrightness(map1);
        pixel2.setBrightness(map2);
        lightMode = 0;
      }
      pixel0.show(); // This sends the updated pixel color to the hardware.
      pixel1.show();
      pixel2.show();
    }

  }

}
