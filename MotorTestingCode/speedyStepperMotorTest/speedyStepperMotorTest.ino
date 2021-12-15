#include "SpeedyStepper.h"

SpeedyStepper translateStepper;
SpeedyStepper panStepper;

#define dirPin1 2
#define stepPin1 3
#define dirPin2 4
#define stepPin2 5
#define LIMIT_SWITCH_PIN_TRANSLATE 6
#define LIMIT_SWITCH_PIN_PAN 7

int speedCounterTranslate = 200;
int positionCounterTranslate = 1150;
bool homed = false;
int speedCounterPan = 30;
int positionCounterPan = 90;

int maxTranslatePosition = 1150;
int maxPanPosition = 180;
int maxTranslateSpeed = 200;
int maxPanSpeed = 360;

void setup() {
  // put your setup code here, to run once:

  if (speedCounterPan >= maxPanSpeed) {speedCounterPan = maxPanSpeed;}
  if (speedCounterTranslate >= maxTranslateSpeed) {speedCounterTranslate = maxTranslateSpeed;}

  if (positionCounterTranslate >= maxTranslatePosition) {positionCounterTranslate = maxTranslatePosition;}
  if (positionCounterPan >= maxPanPosition) {positionCounterPan = maxPanPosition;}

  translateStepper.connectToPins(stepPin1, dirPin1);
  panStepper.connectToPins(stepPin2, dirPin2);
  
  pinMode(LIMIT_SWITCH_PIN_PAN, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_PIN_TRANSLATE, INPUT_PULLUP);
  
  translateStepper.setStepsPerMillimeter(20);
  translateStepper.setAccelerationInMillimetersPerSecondPerSecond(100);

  panStepper.setStepsPerRevolution(1600);
  panStepper.setAccelerationInRevolutionsPerSecondPerSecond(0.2);


}

void loop() {
  // put your main code here, to run repeatedly:
  if (!homed) {
    bool homed_translate = translateStepper.moveToHomeInMillimeters(-1, 75, 1300, LIMIT_SWITCH_PIN_TRANSLATE);
    //bool homed_pan = panStepper.moveToHomeInRevolutions(-1, 0.1, 0.5, LIMIT_SWITCH_PIN_PAN);
    homed = homed_translate; //&& homed_pan;
  }

// while(!go) {
//
//while(true){}

  translateStepper.setSpeedInMillimetersPerSecond(speedCounterTranslate);
  panStepper.setSpeedInRevolutionsPerSecond((speedCounterPan/360)*4);

  translateStepper.setupMoveInMillimeters(positionCounterTranslate);
  panStepper.setupMoveInRevolutions((positionCounterPan/360)*4);

  while((!translateStepper.motionComplete()) || (!panStepper.motionComplete())) {
    translateStepper.processMovement();
    panStepper.processMovement();
    homed = false;
  }

  while (!homed) {}
}
