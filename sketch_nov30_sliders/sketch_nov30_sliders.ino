/*
 * Slider control
 * For Adafruit 2.8" TFT LCD - capacitive.
 * Any of the four screen rotations may be used.
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>

// Default values for Adafruit shield v2.
#define TFT_DC 9
#define TFT_CS 10

// The FT6206 uses hardware I2C (SCL/SDA)
Adafruit_FT6206 ctp = Adafruit_FT6206();
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define Slider_X 170
#define Slider_Y 40
#define Slider_W 5
#define Slider_H 170
 
#define Thumb_W 50
#define Thumb_H 25
#define Thumb_X Slider_X - Thumb_W/2 + 2
#define initThumb_Y 125

 TS_Point p;
 int x, y, y1;
 int sliderValue = 0;
 int Thumb_Y = 0;
 char sliderValueStr[5];

 // Rotations 0,2 = portrait  : 0->USB=right,upper : 2->USB=left,lower
 // Rotations 1,3 = landscape : 1->USB=left,upper  : 3->USB=right,lower
 byte rotation = 1; //(0->3)

void showSliderValue(int value)
{
 sprintf(sliderValueStr,"%03d",value);
 tft.setCursor(30,30);
 tft.setTextColor(WHITE, BLUE);
 tft.setTextSize(3);
 tft.print(sliderValueStr);
}

void showOrigin() {
 tft.setCursor(0, 0);
 tft.setTextColor(WHITE);
 tft.setTextSize(2);
 tft.print("o");
}

void sliderHandler() {
 sliderValue = (tft.height() - y) - (tft.height()-(Slider_Y+Slider_H-Thumb_H));
 if (sliderValue < 0) sliderValue = 0;
 showSliderValue(sliderValue);
 // erase previous thumb by redrawing with background color
 tft.drawRect(Thumb_X, y1, Thumb_W, Thumb_H, BLUE);
 // then draw new thumb
 tft.drawRect(Thumb_X, y, Thumb_W, Thumb_H, WHITE);
 tft.fillRoundRect(Slider_X, Slider_Y, Slider_W, Slider_H, 2, WHITE);
}

void setup() {
 tft.begin();
 if (! ctp.begin()) {  // pass in 'sensitivity' coefficient
  Serial.println("Couldn't start FT6206 touchscreen controller");
  while (1);
 }
 tft.setRotation(rotation);
 tft.fillScreen(BLUE);
 showOrigin();
 Thumb_Y = initThumb_Y;
 y = Thumb_Y + Thumb_H/2;
 y1 = y;
 sliderHandler();
}

void loop() {
 if (!ctp.touched()) { 
    return;
 }
 
 p = ctp.getPoint();
 
 switch (rotation) {
  case 0:
   x = 240 - p.x;
   y = 320 - p.y;
  break;
  case 1:   // p.x, p.y reversed
   x = 320 - p.y;
   y = p.x;
  break;
  case 2:
   x = p.x;
   y = p.y;
  break;
  case 3:  // p.x, p.y reversed
   x = p.y;
   y = 240 - p.x;
  break;
  }
   
   Thumb_Y = y - Thumb_H/2;
   if(x > Thumb_X && x < (Thumb_X + Thumb_W)) {
     if(y > Thumb_Y && y < (Thumb_Y + Thumb_H)) {
       if(Thumb_Y < Slider_Y){
         y =  Slider_Y;
        }
       else if((Thumb_Y + Thumb_H) > (Slider_Y+Slider_H)){
         y = (Slider_Y+Slider_H)-Thumb_H;
        }
       sliderHandler();
       y1 = y;
     }
   }     
}
