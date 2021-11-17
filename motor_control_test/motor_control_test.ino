#define dirPin 2
#define stepPin 3
#define configPin 4
#define stepsPerRevolution 200

int revsOnTrack = 16;
int totalSteps = revsOnTrack*stepsPerRevolution;
float startPosition;
float initial;
int goTo;
int speedCounter = 10;
int positionCounter = 20;
int microDelay;
bool buttonPressed;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int buttonState;
int lastButtonState = LOW;


void setup() {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(configPin, INPUT);

    Serial.begin(9600);
    goTo = revsOnTrack*stepsPerRevolution;

    digitalWrite(dirPin, HIGH);
    while (!buttonPressed) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(1000);
        buttonPress();
    }
}

void loop() {
    readVals();
    findDelay();
    startPos();
  
    digitalWrite(dirPin, LOW);
    for (int i = 0; i < goTo; i ++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(microDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(microDelay);
    }

    delay(2000);
    digitalWrite(dirPin, HIGH);

    for (int i = 0; i < goTo; i ++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(microDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(microDelay);
    }

}

void findDelay() {
    microDelay = 2000 - (speedCounter*100);
    if (microDelay < 1000) {
        microDelay = 1000;
    }
}

 void startPos() {
   startPosition = positionCounter / 20;
   initial = startPosition * revsOnTrack;
   goTo = floor(initial * stepsPerRevolution);
 }

 void readVals() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
    String temp_thresh;
    const byte numChars = 4;
    char receivedChars[numChars];
    char * strtokIndx;

    boolean newData = false;

    if (Serial.available() > 0) {
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
    if (rc == endMarker) {
      strtokIndx = strtok(receivedChars, ",");
      positionCounter = atoi(strtokIndx);
    }
    }
}

void buttonPress() {
    int reading = digitalRead(configPin);

    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }

    if (millis() - lastDebounceTime > debounceDelay) {
        if (reading != buttonState) {
            buttonState = reading;

        if (buttonState) {
            buttonPressed = 1;
        }
        }
    }

    lastButtonState = reading;
}