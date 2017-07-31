#include <Arduino.h>
#include "MagicHoodie.h"

// Define Pins
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
int color[3];

void setup() {
  // Setup your hoodie
   myHoodie.setRGBPins(R_PIN, G_PIN, B_PIN);
   myHoodie.setSensorPins(S0_PIN, S1_PIN, S2_PIN, S3_PIN, OE_PIN, OUT_PIN);
}

void loop() {
  // Capture color when button is pushed
  state = digitalRead(BUTTON_PIN);
  if (state == HIGH) {
    myHoodie.setColorRGB(color[0], color[1], color[2]);
    delay(200);
  }


}
