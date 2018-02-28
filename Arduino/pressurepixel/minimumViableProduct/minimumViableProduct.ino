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
int brightness0 = 20;
int brightness1 = 20;
int brightness2 = 20;
int led0State;
int led1State;
int led2State;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long windDown = 1;
unsigned long windUp = 1;

Adafruit_NeoPixel pixels[] = {
  Adafruit_NeoPixel(1, PIN0, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(1, PIN1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(1, PIN2, NEO_GRB + NEO_KHZ800),
};


void setup(void) {
  Serial.begin(9600);
  pixels[0].begin();
  pixels[1].begin();
  pixels[2].begin();
  
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  pixels[0].setPixelColor(0, pixels[0].Color(0, 150, 0)); // Moderately bright green color.
  pixels[1].setPixelColor(0, pixels[1].Color(0, 150, 0)); // Moderately bright green color.
  pixels[2].setPixelColor(0, pixels[2].Color(0, 150, 0)); // Moderately bright green color.
}

void update() {
  if (currentMillis - previousMillis >= windDown) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    if (brightness0 > 20) {
      brightness0 = pixels[0].getBrightness() - 1;
      pixels[0].setBrightness(brightness0);
    } else {
      pixels[0].setBrightness(20);
    }

    if (brightness1 > 20) {
      brightness1 = pixels[1].getBrightness() - 1;
      pixels[1].setBrightness(brightness1);
    } else {
      pixels[1].setBrightness(20);
    }

    if (brightness2 > 20) {
      brightness2 = pixels[2].getBrightness() - 1;
      pixels[2].setBrightness(brightness2);
    } else {
      pixels[2].setBrightness(20);
    }

    pixels[0].show(); // This sends the updated pixel color to the hardware.
    pixels[1].show();
    pixels[2].show();

  }
}

void loop() {
  currentMillis = millis();
  
  fsrReading0 = analogRead(fsrPin0);
  fsrReading1 = analogRead(fsrPin1);
  fsrReading2 = analogRead(fsrPin2);

  int map0 = map(fsrReading0, 0, 1000, 20, 255);

  if (brightness0 > 20) {
    update();
  } else {
    brightness0 = map0;
  }

  int map1 = map(fsrReading1, 0, 1000, 20, 255);

  if (brightness1 > 20) {
    update();
  } else {
    brightness1 = map1;
  }

  int map2 = map(fsrReading2, 0, 1000, 20, 255);

  if (brightness2 > 20) {
    update();
  } else {
    brightness2 = map2;
  }

}
