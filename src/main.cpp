#include <FastLED.h>
CRGB monPixel;

#include <Arduino.h>
#define MA_BROCHE_ANGLE 32

#define MA_BROCHE_BOUTON 39

#include <MicroOscSlip.h>
// Le nombre 128 entre les < > ci-dessous est le nombre maximal d'octets réservés pour les messages entrants.
// Les messages sortants sont écrits directement sur la sortie et ne nécessitent pas de réservation d'octets supplémentaires.
MicroOscSlip<128> monOsc(&Serial);

void setup()
{
  Serial.begin(115200);
  FastLED.addLeds<WS2812, 27, GRB>(&monPixel, 1);
  monPixel = CRGB(255, 150, 0);
  FastLED.show();
}

void loop()
{
  int maLectureAnalogique;
  maLectureAnalogique = analogRead(MA_BROCHE_ANGLE);
  monOsc.sendInt("/pot", maLectureAnalogique);

  int maLectureBouton;
  maLectureBouton = digitalRead( MA_BROCHE_BOUTON );
 
  monOsc.sendInt( "/but" , maLectureBouton );
 
  delay(200);

  /*if (maLectureAnalogique < 2000)
  {
    monPixel = CRGB (0,0,255);
    FastLED.show();
  }

else if (maLectureAnalogique > 2000)
{
  monPixel = CRGB(255,150,0);
  FastLED.show();
};*/
}
