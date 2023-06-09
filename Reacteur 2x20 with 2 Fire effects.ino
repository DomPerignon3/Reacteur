//Reacteur 2x20 with 2 Fire effects
//Adapted from neopixel-fastled-fire.ino  https://gist.github.com/rahulbot/1e80f24154fe3a03eca254f98b4596d8

#include <FastLED.h>
#define PIN_NEO_PIXEL0 6   // Arduino pin that connects to strip 0
#define PIN_NEO_PIXEL1 5   // Arduino pin that connects to strip 1
#define NUM_LEDS  20

CRGB leds0[NUM_LEDS];
CRGB leds1[NUM_LEDS];

void setup() { 
  delay(3000); // sanity delay
  FastLED.addLeds<NEOPIXEL, PIN_NEO_PIXEL0>(leds0, NUM_LEDS); 
  FastLED.addLeds<NEOPIXEL, PIN_NEO_PIXEL1>(leds1, NUM_LEDS); 
  }

void loop() {
  Fire(55,120,15);
  }

void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat0[NUM_LEDS];
  static byte heat1[NUM_LEDS];
  int cooldown0;
  int cooldown1;
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    cooldown0 = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
    cooldown1 = random(0, ((Cooling * 10) / NUM_LEDS) + 2);

    if(cooldown0>heat0[i]) {
      heat0[i]=0;
    } else {
      heat0[i]=heat0[i]-cooldown0;
    }
     if(cooldown1>heat1[i]) {
      heat1[i]=0;
    } else {
      heat1[i]=heat1[i]-cooldown1;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    heat0[k] = (heat0[k - 1] + heat0[k - 2] + heat0[k - 2]) / 3;
    heat1[k] = (heat1[k - 1] + heat1[k - 2] + heat1[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat0[y] =  heat0[y] + random(160,255);
    heat1[y] =  heat1[y] + random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor0(j, heat0[j] );
    setPixelHeatColor1(j, heat1[j] );
  }

  FastLED.show();
  delay(SpeedDelay);
}


void setPixel0(int Pixel0, byte red, byte green, byte blue) {
  // FastLED
  leds0[Pixel0].r = red;
  leds0[Pixel0].g = green;
  leds0[Pixel0].b = blue;
}

void setPixel1(int Pixel1, byte red, byte green, byte blue) {
  // FastLED
  leds1[Pixel1].r = red;
  leds1[Pixel1].g = green;
  leds1[Pixel1].b = blue;
}

void setPixelHeatColor0 (int Pixel0, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel0(Pixel0, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel0(Pixel0, 255, heatramp, 0);
  } else {                               // coolest
    setPixel0(Pixel0, heatramp, 0, 0);
  }

}

void setPixelHeatColor1 (int Pixel1, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel1(Pixel1, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel1(Pixel1, 255, heatramp, 0);
  } else {                               // coolest
    setPixel1(Pixel1, heatramp, 0, 0);
  }

}
