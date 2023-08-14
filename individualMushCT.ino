/*
This code is an extension of capacitiveTouch.ino but instead of all LEDs being connected to one pin on the Arduino Uno, the LEDs in each 
individual mushroom are connected to their own pin. This allows us to manipulate the code so that each mushroom will be a different color. 
*/

//touch sensor
#include <Wire.h>
#include "Adafruit_MPR121.h"

//lights
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

//touch sensor
#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

//lights
#define PIN_NEO_PIXEL  6 // Mushroom 1 is connected to pin 6 on the Arduino Uno
#define PIN_NEO_PIXEL2 8 // Mushroom 2 is connected to pin 8 on the Arduino Uno
#define PIN_NEO_PIXEL3 10 // Mushroom 3 is connected to pin 10 on the Arduino Uno

#define NUM_PIXELS     12 // 12 LEDs (pixels) in Mushroom 1
#define NUM_PIXELS2    25 // 25 LEDs (pixels) in Mushroom 2
#define NUM_PIXELS3    12 // 12 LEDs (pixels) in Mushroom 3

int showType = 0;

Adafruit_MPR121 cap = Adafruit_MPR121();
Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel NeoPixel2(NUM_PIXELS2, PIN_NEO_PIXEL2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel NeoPixel3(NUM_PIXELS3, PIN_NEO_PIXEL3, NEO_GRB + NEO_KHZ800);

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void setup() {
  NeoPixel.begin();
  NeoPixel2.begin();
  NeoPixel3.begin();
  Serial.begin(9600);

  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }
  
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

}

void loop() {
  // Get the currently touched pads
  currtouched = cap.touched();

  for (uint8_t i=0; i<9; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");
      if (i==0) {
        startShow(random(1, 9));
      } else {
        startShow(i);
      }
    }
  }
  
  lasttouched = currtouched;

}

// Function to set neopixel color for Mushroom 1 
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<NUM_PIXELS; i++) {
    NeoPixel.setPixelColor(i, c);
    NeoPixel.show();
    delay(wait);
    }
  }
// Function to set neopixel color for Mushroom 2
void colorWipe2(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<NUM_PIXELS2; i++) {
    NeoPixel2.setPixelColor(i, c);
    NeoPixel2.show();
    delay(wait);
    }
  }
// Function to set neopixel color for Mushroom 3
void colorWipe3(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<NUM_PIXELS3; i++) {
    NeoPixel3.setPixelColor(i, c);
    NeoPixel3.show();
    delay(wait);
    }
  }

// Each case represents a different light mode and the case number corresponds to the pins on the I2C touch sensor 
void startShow(int i) {
  switch(i){
    
    case 1: colorWipe(NeoPixel.Color(255, 0, 0), 50);  // Red
            colorWipe2(NeoPixel2.Color(0, 255, 0), 50);
            colorWipe3(NeoPixel3.Color(0, 0, 255), 50);
            break;
    case 2: colorWipe(NeoPixel.Color(255, 255, 0), 50);  // Yellow
            colorWipe2(NeoPixel2.Color(255, 0, 255), 50);
            colorWipe3(NeoPixel3.Color(64, 224, 208), 50);
            break;
    case 3: colorWipe(NeoPixel.Color(0, 255, 255), 50);  // Green
            colorWipe2(NeoPixel2.Color(128, 0, 128), 50);
            colorWipe3(NeoPixel3.Color(255, 192, 203), 50);
            break;
    case 4: colorWipe(NeoPixel.Color(128, 0, 128), 50);  // Blue
            colorWipe2(NeoPixel2.Color(255, 0, 255), 50);
            colorWipe3(NeoPixel3.Color(64, 224, 208), 50);
            break;
    case 5: colorWipe(NeoPixel.Color(255, 255, 0), 50);  // Pink
            colorWipe2(NeoPixel2.Color(255, 192, 203), 50);
            colorWipe3(NeoPixel3.Color(0, 0, 255), 50);
            break;
    case 6: colorWipe(NeoPixel.Color(0, 255, 0), 50);  // Turquoise
            colorWipe2(NeoPixel2.Color(128, 0, 128), 50);
            colorWipe3(NeoPixel3.Color(255, 192, 203), 50);
            break;
    case 7: rainbow(20);  // Rainbow
            break;
    case 8: colorWipe(NeoPixel.Color(0, 0, 0), 50);    // Black/off
            break;
    
  }
  }

  //Pulse through colors of rainbow
  void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<NUM_PIXELS; i++) {
      NeoPixel.setPixelColor(i, Wheel((i+j) & 255));
    }
    NeoPixel.show();
    for(i=0; i<NUM_PIXELS2; i++) {
      NeoPixel2.setPixelColor(i, Wheel((i+j) & 255));
    }
    NeoPixel2.show();
    for(i=0; i<NUM_PIXELS3; i++) {
      NeoPixel3.setPixelColor(i, Wheel((i+j) & 255));
    }
    NeoPixel3.show();
    delay(wait);
  }
}

  uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return NeoPixel.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return NeoPixel.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return NeoPixel.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
