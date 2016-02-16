#include <Adafruit_NeoPixel.h>

#define PIN 12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(9, PIN, NEO_GRB + NEO_KHZ800);


void setRGB(int r, int g, int b)
{
    for(int i=0; i< strip.numPixels(); i++) 
    {
        strip.setPixelColor(i, r, g, b);
    }
    strip.show();
}

void setup() 
{
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void loop() 
{
    setRGB(255, 0, 0);
    delay(2000);
    setRGB(0, 255, 0);
    delay(2000);
    setRGB(0, 0, 255);
    delay(2000);
}