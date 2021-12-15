#include <LiquidCrystal.h>
#include <SPI.h>
#include <U8glib.h>

#define DOGLCD_CS       45
#define DOGLCD_MOSI     47
#define DOGLCD_SCK      43


#define CLK 8
#define DT 9
#define SW 10
#define dirPin 22
#define stepPin 23
#define stepsPerRevolution 200

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

int microDelay = 500;

const int ledPin = 13;


unsigned long previousMillis = 0;       //Previous Millis value
unsigned long currentMillis;            //Current Millis value
const long interval = 1000/4;           //How often to run the display loop, every 1/3 of a second aproximatly 

U8GLIB_ST7920_128X64_1X u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Direction:");
  
    // Set encoder pins as inputs
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  // Setup Serial Monitor
  Serial.begin(9600);
  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);

  pinMode(b1, INPUT);
  pinMode(b2, INPUT);

  lcd.clear();

  pinMode(ledPin, OUTPUT);

  u8g.setFont(u8g_font_helvR08);        //Set the font for the display
  u8g.setColorIndex(1);           
}

void loop() {
// Read the current state of CLK
  currentStateCLK = digitalRead(CLK);
  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 0){
      if (digitalRead(DT) == 0) {
      positioncounter --;
      currentDir ="CCW";
    } else {
      // Encoder is rotating CW so increment
      positioncounter ++;
      currentDir ="CW";
    }
    }

  // Remember last CLK state
  lastStateCLK = currentStateCLK;
  lastDir = currentDir;

lcd.setCursor(1,1);
lcd.print("Position: ");
lcd.setCursor(11,1);
lcd.print(positioncounter);
lcd.print(" ");

Serial.print(positioncounter);

  /* u8g.firstPage();
    do {  
      draw();
    } while( u8g.nextPage() ); */


  // Put in a slight delay to help debounce the reading
  delay(1);
}

void draw(){
  u8g.setColorIndex(0);
  u8g.drawBox(0,0,128,64);
  u8g.setColorIndex(1);

 // u8g.drawStr( 2, 6*9, "Encoder value:");
 // sprintf (posStr, "%d", encoderPos);
 // u8g.drawStr( 84, 6*9, posStr );
 

  u8g.drawFrame(0,0,128,64);
}
