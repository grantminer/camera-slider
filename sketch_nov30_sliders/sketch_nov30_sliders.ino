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
/*

#define Slider_X 75
#define Slider_Y 50
#define Slider_W 5
#define Slider_H 155
 
#define Thumb_W 50
#define Thumb_H 25
#define Thumb_X Slider_X - Thumb_W/2 + 2
#define initThumb_Y 125
#define Thumb_RAD 7

#define Slider_X_2 150
#define Slider_Y_2 50
#define Slider_W_2 5
#define Slider_H_2 155
 
#define Thumb_W_2 50
#define Thumb_H_2 25
#define Thumb_X_2 Slider_X_2 - Thumb_W_2/2 + 2
#define initThumb_Y_2 125
#define Thumb_RAD_2 7
*/
// Rotated!

#define Slider_X 115
#define Slider_Y 75
#define Slider_W 155//5
#define Slider_H 5//155
 
#define Thumb_H 50
#define Thumb_W 25
#define initThumb_X 125 //Slider_Y - Thumb_W/2 + 2
#define Thumb_Y 50 
#define Thumb_RAD 7

#define Slider_X_2 115
#define Slider_Y_2 150
#define Slider_W_2 155
#define Slider_H_2 5
 
#define Thumb_W_2 25
#define Thumb_H_2 50
#define initThumb_X_2 125//Slider_Y_2 - Thumb_W_2/2 + 2
#define Thumb_Y_2 125
#define Thumb_RAD_2 7


/*
#define Start_W 200
#define Start_H 40
#define Start_X 60
#define Start_Y 190
#define Start_RAD 10*/

#define Start_W 200//40
#define Start_H 40//200
#define Start_X 60 //190
#define Start_Y 190
#define Start_RAD 10

 TS_Point p;
 int x, y, x1;
 int sliderValue = 0;
 int Thumb_X = 0;
 char sliderValueStr[5];

 int x_2, y_2, x1_2;
 int sliderValue2 = 0;
 int Thumb_X_2 = 0;
 char sliderValueStr2[5];

 // Rotations 0,2 = portrait  : 0->USB=right,upper : 2->USB=left,lower
 // Rotations 1,3 = landscape : 1->USB=left,upper  : 3->USB=right,lower
 byte rotation = 1; //(0->3) //2
 

void showSliderValue(int value)
{
 sprintf(sliderValueStr,"%03d",value);
 //tft.setRotation(1);
 tft.setCursor(280,70);
 tft.setTextColor(WHITE, BLACK);
 tft.setTextSize(2);
 tft.print(sliderValueStr);
 tft.setCursor(10, 70);
 tft.setTextColor(WHITE, BLACK);
 tft.setTextSize(2);
 tft.print("Speed");
 //tft.setRotation(2);
}

void showSliderValue2(int value2)
{
 sprintf(sliderValueStr2,"%03d",value2);
 //tft.setRotation(1);
 tft.setCursor(280,150);
 tft.setTextColor(WHITE, BLACK);
 tft.setTextSize(2);
 tft.print(sliderValueStr2);
 tft.setCursor(10, 150);
 tft.setTextColor(WHITE, BLACK);
 tft.setTextSize(2);
 tft.print("Distance");
 //tft.setRotation(2);
}

void sliderHandler() {
 sliderValue = (tft.width() + x) - 435;//((tft.width() - x) - (tft.width()-(Slider_X+Slider_W-Thumb_W)));
 if (sliderValue < 0) sliderValue = 0;
 showSliderValue(sliderValue);
 // erase previous thumb by redrawing with background color
 tft.drawRoundRect(x1, Thumb_Y, Thumb_W, Thumb_H, Thumb_RAD, BLACK);
 // then draw new thumb
 tft.drawRoundRect(x, Thumb_Y, Thumb_W, Thumb_H, Thumb_RAD, WHITE);
 tft.fillRoundRect(Slider_X, Slider_Y, Slider_W, Slider_H, 2, WHITE);
}

