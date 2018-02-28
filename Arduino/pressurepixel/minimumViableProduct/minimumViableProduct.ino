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

unsigned long previousMillis = 0;
unsigned long interval = 1;

Adafruit_NeoPixel pixel0 = Adafruit_NeoPixel(1, PIN0, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel1 = Adafruit_NeoPixel(1, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel2 = Adafruit_NeoPixel(1, PIN2, NEO_GRB + NEO_KHZ800);


void setup(void) {
  Serial.begin(9600);
  pixel0.begin();
  pixel1.begin();
  pixel2.begin();
}

void loop() {
  Serial.println(brightness0);
  unsigned long currentMillis = millis();

  fsrReading0 = analogRead(fsrPin0);
  fsrReading1 = analogRead(fsrPin1);
  fsrReading2 = analogRead(fsrPin2);

  int map0 = map(fsrReading0, 0, 1000, 20, 255);

  if (map0 > 25) {
    brightness0 = map0;
  }

  int map1 = map(fsrReading1, 0, 1000, 20, 255);

  if (map1 > 25) {
    brightness1 = map1;
  }

  int map2 = map(fsrReading2, 0, 1000, 20, 255);

  if (map2 > 25) {
    brightness2 = map2;
  }

  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  pixel0.setPixelColor(0, pixel0.Color(0, 150, 0)); // Moderately bright green color.
  pixel1.setPixelColor(0, pixel1.Color(0, 150, 0)); // Moderately bright green color.
  pixel2.setPixelColor(0, pixel2.Color(0, 150, 0)); // Moderately bright green color.

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    if (brightness0 > 10) {
      brightness0 = pixel0.getBrightness() - 1;
      pixel0.setBrightness(brightness0);
    } else {
      pixel0.setBrightness(map0);
    }

    if (brightness1 > 10) {
      brightness1 = pixel1.getBrightness() - 1;
      pixel1.setBrightness(brightness1);
    } else {
      pixel1.setBrightness(map1);
    }

    if (brightness2 > 10) {
      brightness2 = pixel2.getBrightness() - 1;
      pixel2.setBrightness(brightness2);
    } else {
      pixel2.setBrightness(map2);
    }

    pixel0.show(); // This sends the updated pixel color to the hardware.
    pixel1.show();
    pixel2.show();

  }

}
