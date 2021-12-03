#include "SpeedyStepper.h"

SpeedyStepper translateStepper;
SpeedyStepper panStepper;

#define dirPin1 2
#define stepPin1 3
//#define dirPin2 4
//#define stepPin2 5
#define LIMIT_SWITCH_PIN 6

int maxRevs = 16;

int speedCounterTranslate = 10;
int positionCounterTranslate = 20;
int maxPosition = 100;
bool homed = false;
//int speedCounterPan = 5;
//int positionCounterPan = 1;

void setup() {
  // put your setup code here, to run once:

  translateStepper.connectToPins(stepPin1, dirPin1);
//  panStepper.connectToPins(stepPin2, dirPin2);
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
  translateStepper.setStepsPerRevolution(1600);
  translateStepper.setAccelerationInRevolutionsPerSecondPerSecond(1);
  translateStepper.setSpeedInRevolutionsPerSecond(speedCounterTranslate/10);
//  panStepper.setSpeedInRevolutionsPerSecond(speedCounterPan/10);

  Serial.println(translateStepper.getCurrentPositionInRevolutions());
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!homed) {
    homed = translateStepper.moveToHomeInRevolutions(-1, 1, 16, LIMIT_SWITCH_PIN);
  }

// while(!go) {
//
//while(true){}

  translateStepper.setupMoveInRevolutions((positionCounterTranslate/maxPosition)*maxRevs);
//  panStepper.setupRelativeMoveInRevolutions(0.5);

  while((!translateStepper.motionComplete()) || (!panStepper.motionComplete())) {
    Serial.println(translateStepper.getCurrentPositionInRevolutions());
    translateStepper.processMovement();
    panStepper.processMovement();
    homed = false;
  }

  delay(2000);

// while(!home)
}
