//This example implements a simple sliding On/Off button. The example
// demonstrates drawing and touch operations.
//
//Thanks to Adafruit forums member Asteroid for the original sketch!
//
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>

// The FT6206 uses hardware I2C (SCL/SDA)
Adafruit_FT6206 ctp = Adafruit_FT6206();

#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define FRAME_X 0
#define FRAME_Y 0
#define FRAME_W 320
#define FRAME_H 240

#define BUTTON_X 100
#define BUTTON_Y 200
#define BUTTON_W 120
#define BUTTON_H 60

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

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

int touch = 0;
String screen;
int x, y, y1;
TS_Point p;
byte scrn_rotation = 1; //horizontal align on screen

//define states
enum states {
  INTRO,
  SETTINGS,
  RUN,
  STOP,
};
states state;            // Global variable to store current state

#define Start_W 40
#define Start_H 200
#define Start_X 190
#define Start_Y 60
#define Start_RAD 10

 int sliderValue = 0;
 int Thumb_Y = 0;
 char sliderValueStr[5];

 int x_2, y_2, y1_2;
 int sliderValue2 = 0;
 int Thumb_Y_2 = 0;
 char sliderValueStr2[5];

void showSliderValue(int value){
 sprintf(sliderValueStr,"%03d",value);
 tft.setCursor(280,70);
 tft.setTextColor(WHITE, BLACK);
 tft.setTextSize(2);
 tft.print(sliderValueStr);
 tft.setCursor(10, 70);
 tft.setTextColor(WHITE, BLACK);
 tft.setTextSize(2);
 tft.print("Speed");
}

void showSliderValue2(int value2){
 sprintf(sliderValueStr2,"%03d",value2);
 tft.setCursor(280,150-6);
 tft.setTextColor(WHITE, BLACK);
 tft.setTextSize(2);
 tft.print(sliderValueStr2);
 tft.setCursor(10, 150-6);
 tft.setTextColor(WHITE, BLACK);
 tft.setTextSize(2);
 tft.print("Distance");
}

void sliderHandler() {
 sliderValue = ((tft.height() - y) - (tft.height()-(Slider_Y+Slider_H-Thumb_H)));
 if (sliderValue < 0) sliderValue = 0;
 showSliderValue(sliderValue);
 // erase previous thumb by redrawing with background color
 tft.drawRoundRect(Thumb_X, y1, Thumb_W, Thumb_H, Thumb_RAD, BLACK);
 // then draw new thumb
 tft.drawRoundRect(Thumb_X, y, Thumb_W, Thumb_H, Thumb_RAD, WHITE);
 tft.fillRoundRect(Slider_X, Slider_Y, Slider_W, Slider_H, 2, WHITE);
}

void sliderHandler2() {
sliderValue2 = ((tft.height() - y_2) - (tft.height()-(Slider_Y_2+Slider_H_2-Thumb_H_2)));
 if (sliderValue2 < 0) sliderValue2 = 0;
 showSliderValue2(sliderValue2);
 // erase previous thumb by redrawing with background color
 tft.drawRoundRect(Thumb_X_2, y1_2, Thumb_W_2, Thumb_H_2, Thumb_RAD_2, BLACK);
// tft.drawCircle(Thumb_X_2, Thumb_W_2, y1_2, BLACK);
 // then draw new thumb
 tft.drawRoundRect(Thumb_X_2, y_2, Thumb_W_2, Thumb_H_2, Thumb_RAD_2, WHITE);
// tft.drawCircle(Thumb_X_2, Thumb_W_2, y1_2, WHITE);
 tft.fillRoundRect(Slider_X_2, Slider_Y_2, Slider_W_2, Slider_H_2, 2, WHITE);
}

void drawFrame(){
  tft.fillScreen(ILI9341_BLACK); //clears screen
  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
}

void startup(){
  drawFrame();
  tft.setCursor(97, 10);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("PIE Project");
  tft.setCursor(105, 25);
  tft.println("GROUP 30");
  tft.setCursor(87, 50);
  tft.println("Camera Slider");
  tft.setCursor(50, 65);
  tft.println("Touch to continue...");
}

void settings(){
  drawFrame();
  tft.fillRoundRect(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, 10, ILI9341_GREEN);
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); //writes over prev text
  tft.setTextSize(2);
  tft.println("Settings");
  tft.println("Speed");
  tft.println("Distance");
}

void runscreen(){
  drawFrame();
  tft.fillRoundRect(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, 10, ILI9341_RED);
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.println("Running");
}

void stopscreen(){
  drawFrame();
  tft.fillRoundRect(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, 10, ILI9341_BLUE);
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.println("Stopped");
}

void setup(){
  Serial.begin(9600);
  tft.begin();
  if (!ctp.begin(40)) { 
    Serial.println("Unable to start touchscreen.");
  } 
  else {
    Serial.println("Touchscreen started."); 
  }
  Thumb_Y = initThumb_Y;
  y = Thumb_Y + Thumb_H/2;
  y1 = y;
  Thumb_Y_2 = initThumb_Y_2;
  y_2 = Thumb_Y_2 + Thumb_H_2/2;
  y1_2 = y_2;
  sliderHandler();
  sliderHandler2();
  tft.fillScreen(ILI9341_BLACK);
  // origin = left,top landscape (USB left upper)
  tft.setRotation(scrn_rotation); 
  startup();
}

void loop(){
  if (! ctp.touched()) { 
    return;
  }
 // if (ctp.touched()) {
  p = ctp.getPoint();
  x = 320 - p.y;
  y = p.x;
 switch (state) {
    case INTRO:
      startup();
      break;
    case SETTINGS:
      settings();
      break;
    case RUN:
      runscreen();
      break;
    case STOP:
      stopscreen();
      break;
 }
   if (state == INTRO) {
    state = SETTINGS;    
  }
  if (state == SETTINGS){
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
   Thumb_Y_2 = y_2 - Thumb_H_2/2;
   if(x_2 > Thumb_X_2 && x_2 < (Thumb_X_2 + Thumb_W_2)) {
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
  if((x > BUTTON_X) && (x < (BUTTON_X + BUTTON_W)) && (y > BUTTON_Y) && (y <= (BUTTON_Y + BUTTON_H))) {
    if (state == SETTINGS) {
      state = RUN;
    }
    else if (state == RUN) {
      state = STOP;
    }
    else if (state == STOP) {
      state = SETTINGS;
    }
  }
// }
}  
