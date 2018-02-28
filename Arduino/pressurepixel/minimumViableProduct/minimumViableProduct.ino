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

Adafruit_NeoPixel pixel0 = Adafruit_NeoPixel(1, PIN0, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel1 = Adafruit_NeoPixel(1, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixel2 = Adafruit_NeoPixel(1, 7, NEO_GRB + NEO_KHZ800);


void setup(void) {
  Serial.begin(9600);
  pixel0.begin();
  pixel1.begin();
  pixel2.begin();
}

void loop() {
  fsrReading0 = analogRead(fsrPin0);
  fsrReading1 = analogRead(fsrPin1);
  fsrReading2 = analogRead(fsrPin2);

  int map0 = map(fsrReading0, 0, 1000, 0, 255);
  int map1 = map(fsrReading1, 0, 1000, 0, 255);
  int map2 = map(fsrReading2, 0, 1000, 0, 255);

  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixel0.setPixelColor(i, pixel0.Color(0,150,0)); // Moderately bright green color.
    pixel1.setPixelColor(i, pixel1.Color(0,150,0)); // Moderately bright green color.
    pixel2.setPixelColor(i, pixel2.Color(0,150,0)); // Moderately bright green color.

    pixel0.setBrightness(map0);
    pixel1.setBrightness(map1);
    pixel2.setBrightness(map2);

    pixel0.show(); // This sends the updated pixel color to the hardware.
    pixel1.show();
    pixel2.show();

  
  delay(300);
  }
}
