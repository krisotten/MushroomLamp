/*
This code is for an Arduino Uno and incorporates capacitive touch using an I2C sensor to create 8 different light modes for the mushroom lamp.
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
#define PIN_NEO_PIXEL  8   // Arduino pin that connects to NeoPixel
#define NUM_PIXELS     24  // The number of LEDs (pixels) on NeoPixel

int showType = 0;

Adafruit_MPR121 cap = Adafruit_MPR121();
Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

// Keeps track of the last pins touched so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void setup() {
  NeoPixel.begin();
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

// Sets neopixels to given color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<NUM_PIXELS; i++) {
    NeoPixel.setPixelColor(i, c);
    NeoPixel.show();
    delay(wait);
    }
  }

// Each case represents a different light mode and the case number corresponds to the pins on the I2C touch sensor
void startShow(int i) {
  switch(i){
    
    case 1: colorWipe(NeoPixel.Color(255, 0, 0), 50);  // Red
            break;
    case 2: colorWipe(NeoPixel.Color(255, 255, 0), 50);  // Yellow
            break;
    case 3: colorWipe(NeoPixel.Color(0, 255, 0), 50);  // Green
            break;
    case 4: colorWipe(NeoPixel.Color(0, 0, 255), 50);  // Blue
            break;
    case 5: colorWipe(NeoPixel.Color(255, 0, 255), 50);  // Pink
            break;
    case 6: colorWipe(NeoPixel.Color(64, 224, 208), 50);  // Turquoise
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
