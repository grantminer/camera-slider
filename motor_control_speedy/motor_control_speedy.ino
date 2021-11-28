#include "SpeedyStepper.h"

SpeedyStepper translateStepper;
SpeedyStepper panStepper;

#define dirPin1 2
#define stepPin1 3
#define dirPin2 4
#define stepPin2 5
#define LIMIT_SWITCH_PIN 6

int maxRevs = 16;

int speedCounterTranslate = 10;
int positionCounterTranslate = 20;
int speedCounterPan = 5;
int positionCounterPan = 1;

void setup() {
  // put your setup code here, to run once:

  translateStepper.connectToPins(stepPin1, dirPin1);
  panStepper.connectToPins(stepPin2, dirPin2);
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
  translateStepper.setSpeedInRevolutionsPerSecond(speedCounterTranslate/10);
  panStepper.setSpeedInRevolutionsPerSecond(speedCounterPan/10);
  
  translateStepper.moveToHomeInRevolutions(-1, 1, 16, LIMIT_SWITCH_PIN);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  translateStepper.setupMoveInRevolutions((positionCounterTranslate/20)*maxRevs);
  panStepper.setupRelativeMoveInRevolutions(0.5);

  while((!translateStepper.motionComplete()) || (!panStepper.motionComplete())) {
    translateStepper.processMovement();
    panStepper.processMovement();
  }

  delay(2000);

  translateStepper.setupMoveInRevolutions(0);
  panStepper.setupRelativeMoveInRevolutions(-0.5);

  while((!translateStepper.motionComplete()) || (!panStepper.motionComplete())) {
    translateStepper.processMovement();
    panStepper.processMovement();
  }

  delay(10000);
}
