#include <FastLED.h>
CRGB monPixel;

#include <Arduino.h>
#define MA_BROCHE_ANGLE 32



void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812, 27, GRB>(&monPixel, 1);
  monPixel = CRGB(255, 150, 0);
  FastLED.show();

}

void loop() {
  int maLectureAnalogique;
  maLectureAnalogique = analogRead(MA_BROCHE_ANGLE);

  if (maLectureAnalogique < 2000)
  {
    monPixel = CRGB (0,0,255);
    FastLED.show();
  }
  
else if (maLectureAnalogique > 2000)
{
  monPixel = CRGB(255,150,0);
  FastLED.show();
}
}

