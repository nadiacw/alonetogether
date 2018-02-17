#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN            3
#define NUMPIXELS      8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


const int FSR_PIN = A4; // Pin connected to FSR/resistor divider

// Measure the voltage at 5V and resistance of your 3.3k resistor, and enter
// their value's below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 3230.0; // Measured resistance of 3.3k resistor

float max_f = 150;
float min_f = 0.47;

void setup() 
{
  Serial.begin(9600);
  pinMode(FSR_PIN, INPUT);
  
    pixels.begin(); // This initializes the NeoPixel library.
  pixels.setBrightness(20);
}

void loop() 
{
  int fsrADC = analogRead(FSR_PIN);
  // If the FSR has no pressure, the resistance will be
  // near infinite. So the voltage should be near 0.
  if (fsrADC != 0) // If the analog reading is non-zero
  {
    // Use ADC reading to calculate voltage:
    float fsrV = fsrADC * VCC / 1023.0;
    // Use voltage and static resistor value to 
    // calculate FSR resistance:
    float fsrR = R_DIV * (VCC / fsrV - 1.0);
    Serial.println("Resistance: " + String(fsrR) + " ohms");
    // Guesstimate force based on slopes in figure 3 of
    // FSR datasheet:
    float force;
    float fsrG = 1.0 / fsrR; // Calculate conductance
    // Break parabolic curve down into two linear slopes:
    if (fsrR <= 600) 
      force = (fsrG - 0.00075) / 0.00000032639;
    else
      force =  fsrG / 0.000000642857;
    Serial.println("Force: " + String(force) + " g");
    Serial.println();

    int led_f = map(force, min_f, max_f, 0, NUMPIXELS);

    
  for (int i = 0; i < led_f; i++) {
    pixels.setPixelColor(i, pixels.Color(225, 225, 10 + (i + 1) * 10));

  }
  for (int i = led_f; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));

  }

  }
  else
  {
    // No pressure detected
  }
  pixels.show();
}

