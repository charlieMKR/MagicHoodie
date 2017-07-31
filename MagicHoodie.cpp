/*
* Hoodie class for managing the LED strip and color sensor
* More info at https://makemyday.io/post/TCS3200-Color-Sensor/
* @makemydayIO
*/

#include "MagicHoodie.h"
#include <Arduino.h>

//Constructor
Hoodie::Hoodie() {
  // just creates a Hoodie and sets the default frequency scaling
  this->freq_scaling = FREQ_20;
}

// Sets the pin number and mode (INPUT/OUTPUT) for the color sensor pins
void Hoodie::setSensorPins(int S0, int S1, int S2, int S3, int OE, int OUT) {
  this->PIN_S0 = S0;
  this->PIN_S1 = S1;
  this->PIN_S2 = S2;
  this->PIN_S3 = S3;
  this->PIN_OE = OE;
  this->PIN_OUT = OUT;

  pinMode(PIN_S0, OUTPUT);
  pinMode(PIN_S1, OUTPUT);
  pinMode(PIN_S2, OUTPUT);
  pinMode(PIN_S3, OUTPUT);
  pinMode(PIN_OE, OUTPUT);
  pinMode(PIN_OUT, INPUT);
  digitalWrite(PIN_OE, HIGH);
}

// Sets the pin number and mode (OUTPUT) for the RGB Led Strip pins
void Hoodie::setRGBPins(int R, int G, int B) {
  this->PIN_LED_R = R;
  this->PIN_LED_G = G;
  this->PIN_LED_B = B;

  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);

}

// Frequencies of Clear, Red, Green and Blue filters. freqs array is the output
void Hoodie::measureFrequencies(unsigned long * freqs) {
  long half_period;
  // TODO: Apply frequency scaling (S0, S1)
  digitalWrite(PIN_S0, freq_scaling >> 1); // Right shift, then use least-significant bit
  digitalWrite(PIN_S1, freq_scaling & 0x01); // Use least-significant bit

  digitalWrite(PIN_OE, LOW); // TODO: Enable Output
  delay(10);
  /** Clear **/
  digitalWrite(PIN_S2, HIGH);
  digitalWrite(PIN_S3, LOW);
  delay(10);
  half_period = pulseIn(PIN_OUT, LOW); //microseconds
  freqs[0] = 1000000/(half_period*2);
  /** RED **/
  digitalWrite(PIN_S2, LOW);
  digitalWrite(PIN_S3, LOW);
  delay(10);
  half_period = pulseIn(PIN_OUT, LOW); //microseconds
  freqs[1] = round(1000000/(half_period*2));
  /** GREEN **/
  digitalWrite(PIN_S2, HIGH);
  digitalWrite(PIN_S3, HIGH);
  delay(10);
  half_period = pulseIn(PIN_OUT, LOW); //microseconds
  freqs[2] = round(1000000/(half_period*2));
  /** BLUE **/
  digitalWrite(PIN_S2, LOW);
  digitalWrite(PIN_S3, HIGH);
  delay(10);
  half_period = pulseIn(PIN_OUT, LOW); //microseconds
  freqs[3] = round(1000000/(half_period*2));
  digitalWrite(PIN_OE, HIGH); // TODO: Disable Output
}

// output: array of 3 int. 8-bit RGB values
/** MODIFY THIS FUNCTION ACCORDING TO YOUR OWN MEASUREMENTS **/
void Hoodie::generateRGB(int rgb[3], unsigned long values[4]) {
  // Begin with red. If >40 means it's the dominant color
  if (values[1] > 40) {
    // Red, Orange, yellow, Fucsia, pink...
    // Check blue
    if (values[3] > 30 ) {
      //Mix of red and blue: Fucsia / Pink
      rgb[0] = 255; // Max value
      rgb[1] = map(values[2], 18, 23, 0, 100); // For pink
      rgb[2] = 100;
    } else {
      // Mix of red and green: Yellow - Orange - Red
      rgb[0] = 255;
      rgb[1] = map(values[2], 17, 34, 0, 255); //min: red, max: yellow
      rgb[2] = 0; // We don't want any blue
    }
  // If not, check Blue
  } else if (values[3] > 40) {
    //check red
    if (values[1] < 30) {
      // No red: Blue colors / Turquoise
      rgb[0] = 0; // No red
      rgb[1] = map(values[2], 23, 30, 0, 100);
      rgb[2] = map(values[3], 40, 50, 0, 100) ;
    } else {
      // No green: Purple colors
      rgb[0] = map(values[1], 16, 39, 0, 100);
      rgb[1] = 0; // No green
      rgb[2] = 255;
    }
  // If not, check green
  } else if (values[2] > 30) {
    //check red
    if (values[1] < 30) {
      //Mix of green and blue
      rgb[0] = 0; // No red
      rgb[1] = 255;
      rgb[2] = map(values[3], 23, 34, 0, 100);
    } else {
      //Mix of green and red
      rgb[0] = map(values[1], 27, 34, 0, 100);
      rgb[1] = 255;
      rgb[2] = 0; // No blue
    }
  } else {
    // White / black
    if ( values[0] > 70000) {
      rgb[0] = 255;
      rgb[1] = 255;
      rgb[2] = 255;
    } else if (values[0] < 15000) {
      rgb[0] = 0;
      rgb[1] = 0;
      rgb[2] = 0;
    }
  }
}

// Sets the color to the RGB LED or LED strip
void Hoodie::setColorRGB(int r, int g, int b) {
  // TODO: set the r, g and b values to the led strip
  analogWrite(PIN_LED_R, r);
  analogWrite(PIN_LED_G, g);
  analogWrite(PIN_LED_B, b);
}

// Reads from the sensor. Returns an array of 3 int. 8-bit RGB values
void Hoodie::getColor(int* rgb) {
  unsigned long result[4];
  getTenSamples(result);
  generateRGB(rgb, result);
}

// Sets the frequency scaling
void Hoodie::setFreqScaling(int scaling){
  if (scaling > 0 && scaling < 4) { // Consider possible values only
    this->freq_scaling = scaling;
  }
}

// Gets ten samples, returns mean values
// First value (0) is clear frequency,
// Values 1, 2 and 3 are r, g and b frequencies expressed as a % of 0.
void Hoodie::getTenSamples(unsigned long* result) {
  unsigned long freqs[4];
  unsigned long samples[4] = {0, 0, 0, 0};
  for (int i=0; i<10; i++) {
    // Measure it
    measureFrequencies(freqs);
    // Add it
    for (int e=0; e<4; e++) {
      if (e==0) {
        samples[e] = samples[e] + freqs[e];
      } else {
        samples[e] = samples[e] + round(float(freqs[e])/float(freqs[0])*100);;
      }

    }
  }
  for (int i=0; i<4;i++) {
    result[i] = samples[i]/10;
  }
}
