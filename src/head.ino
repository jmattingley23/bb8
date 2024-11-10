#include "FastLED.h"

CRGB psi_leds[4];
CRGB holo_leds[8];
CRGB logic_leds[56];

void setup() {
  FastLED.addLeds<WS2812B, 16, GRB>(logic_leds, 56).setCorrection(TypicalSMD5050).setTemperature(Tungsten40W);
 
  FastLED.addLeds<SK6812, 17, GRB>(psi_leds, 4).setRgbw(RgbwDefault()).setCorrection(TypicalSMD5050).setTemperature(Halogen);
  
  FastLED.addLeds<SK6812, 18, GRB>(holo_leds, 8).setRgbw(Rgbw(
      kRGBWDefaultColorTemp,
      kRGBWMaxBrightness,
      W3
  ));
  
  FastLED.setBrightness(255);
  FastLED.show();
}
void loop(){
  // PSI
  for(int i = 0; i < 4; i++){
    psi_leds[i] = CRGB::White;
  }

  // holo
  for(int i = 0; i < 7; i++){
    holo_leds[i] = CRGB::White;
  }

  // front logic top
  for(int i = 0; i < 12; i++){
    logic_leds[i] = CRGB::White;
  }
//  // front logic bottom
//  for(int i = 22; i < 34; i++){
//    logic_leds[i] = CRGB::White;
//  }

  // rear logic
  for(int i = 46; i < 56; i++){
    logic_leds[i] = CRGB::Blue;
  }
  FastLED.show();
}
