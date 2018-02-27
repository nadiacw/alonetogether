#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <SoftwareSerial.h>

#define NUMPIXELS      1

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS, 7, NEO_GRB + NEO_KHZ800);
int delayval = 500; // delay for half a second



const int FSR_PIN = A0; // Pin connected to FSR/resistor divider

// Measure the voltage at 5V and resistance of your 3.3k resistor, and enter
// their value's below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 3230.0; // Measured resistance of 3.3k resistor

float max_f = 150;
float min_f = 0.47;

SoftwareSerial BT(10, 11); // Bluetooth 10 RX, 11 TX.
int bytesToSend[2]; //Array that hold the values you will be sending to OF


void setup()
{
  BT.begin(9600);
  Serial.begin(9600);
  pinMode(FSR_PIN, INPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pixels.begin(); // This initializes the NeoPixel library.
  pixels2.begin();
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
    int test = map(force, 0, 7000, 0, 255);
    Serial.println(test);

    if(test > 5) {
      for (int i = 0; i < NUMPIXELS; i++) {
  
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // Moderately bright green color.
        pixels2.setPixelColor(i, pixels.Color(0, 150, 0)); // Moderately bright green color.
  
        pixels.show(); // This sends the updated pixel color to the hardware.
        pixels2.show();
  
        delay(delayval); // Delay for a period of time (in milliseconds).
  
      }
    }
  }

  /**************** SEND Bluetooth messages ***************/
  if (BT.available()) {
    float force;
    int fsrADC = analogRead(FSR_PIN);

    float fsrV = fsrADC * VCC / 1023.0;

    float fsrR = R_DIV * (VCC / fsrV - 1.0);

    float fsrG = 1.0 / fsrR; // Calculate conductance
    force =  fsrG / 0.000000642857;

    //    bytesToSend[0] = map(angle, 0, 90, 0, 255);
    bytesToSend[0] = map(force, 0, 5000, 0, 255);

    bytesToSend[1] = 0;

    for (int i = 0; i < 2 ; i++) { // loops through the Bytes Array
      BT.write(bytesToSend[i]); // When using Serial Write you are sending the Info as a Byte which is needed to send info to OF
      // This means this info won't appear in your arduino serial viewer
    }

  }
  /**************** READ Bluetooth messages ***************/
  if (BT.available()) {
    Serial.println("hello!");

    // get value from OF
    char value = BT.read();
    Serial.println(value);

    if (value == '0') {
      digitalWrite(LED_BUILTIN, HIGH);
      //setColor(255, 0, 0); // Red Color;
    }
    if (value == '1') {
      digitalWrite(LED_BUILTIN, LOW);
      //setColor(0, 0, 0);
    }

  }

}


