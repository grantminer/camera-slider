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

#define Slider_X 100
#define Slider_Y 50
#define Slider_W 5
#define Slider_H 155
 
#define Thumb_W 50
#define Thumb_H 25
#define Thumb_X Slider_X - Thumb_W/2 + 2
#define initThumb_Y 125

#define Slider_X_2 175
#define Slider_Y_2 50
#define Slider_W_2 5
#define Slider_H_2 155
 
#define Thumb_W_2 50
#define Thumb_H_2 25
#define Thumb_X_2 Slider_X_2 - Thumb_W_2/2 + 2
#define initThumb_Y_2 125

 TS_Point p;
 int x, y, y1;
 int sliderValue = 0;
 int Thumb_Y = 0;
 char sliderValueStr[5];

 int x_2, y_2, y1_2;
 int sliderValue2 = 0;
 int Thumb_Y_2 = 0;
 char sliderValueStr2[5];

 // Rotations 0,2 = portrait  : 0->USB=right,upper : 2->USB=left,lower
 // Rotations 1,3 = landscape : 1->USB=left,upper  : 3->USB=right,lower
 byte rotation = 2; //(0->3)

void showSliderValue(int value)
{
 sprintf(sliderValueStr,"%03d",value);
 tft.setRotation(1);
 tft.setCursor(30,92);
 tft.setTextColor(WHITE, BLUE);
 tft.setTextSize(3);
 tft.print(sliderValueStr);
 tft.setRotation(2);
}

void showSliderValue2(int value2)
{
 sprintf(sliderValueStr2,"%03d",value2);
 tft.setRotation(1);
 tft.setCursor(30,175-8);
 tft.setTextColor(RED, BLUE);
 tft.setTextSize(3);
 tft.print(sliderValueStr2);
 tft.setRotation(2);
}

void sliderHandler() {
 sliderValue = ((tft.height() - y) - (tft.height()-(Slider_Y+Slider_H-Thumb_H)));
 if (sliderValue < 0) sliderValue = 0;
 showSliderValue(sliderValue);
 // erase previous thumb by redrawing with background color
 tft.drawRect(Thumb_X, y1, Thumb_W, Thumb_H, BLUE);
 // then draw new thumb
 tft.drawRect(Thumb_X, y, Thumb_W, Thumb_H, WHITE);
 tft.fillRoundRect(Slider_X, Slider_Y, Slider_W, Slider_H, 2, WHITE);
}

void sliderHandler2() {
sliderValue2 = ((tft.height() - y_2) - (tft.height()-(Slider_Y_2+Slider_H_2-Thumb_H_2)));
 if (sliderValue2 < 0) sliderValue2 = 0;
 showSliderValue2(sliderValue2);
 // erase previous thumb by redrawing with background color
 tft.drawRect(Thumb_X_2, y1_2, Thumb_W_2, Thumb_H_2, BLUE);
 // then draw new thumb
 tft.drawRect(Thumb_X_2, y_2, Thumb_W_2, Thumb_H_2, RED);
 tft.fillRoundRect(Slider_X_2, Slider_Y_2, Slider_W_2, Slider_H_2, 2, WHITE);
}

void setup() {
 tft.begin();
 if (! ctp.begin()) {  // pass in 'sensitivity' coefficient
  Serial.println("Couldn't start FT6206 touchscreen controller");
  while (1);
 }
 tft.setRotation(rotation);
 tft.fillScreen(BLUE);
 
 Thumb_Y = initThumb_Y;
 y = Thumb_Y + Thumb_H/2;
 y1 = y;
 sliderHandler();
 
 Thumb_Y_2 = initThumb_Y_2;
 y_2 = Thumb_Y_2 + Thumb_H_2/2;
 y1_2 = y_2;
 sliderHandler2();

 Serial.begin(9600);

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
   x_2 = 240 - p.x;
   y_2 = 320 - p.y;
  break;
  case 1:   // p.x, p.y reversed
   x = 320 - p.y;
   y = p.x;
   x_2 = 320 - p.y;
   y_2 = p.x;
  break;
  case 2:
   x = p.x;
   y = p.y;
   x_2 = p.x;
   y_2 = p.y;
  break;
  case 3:  // p.x, p.y reversed
   x = p.y;
   y = 240 - p.x;
   x_2 = p.y;
   y_2 = 240 - p.x;
  break;
  }
   
   Thumb_Y = y - Thumb_H/2;
   if(x > Thumb_X && x < (Thumb_X + Thumb_W)) {
    Serial.print('k');
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

   Thumb_Y_2 = y_2 - Thumb_H_2/2;
   if(x_2 > Thumb_X_2 && x_2 < (Thumb_X_2 + Thumb_W_2)) {
    Serial.print('l');
     if(y_2 > Thumb_Y_2 && y_2 < (Thumb_Y_2 + Thumb_H_2)) {
       if(Thumb_Y_2 < Slider_Y_2){
         y_2 =  Slider_Y_2;
        }
       else if((Thumb_Y_2 + Thumb_H_2) > (Slider_Y_2+Slider_H_2)){
         y_2 = (Slider_Y_2+Slider_H_2)-Thumb_H_2;
        }
       sliderHandler2();
       y1_2 = y_2;
     }
   }     
}
