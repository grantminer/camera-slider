/*
 * Slider control
 * For Adafruit 2.8" TFT LCD - capacitive.
 * Any of the four screen rotations may be used.
*/

#include <SPI.h>
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
#define initThumb_X 102.5 //Slider_Y - Thumb_W/2 + 2
#define Thumb_Y 50 
#define Thumb_RAD 7

#define Slider_X_2 115
#define Slider_Y_2 150
#define Slider_W_2 155
#define Slider_H_2 5

#define Slider_X_3 115
#define Slider_Y_3 75
#define Slider_W_3 155//5
#define Slider_H_3 5//155

#define Slider_X_4 115
#define Slider_Y_4 150
#define Slider_W_4 155
#define Slider_H_4 5

 
#define Thumb_W_2 25
#define Thumb_H_2 50
#define initThumb_X_2 102.5//Slider_Y_2 - Thumb_W_2/2 + 2
#define Thumb_Y_2 125
#define Thumb_RAD_2 7

#define Thumb_H_3 50
#define Thumb_W_3 25
#define initThumb_X_3 102.5 //Slider_Y - Thumb_W/2 + 2
#define Thumb_Y_3 50 
#define Thumb_RAD_3 7

#define Thumb_W_4 25
#define Thumb_H_4 50
#define initThumb_X_4 102.5//Slider_Y_2 - Thumb_W_2/2 + 2
#define Thumb_Y_4 125
#define Thumb_RAD_4 7

#define Start_W 200//40
#define Start_H 40//200
#define Start_X 60 //190
#define Start_Y 190
#define Start_RAD 10

#define Back_W 60
#define Back_H 30
#define Back_X 250
#define Back_Y 10
#define Back_RAD 5

int sliderValue_store1 = 0;
int thumb_pos = 0;

int sliderValue_store2 = 0;
int thumb_pos_2 = 0;

int sliderValue_store3 = 0;
int thumb_pos_3 = 0;

int sliderValue_store4 = 0;
int thumb_pos_4 = 0;


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

int x_3, y_3, x1_3;
int sliderValue3 = 0;
int Thumb_X_3 = 0; 
char sliderValueStr3[5];

int x_4, y_4, x1_4;
int sliderValue4 = 0;
int Thumb_X_4 = 0;
char sliderValueStr4[5];
 
byte rotation = 1; //(0->3) //2


int wscreen = 0;

#include "SpeedyStepper.h"

SpeedyStepper translateStepper;
SpeedyStepper panStepper;

#define dirPin1 2
#define stepPin1 3
#define dirPin2 4
#define stepPin2 5
#define LIMIT_SWITCH_PIN_TRANSLATE 6
#define LIMIT_SWITCH_PIN_PAN 7

int maxTranslatePosition = 1150;
int maxPanPosition = 180;
int maxTranslateSpeed = 200;
int maxPanSpeed = 360;
bool homed = false;

float roundedPanSpeed;
float roundedPanPosition;
float speedCounterTranslate;
float positionCounterTranslate;
float speedCounterPan;
float positionCounterPan;

