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
#define BUTTON_Y 140
#define BUTTON_W 120
#define BUTTON_H 100

int touch = 0;
String screen;
int x, y;
TS_Point p;
byte rotation = 1; //horizontal align on screen

//define states
enum states {
  INTRO,
  SETTINGS,
  RUN,
  STOP,
};
states state;            // Global variable to store current state


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
  tft.fillRect(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, ILI9341_GREEN);
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); //writes over prev text
  tft.setTextSize(2);
  tft.println("Settings");
  tft.println("Speed");
  tft.println("Distance");
}

void runscreen(){
  drawFrame();
  tft.fillRect(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, ILI9341_RED);
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.println("Running");
}

void stopscreen(){
  drawFrame();
  tft.fillRect(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, ILI9341_BLUE);
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
  tft.fillScreen(ILI9341_BLACK);
  // origin = left,top landscape (USB left upper)
  tft.setRotation(rotation); 
  state  = INTRO;
}

void loop(){
  if (! ctp.touched()) { 
    return;
  }
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
//    if (state == SETTINGS) {
//    state = RUN;
//    //delay(500);
//  }
   if (state == INTRO) {
    state = SETTINGS;
    //delay(500);
  }
  if((x > BUTTON_X) && (x < (BUTTON_X + BUTTON_W)) && (y > BUTTON_Y) && (y <= (BUTTON_Y + BUTTON_H))) {
    if (state == SETTINGS) {
      state = RUN;
      //delay(500);
    }
    else if (state == RUN) {
      state = STOP;
      //delay(500);
    }
    else if (state == STOP) {
      state = SETTINGS;
      //delay(500);
    }
  }
}  
