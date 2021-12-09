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
Adafruit_FT6206 ts = Adafruit_FT6206();

#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define FRAME_X 0
#define FRAME_Y 0
#define FRAME_W 320
#define FRAME_H 240

#define START_X 260
#define START_Y 190
#define START_W 60
#define START_H 50

#define STOP_X 100
#define STOP_Y 140
#define STOP_W 120
#define STOP_H 100

#define HOME_X 0
#define HOME_Y 190
#define HOME_W 60
#define HOME_H 50

int touch = 0;
String screen;


void drawFrame()
{
  tft.fillScreen(ILI9341_BLACK); //clears screen
  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
}

void startup()
{
  drawFrame();
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("PIE");
  tft.println("Touch to continue...");
}

void settings()
{
  drawFrame();
  tft.fillRect(START_X, START_Y, START_W, START_H, ILI9341_GREEN);
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); //writes over prev text
  tft.setTextSize(2);
  tft.println("Settings");
}
void runscreen()
{
  drawFrame();
  tft.fillRect(STOP_X, STOP_Y, STOP_W, STOP_H, ILI9341_RED);
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.println("Running");
}

void stopscreen()
{
  drawFrame();
  tft.fillRect(HOME_X, HOME_Y, HOME_W, HOME_H, ILI9341_BLUE);
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.println("Stopped");
}


void setup(void)
{
  Serial.begin(9600);
  tft.begin();
  if (!ts.begin(40)) { 
    Serial.println("Unable to start touchscreen.");
  } 
  else { 
    Serial.println("Touchscreen started."); 
  }

  tft.fillScreen(ILI9341_BLACK);
  // origin = left,top landscape (USB left upper)
  tft.setRotation(1); 
  startup();
  touch = 0;
}

void loop()
{
  // See if there's any  touch data for us
  if (ts.touched())
  {   
    if (touch == 0) {
      settings();
      touch ++;
    }
    // Retrieve a point  
    TS_Point p = ts.getPoint(); 
    // rotate coordinate system
    // flip it around to match the screen.
    p.x = map(p.x, 0, 240, 240, 0);
    p.y = map(p.y, 0, 320, 320, 0);
    int y = tft.height() - p.x;
    int x = p.y;
    if((x > START_X) && (x < (START_X + START_W))) {
        if ((y > START_Y) && (y <= (START_Y + START_H))) {
        runscreen();
      }
    }
    else if((x > STOP_X) && (x < (STOP_X + STOP_W))) {
        if ((y > STOP_Y) && (y <= (STOP_Y + STOP_H))) {
      stopscreen();
      }
    }
    else if((x > HOME_X) && (x < (HOME_X + HOME_W))) {
        if ((y > HOME_Y) && (y <= (HOME_Y + HOME_H))) {
      settings();
      }
    }
  } 
}
