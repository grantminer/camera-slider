#include "SpeedyStepper.h"

SpeedyStepper UUT;

#define dirPin 2
#define stepPin 3
#define LIMIT 4

bool homed = false;

void setup() {
  UUT.connectToPins(stepPin, dirPin);

  pinMode(LIMIT, INPUT_PULLUP);

  UUT.setStepsPerRevolution(1600);
}

void loop() {

  while (!homed) {
    homed = UUT.moveToHomeInRevolutions(-1, 0.1, 0.5, LIMIT);
  }

  UUT.setSpeedInRevolutionsPerSecond(1);
  UUT.setupMoveInRevolutions(0.5);

  while (!UUT.motionComplete()) {
    UUT.processMovement();
    homed = false;
  }

  while (!homed) {}
}