void sliderHandler2() {
 sliderValue2 = (tft.width() + x_2) - 435;//((tft.width() - x) - (tft.width()-(Slider_X+Slider_W-Thumb_W)));
 if (sliderValue2 < 0) sliderValue2 = 0;
 showSliderValue2(sliderValue2);
 // erase previous thumb by redrawing with background color
 tft.drawRoundRect(x1_2, Thumb_Y_2, Thumb_W_2, Thumb_H_2, Thumb_RAD_2, BLACK);
 // then draw new thumb
 tft.drawRoundRect(x_2, Thumb_Y_2, Thumb_W_2, Thumb_H_2, Thumb_RAD_2, WHITE);
 tft.fillRoundRect(Slider_X_2, Slider_Y_2, Slider_W_2, Slider_H_2, 2, WHITE);
}

void startButton() {
  //tft.setRotation(1);
  tft.fillRoundRect(Start_X, Start_Y, Start_W, Start_H, Start_RAD, GREEN);
  tft.setCursor(Start_X+40, Start_Y+5);
  tft.setTextColor(BLACK, GREEN);
  tft.setTextSize(4);
  tft.print("Start");
}

void startButton2() {
  //tft.setRotation(1);
  tft.fillRoundRect(Start_X, Start_Y, Start_W, Start_H, Start_RAD, RED);
  tft.setCursor(Start_X+40, Start_Y+5);
  tft.setTextColor(BLACK, RED);
  tft.setTextSize(4);
  tft.print("Start");
}

void showOrigin() {
 tft.setCursor(0, 0);
 tft.setTextColor(WHITE);
 tft.setTextSize(2);
 tft.print("o");
}

void setup() {
 tft.begin();
 if (! ctp.begin()) {  // pass in 'sensitivity' coefficient
  Serial.println("Couldn't start FT6206 touchscreen controller");
  while (1);
 }
 tft.setRotation(rotation);
 tft.fillScreen(BLACK);

 //tft.setRotation(1);
// showOrigin();

 //tft.setRotation(1);
 tft.setCursor(50, 10);
 tft.setTextColor(WHITE, BLACK);
 tft.setTextSize(3);
 tft.print("Settings");
 //tft.setRotation(2);
 
 Thumb_X = initThumb_X;
 x = Thumb_X + Thumb_W/2;
 x1 = x;
 sliderHandler();

 Thumb_X_2 = initThumb_X_2;
 x_2 = Thumb_X_2 + Thumb_W_2/2;
 x1_2 = x_2;
 sliderHandler2();

 startButton();

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

  if((x > Start_X) && (x < (Start_X + Start_W)) && (y > Start_Y) && (y <= (Start_Y + Start_H))) {
    //if (state == SETTINGS) {
      //state = RUN;
      startButton2();
      //delay(500);
    }
  
   Thumb_X = x - Thumb_W/2;
   if(y > Thumb_Y && y < (Thumb_Y + Thumb_H)) {
     if(x > Thumb_X && x < (Thumb_X + Thumb_W)) {
       if(Thumb_X < Slider_X){
         x =  Slider_X;
        }
       else if((Thumb_X + Thumb_W) > (Slider_X+Slider_W)){
         x = (Slider_X+Slider_W)-Thumb_W;
        }
       sliderHandler();
       x1 = x;
     }
   }
    
   Thumb_X_2 = x_2 - Thumb_W_2/2;
   if(y_2 > Thumb_Y_2 && y_2 < (Thumb_Y_2 + Thumb_H_2)) {
     if(x_2 > Thumb_X_2 && x_2 < (Thumb_X_2 + Thumb_W_2)) {
       if(Thumb_X_2 < Slider_X_2){
         x_2 =  Slider_X_2;
        }
       else if((Thumb_X_2 + Thumb_W_2) > (Slider_X_2+Slider_W_2)){
         x_2 = (Slider_X_2+Slider_W_2)-Thumb_W_2;
        }
       sliderHandler2();
       x1_2 = x_2;
     }
   }   
}
