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

#define FRAME_X 0
#define FRAME_Y 0
#define FRAME_W 320
#define FRAME_H 240

#define BUTTON_X 60
#define BUTTON_Y 75
#define BUTTON_W 200
#define BUTTON_H 100


// Common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

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

#define Start_W 200//40
#define Start_H 40//200
#define Start_X 60 //190
#define Start_Y 190
#define Start_RAD 10


int touch = 0;
String screen;
byte scrn_rotation = 1; //horizontal align on screen
 TS_Point p;
 int x, y, x1;
 int sliderValue = 0;
 int Thumb_X = 0;
 char sliderValueStr[5];

 int x_2, y_2, x1_2;
 int sliderValue2 = 0;
 int Thumb_X_2 = 0;
 char sliderValueStr2[5];

 byte rotation = 1; //(0->3) //2


int wscreen = 1;

void runscreen(){
  tft.fillScreen(ILI9341_BLACK); //clears screen
  wscreen = 2;
  //tft.fillRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
  tft.fillRoundRect(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, 10, ILI9341_RED);
  tft.setCursor(100,105);
  tft.setTextColor(ILI9341_WHITE, ILI9341_RED);
  tft.setTextSize(5);
  tft.println("STOP");
  
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.println("Running");
}

void stopscreen(){
  tft.fillScreen(ILI9341_BLACK); //clears screen
  wscreen = 3;
  //tft.fillRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
  tft.fillRoundRect(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, 10, ILI9341_BLUE);
  tft.setCursor(100,105);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLUE);
  tft.setTextSize(5);
  tft.println("Home");
  
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.println("Stopped");
}

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
 sliderValue = (tft.width() + x) - 435;
 if (sliderValue < 0) sliderValue = 0;
 showSliderValue(sliderValue);
 // erase previous thumb by redrawing with background color
 tft.drawRoundRect(x1, Thumb_Y, Thumb_W, Thumb_H, Thumb_RAD, BLACK);
 // then draw new thumb
 tft.drawRoundRect(x, Thumb_Y, Thumb_W, Thumb_H, Thumb_RAD, WHITE);
 tft.fillRoundRect(Slider_X, Slider_Y, Slider_W, Slider_H, 2, WHITE);
}

void sliderHandler2() {
 sliderValue2 = (tft.width() + x_2) - 435;
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
  tft.setTextColor(WHITE, GREEN);
  tft.setTextSize(4);
  tft.print("Start");
}

void settings() {
  tft.fillScreen(BLACK);
  wscreen = 1;
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

}

void setup() {
 tft.begin();
 if (! ctp.begin()) {  // pass in 'sensitivity' coefficient
  Serial.println("Couldn't start FT6206 touchscreen controller");
  while (1);
 }
 tft.setRotation(rotation);
settings();

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

  if((x > Start_X) && (x < (Start_X + Start_W)) && (y > Start_Y) && (y <= (Start_Y + Start_H)) && (wscreen == 1)) {
      runscreen();
    }
 if((x > BUTTON_X) && (x < (BUTTON_X + BUTTON_W)) && (y > BUTTON_Y) && (y <= (BUTTON_Y + BUTTON_H))) {
  if (wscreen == 2) {
    stopscreen();
  }
  else if (wscreen == 3) {
    settings();
  }
}  

if(wscreen == 1) {
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
}