void startup(){
  tft.fillScreen(ILI9341_BLACK); //clears screen
  wscreen = 0;
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


void runscreen(){
  tft.fillScreen(ILI9341_BLACK); //clears screen
  wscreen = 3;
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
  wscreen = 4;
  //tft.fillRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
  tft.fillRoundRect(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, 10, ILI9341_GREEN);
  tft.setCursor(100,105);
  tft.setTextColor(ILI9341_WHITE, ILI9341_GREEN);
  tft.setTextSize(5);
  tft.println("Home");
  
  tft.setCursor(0,0);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.println("Stopped");
}

void showSliderValue(int value1) {
  sprintf(sliderValueStr,"%03d",value1);
  //tft.setRotation(1);
  tft.setCursor(280,70);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print(sliderValueStr);
  tft.setCursor(10, 70);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print("Speed");
  tft.setCursor(10, 90);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print("(cm/s)");
  //tft.setRotation(2);
  sliderValue_store1 = value1;
}

void showSliderValue2(int value2) {
  sprintf(sliderValueStr2,"%03d",value2);
  //tft.setRotation(1);
  tft.setCursor(280,145);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print(sliderValueStr2);
  tft.setCursor(10, 145);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print("Distance");
  tft.setCursor(10, 165);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print("(cm)");
  //tft.setRotation(2);
  sliderValue_store2 = value2;
}

void showSliderValue3(int value3) {
  sprintf(sliderValueStr3,"%03d",value3);
  //tft.setRotation(1);
  tft.setCursor(280,70);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print(sliderValueStr3);
  tft.setCursor(10,70);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print("Speed");
  tft.setCursor(10, 90);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print("(deg/s)");
  //tft.setRotation(2);
  sliderValue_store3 = value3;
}

void showSliderValue4(int value4) {
  sprintf(sliderValueStr4,"%03d",value4);
  //tft.setRotation(1);
  tft.setCursor(280,145);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print(sliderValueStr4);
  tft.setCursor(10, 145);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print("Rotation");
  tft.setCursor(10, 165);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.print("(deg)");
  //tft.setRotation(2);
  sliderValue_store4 = value4;
}

void sliderHandler() {
  sliderValue = sliderValue_store1;
  thumb_pos = (0.26*sliderValue_store1) + 115;
  tft.drawRoundRect(thumb_pos, Thumb_Y, Thumb_W, Thumb_H, Thumb_RAD, WHITE);
  tft.fillRoundRect(Slider_X, Slider_Y, Slider_W, Slider_H, 2, WHITE);
  
  if (ctp.touched()) {
  sliderValue = ((tft.width() + x) - 435)*3.84615385;
  tft.drawRoundRect(thumb_pos, Thumb_Y, Thumb_W, Thumb_H, Thumb_RAD, BLACK);
  tft.drawRoundRect(x1, Thumb_Y, Thumb_W, Thumb_H, Thumb_RAD, BLACK);
  // then draw new thumb
  tft.drawRoundRect(x, Thumb_Y, Thumb_W, Thumb_H, Thumb_RAD, WHITE);
  tft.fillRoundRect(Slider_X, Slider_Y, Slider_W, Slider_H, 2, WHITE);
  }
  
  if (sliderValue < 0) sliderValue = 0;
  if (sliderValue > 500) sliderValue = 500;
  showSliderValue(sliderValue);
  // erase previous thumb by redrawing with background color
}

void sliderHandler2() {
  sliderValue2 = sliderValue_store2;
  thumb_pos_2 = (1.06557*sliderValue_store2) + 115;
  tft.drawRoundRect(thumb_pos_2, Thumb_Y_2, Thumb_W_2, Thumb_H_2, Thumb_RAD_2, WHITE);
  tft.fillRoundRect(Slider_X_2, Slider_Y_2, Slider_W_2, Slider_H_2, 2, WHITE);
  
  if (ctp.touched()) {
  sliderValue2 = ((tft.width() + x_2) - 435)/1.06557;
  tft.drawRoundRect(thumb_pos_2, Thumb_Y_2, Thumb_W_2, Thumb_H_2, Thumb_RAD_2, BLACK);
  tft.drawRoundRect(x1_2, Thumb_Y_2, Thumb_W_2, Thumb_H_2, Thumb_RAD_2, BLACK);
  // then draw new thumb
  tft.drawRoundRect(x_2, Thumb_Y_2, Thumb_W_2, Thumb_H_2, Thumb_RAD_2, WHITE);
  tft.fillRoundRect(Slider_X_2, Slider_Y_2, Slider_W_2, Slider_H_2, 2, WHITE);
  }
  
  if (sliderValue2 < 0) sliderValue2 = 0;
  if (sliderValue2 > 122) sliderValue2 = 122;
  showSliderValue2(sliderValue2); 
}

void sliderHandler3() {
  sliderValue3 = sliderValue_store3;
  thumb_pos_3 = (0.361111*sliderValue_store3) + 115;
  tft.drawRoundRect(thumb_pos_3, Thumb_Y_3, Thumb_W_3, Thumb_H_3, Thumb_RAD_3, WHITE);
  tft.fillRoundRect(Slider_X_3, Slider_Y_3, Slider_W_3, Slider_H_3, 2, WHITE);
  
  if (ctp.touched()) {
  sliderValue3 = ((tft.width() + x_3) - 435)*2.76923077;
  tft.drawRoundRect(thumb_pos_3, Thumb_Y_3, Thumb_W_3, Thumb_H_3, Thumb_RAD_3, BLACK);
  tft.drawRoundRect(x1_3, Thumb_Y_3, Thumb_W_3, Thumb_H_3, Thumb_RAD_3, BLACK);
  // then draw new thumb
  tft.drawRoundRect(x_3, Thumb_Y_3, Thumb_W_3, Thumb_H_3, Thumb_RAD_3, WHITE);
  tft.fillRoundRect(Slider_X_3, Slider_Y_3, Slider_W_3, Slider_H_3, 2, WHITE);
  }
  
  if (sliderValue3 < 0) sliderValue3 = 0;
  if (sliderValue3 > 360) sliderValue3 = 360;
  showSliderValue3(sliderValue3);
}

void sliderHandler4() {
  sliderValue4 = sliderValue_store4;
  thumb_pos_4 = (0.722222*sliderValue_store4) + 115;
  tft.drawRoundRect(thumb_pos_4, Thumb_Y_4, Thumb_W_4, Thumb_H_4, Thumb_RAD_4, WHITE);
  tft.fillRoundRect(Slider_X_4, Slider_Y_4, Slider_W_4, Slider_H_4, 2, WHITE);
  
  if (ctp.touched()) {
  sliderValue4 = ((tft.width() + x_4) - 435)*1.38461538;
  tft.drawRoundRect(thumb_pos_4, Thumb_Y_4, Thumb_W_4, Thumb_H_4, Thumb_RAD_4, BLACK);
  tft.drawRoundRect(x1_4, Thumb_Y_4, Thumb_W_4, Thumb_H_4, Thumb_RAD_4, BLACK);
  // then draw new thumb
  tft.drawRoundRect(x_4, Thumb_Y_4, Thumb_W_4, Thumb_H_4, Thumb_RAD_4, WHITE);
  tft.fillRoundRect(Slider_X_4, Slider_Y_4, Slider_W_4, Slider_H_4, 2, WHITE);
  }
  
  if (sliderValue4 < 0) sliderValue4 = 0;
  if (sliderValue4 > 180) sliderValue4 = 180;
  showSliderValue4(sliderValue4);
}

void startButton() {
  if (wscreen == 1) {
  tft.fillRoundRect(Start_X, Start_Y, Start_W, Start_H, Start_RAD, BLUE);
  tft.setCursor(Start_X+20, Start_Y+5);
  tft.setTextColor(WHITE, BLUE);
  tft.setTextSize(4);
  tft.print("Set Pan");
  }
  else if (wscreen == 2) {
  tft.fillRoundRect(Start_X, Start_Y, Start_W, Start_H, Start_RAD, GREEN);
  tft.setCursor(Start_X+40, Start_Y+5);
  tft.setTextColor(WHITE, GREEN);
  tft.setTextSize(4);
  tft.print("Start");
  }
}

void backButton() {
  tft.fillRoundRect(Back_X, Back_Y, Back_W, Back_H, Back_RAD, BLUE);
  tft.setCursor(Back_X+7, Back_Y+7);
  tft.setTextColor(WHITE, BLUE);
  tft.setTextSize(2);
  tft.print("Back");
}

void settings() {
  tft.fillScreen(BLACK);
  wscreen = 1;
  tft.setCursor(10, 10);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(3);
  tft.print("Linear Settings");
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

void setpan() {
  tft.fillScreen(BLACK);
  wscreen = 2;
  tft.setCursor(10, 10);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(3);
  tft.print("Pan Settings");
  //tft.setRotation(2);
  Thumb_X_3 = initThumb_X_3;
  x_3 = Thumb_X_3 + Thumb_W_3/2;
  x1_3 = x_3;
  sliderHandler3();
  Thumb_X_4 = initThumb_X_4;
  x_4 = Thumb_X_4 + Thumb_W_4/2;
  x1_4 = x_4;
  sliderHandler4();
  startButton();
  backButton();
}

void roundPanValues() {
  roundedPanSpeed = round((sliderValue_store3/360.0)*40)/10.0;
  roundedPanPosition = round((sliderValue_store4/360.0)*40)/10.0;
}

void setup() {
  tft.begin();
  if (! ctp.begin()) {  // pass in 'sensitivity' coefficient
  Serial.println("Couldn't start FT6206 touchscreen controller");
  while (1);
  }
  tft.setRotation(rotation);
  startup();
  translateStepper.connectToPins(stepPin1, dirPin1);
  panStepper.connectToPins(stepPin2, dirPin2);
  
  pinMode(LIMIT_SWITCH_PIN_PAN, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_PIN_TRANSLATE, INPUT_PULLUP);
  
  translateStepper.setStepsPerMillimeter(20);
  translateStepper.setAccelerationInMillimetersPerSecondPerSecond(100);

  panStepper.setStepsPerRevolution(1600);
  panStepper.setAccelerationInRevolutionsPerSecondPerSecond(0.2);

  Serial.begin(9600);

}

void loop() {
  if (!ctp.touched()) { 
    return;
  }
  Serial.println(sliderValue_store1);
  if (wscreen == 0) {
    settings();
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
      x_3 = 320 - p.y;
      y_3 = p.x;
      x_4 = 320 - p.y;
      y_4 = p.x;
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
  if ((x > Start_X) && (x < (Start_X + Start_W)) && (y > Start_Y) && (y <= (Start_Y + Start_H))) {
    if (wscreen == 1) {
    setpan();
    } else if (wscreen == 2) {
      runscreen();

      speedCounterTranslate = sliderValue_store1*10.0;
      positionCounterTranslate = sliderValue_store2*10.0;

      speedCounterPan = sliderValue_store3;
      positionCounterPan = sliderValue_store4;
      
      if (speedCounterPan >= maxPanSpeed) {speedCounterPan = maxPanSpeed;}
      if (speedCounterTranslate >= maxTranslateSpeed) {speedCounterTranslate = maxTranslateSpeed;}
    
      if (positionCounterTranslate >= maxTranslatePosition) {positionCounterTranslate = maxTranslatePosition;}
      if (positionCounterPan >= maxPanPosition) {positionCounterPan = maxPanPosition;}

      roundPanValues();
      translateStepper.setSpeedInMillimetersPerSecond(speedCounterTranslate);
      panStepper.setSpeedInRevolutionsPerSecond(roundedPanSpeed);
      
      translateStepper.setupMoveInMillimeters(positionCounterTranslate);
      panStepper.setupMoveInRevolutions(roundedPanPosition);

      if ((!translateStepper.motionComplete()) || (!panStepper.motionComplete())) {
        translateStepper.processMovement();
        panStepper.processMovement();
        homed = false;
      }
    }
  }
  if ((wscreen == 3) && !((x > BUTTON_X) && (x < (BUTTON_X + BUTTON_W)) && (y > BUTTON_Y) && (y <= (BUTTON_Y + BUTTON_H)))) {
    if ((!translateStepper.motionComplete()) || (!panStepper.motionComplete())) {
        translateStepper.processMovement();
        panStepper.processMovement();
    } else if ((translateStepper.motionComplete()) && (panStepper.motionComplete())) {
      wscreen == 4;
    }
  }
  if ((x > BUTTON_X) && (x < (BUTTON_X + BUTTON_W)) && (y > BUTTON_Y) && (y <= (BUTTON_Y + BUTTON_H))) {
    if (wscreen == 3) {
      stopscreen();
    } else if (wscreen == 4) {
      settings();
      
      if (!homed) {
        translateStepper.moveToHomeInMillimeters(-1, 75, 1300, LIMIT_SWITCH_PIN_TRANSLATE);
        panStepper.moveToHomeInRevolutions(-1, 0.1, 0.5, LIMIT_SWITCH_PIN_PAN);
        homed = true;
      }
    }
  }  

  if (wscreen == 1) {
    Thumb_X = x - Thumb_W/2;
    if (y > Thumb_Y && y < (Thumb_Y + Thumb_H)) {
      if (x > Thumb_X && x < (Thumb_X + Thumb_W)) {
        if (Thumb_X < Slider_X) {
          x =  Slider_X;
        } else if ((Thumb_X + Thumb_W) > (Slider_X+Slider_W)) {
          x = (Slider_X+Slider_W)-Thumb_W;
        }
        sliderHandler();
        x1 = x;
      }
    }
    
   Thumb_X_2 = x_2 - Thumb_W_2/2;
   if (y_2 > Thumb_Y_2 && y_2 < (Thumb_Y_2 + Thumb_H_2)) {
     if (x_2 > Thumb_X_2 && x_2 < (Thumb_X_2 + Thumb_W_2)) {
       if (Thumb_X_2 < Slider_X_2) {
         x_2 =  Slider_X_2;
       } else if ((Thumb_X_2 + Thumb_W_2) > (Slider_X_2+Slider_W_2)) {
         x_2 = (Slider_X_2+Slider_W_2)-Thumb_W_2;
       }
       sliderHandler2();
       x1_2 = x_2;
      }
    } 
  }

  if (wscreen == 2) {
    Thumb_X_3 = x_3 - Thumb_W_3/2;
    if (y_3 > Thumb_Y_3 && y_3 < (Thumb_Y_3 + Thumb_H_3)) {
      if (x_3 > Thumb_X_3 && x_3 < (Thumb_X_3 + Thumb_W_3)) {
        if (Thumb_X_3 < Slider_X_3) {
          x_3 =  Slider_X_3;
        } else if ((Thumb_X_3 + Thumb_W_3) > (Slider_X_3 + Slider_W_3)){
          x_3 = (Slider_X_3 + Slider_W_3)-Thumb_W_3; 
        }
        sliderHandler3();
        x1_3 = x_3;
      }
    }
    Thumb_X_4 = x_4 - Thumb_W_4/2;
    
    if (y_4 > Thumb_Y_4 && y_4 < (Thumb_Y_4 + Thumb_H_4)) {
      if (x_4 > Thumb_X_4 && x_4 < (Thumb_X_4 + Thumb_W_4)) {
        if (Thumb_X_4 < Slider_X_4){
          x_4 =  Slider_X_4;
        } else if ((Thumb_X_4 + Thumb_W_4) > (Slider_X_4 + Slider_W_4)) {
          x_4 = (Slider_X_4 + Slider_W_4)-Thumb_W_4;
        }
        sliderHandler4();
        x1_4 = x_4;
      }
    } 
    if ((x > Back_X) && (x < (Back_X + Back_W)) && (y > Back_Y) && (y <= (Back_Y + Back_H))) {
      settings();
   }
  }
}
