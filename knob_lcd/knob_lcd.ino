#include <LiquidCrystal.h>
#define CLK 8
#define DT 9
#define SW 10

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int speedcounter = 10;
int positioncounter = 5;
int currentStateCLK;
int lastStateCLK;
int DTcurrent;
String currentDir ="";
String lastDir = "";
unsigned long lastButtonPress = 0;

const int b1 = 6;
const int b2 = 7;
int bcount = 0;
int buttonoo = 0;
int buttonState1 = 1;
int buttonState2 = 1;
int lastButtonState1 = 1;
int lastButtonState2 = 1;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Direction:");
  
    // Set encoder pins as inputs
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);
  // Setup Serial Monitor
  Serial.begin(9600);
  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);

  pinMode(b1, INPUT);
  pinMode(b2, INPUT);

  lcd.clear();
}

void loop() {
// Read the current state of CLK
  currentStateCLK = digitalRead(CLK);
  
  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    if (buttonoo == 0) {
    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) == 0) {
      speedcounter --;
      currentDir ="CCW";
    } else {
      // Encoder is rotating CW so increment
      speedcounter ++;
      currentDir ="CW";
    }
    } else {
      if (digitalRead(DT) == 0) {
      positioncounter --;
      currentDir ="CCW";
    } else {
      // Encoder is rotating CW so increment
      positioncounter ++;
      currentDir ="CW";
    }
    }
    

  //  Serial.print("Direction: ");
  //  Serial.print(currentDir);
    Serial.print("Speed Counter: ");
    Serial.println(speedcounter);
    Serial.print("Position Counter: ");
    Serial.println(positioncounter);
 //   Serial.println(currentDir);
  //  Serial.println(lastDir);
  }

  // Remember last CLK state
  lastStateCLK = currentStateCLK;
  lastDir = currentDir;

  // Read the button state
  int btnState = digitalRead(SW);

  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed!");
    }

    // Remember last button press event
    lastButtonPress = millis();
  }


buttonState1 = digitalRead(b1);
buttonState2 = digitalRead(b2);
if (buttonState1 != lastButtonState1) {
  if (buttonState1 == HIGH) {
    bcount++;
    buttonoo = 0;
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(0, buttonoo);
    lcd.print(">");
  }
  delay(50);
}

lastButtonState1 = buttonState1;

if (buttonState2 != lastButtonState2) {
  if (buttonState2 == HIGH) {
    bcount--;
    buttonoo = 1;
    lcd.setCursor(0, 0);
    lcd.print(" ");
    lcd.setCursor(0, buttonoo);
    lcd.print(">");
  }
  delay(50);
}

lastButtonState2 = buttonState2;

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):

lcd.setCursor(1,0);
lcd.print("Speed: ");
lcd.setCursor(8,0);
lcd.print(speedcounter);
lcd.print(" ");

lcd.setCursor(1,1);
lcd.print("Position: ");
lcd.setCursor(11,1);
lcd.print(positioncounter);
lcd.print(" ");


 /* if (currentDir != lastDir) {
    lcd.print("Direction: ");
    lcd.setCursor(11,0);
    lcd.print(currentDir + " ");
  } else {
  // print the number of seconds since reset:
  lcd.setCursor(11, 0);
  lcd.print(currentDir); } */
  

  // Put in a slight delay to help debounce the reading
  delay(1);
}
