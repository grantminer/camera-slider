#include "SpeedyStepper.h"

SpeedyStepper translateStepper;
SpeedyStepper panStepper;

#define dirPin1 2
#define stepPin1 3
#define dirPin2 4
#define stepPin2 5
#define LIMIT_SWITCH_PIN 6

int speedCounterTranslate = 100;
int positionCounterTranslate = 20;

int speedCounterPan = 5;
int positionCounterPan = 1;

bool translateHomed = false;

int maxTranslatePosition = 120;
int maxPanPosition = 180;

int maxTranslateSpeed = 100;
int maxPanSpeed = 1;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

void setupSteppers() {
  if (speedCounterPan >= 1) {speedCounterPan = 1;}
  if (speedCounterTranslate >= maxTranslateSpeed) {speedCounterTranslate = maxTranslateSpeed;}

  if (positionCounterTranslate >= maxTranslatePosition) {positionCounterTranslate = maxTranslatePosition;}
  if (positionCounterPan >= maxPanPosition) {positionCounterPan = maxPanPosition;}

  translateStepper.connectToPins(stepPin1, dirPin1);
  panStepper.connectToPins(stepPin2, dirPin2);
  
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
  
  translateStepper.setStepsPerMillimeter(20);
  translateStepper.setAccelerationInMillimetersPerSecondPerSecond(30);
  translateStepper.setSpeedInMillimetersPerSecond(speedCounterTranslate);
  
  panStepper.setSpeedInRevolutionsPerSecond(speedCounterPan*4);
}

void homeTranslate() {
  while (!translateHomed) {
    translateHomed = translateStepper.moveToHomeInMillimeters(-1, 50, 1300, LIMIT_SWITCH_PIN);
  }
}

void goSteppers() {
  translateStepper.setupMoveInMillimeters(positionCounterTranslate);
  panStepper.setupRelativeMoveInRevolutions(positionCounterPan/360);

  while((!translateStepper.motionComplete()) || (!panStepper.motionComplete())) {
    translateStepper.processMovement();
    panStepper.processMovement();
    translateHomed = false;
  }
}
