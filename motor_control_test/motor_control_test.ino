#define dirPin 2
#define stepPin 3
#define stepsPerRevolution 200

int revsOnTrack = 16;
int totalSteps = revsOnTrack*stepsPerRevolution;
float startPosition;
float initial;
int goTo;
int speedCounter = 10;
int positionCounter = 20;
int microDelay;

void setup() {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);

    Serial.begin(9600);
    goTo = revsOnTrack*stepsPerRevolution;
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
