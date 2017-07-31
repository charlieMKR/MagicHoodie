/**
* Takes ten samples with the color sensor and outputs the average.
* The otput values are:
* 1. Measurement of red filter as % of clear frequency measurement
* 2. Measurement of green filter as % of clear frequency measurement
* 3. Measurement of blue filter as % of clear frequency measurement
* More info: https://makemyday.io/post/TCS3200-Color-Sensor/
* @makemydayIO
**/

#include <Arduino.h>
#include "MagicHoodie.h"

// Define pins
const int BUTTON_PIN = 13;
const int R_PIN = 6;
const int G_PIN = 9;
const int B_PIN = 5;
const int S0_PIN = 10;
const int S1_PIN = 11;
const int S2_PIN = 3;
const int S3_PIN = 2;
const int OUT_PIN = 4;
const int OE_PIN = 12;

Hoodie myHoodie; // create Hoodie variable

int state;

void setup() {
  // Setup your hoodie
   myHoodie.setRGBPins(R_PIN, G_PIN, B_PIN);
   myHoodie.setSensorPins(S0_PIN, S1_PIN, S2_PIN, S3_PIN, OE_PIN, OUT_PIN);

   Serial.begin(9600);
}

void loop() {
  // Capture color when button is pushed, print measurements
  state = digitalRead(BUTTON_PIN);
  if (state == HIGH) {
    unsigned long color[4];
    myHoodie.getTenSamples(color);
    Serial.println("---------------------");
    Serial.print("Clear frequency: ");
    Serial.println(color[0]);
    Serial.print("Rojo: ");
    Serial.println(color[1]);
    Serial.print("Verde: ");
    Serial.println(color[2]);
    Serial.print("Azul: ");
    Serial.println(color[3]);
    delay(200);
  }


}
