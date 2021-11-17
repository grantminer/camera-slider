//https://github.com/ellensp/rrd-glcd-tester/blob/master/rrd-glcd-test.ino//
#include <SPI.h>
#include <U8glib.h>

//Lets you test the standard features of a reprap discount full graphics 128x64 LCD
//Test STOP and Encoder button
//Test buzzer (Hold both STOP and Encoder buttons to activate)
//Test SD detect, disply SD card information
//Test Encode left/right
//Verify the LCD is working.

//Standard pins when on a RAMPS 1.4

#define DOGLCD_CS       45
#define DOGLCD_MOSI     47
#define DOGLCD_SCK      43
#define BEEPER_PIN      37
#define KILL_PIN        41
#define UPBUTTON        53
#define DOWNBUTTON      51
#define ENGAGEBUTTON    52


int x=0;                                //Offset postion of title  
int kill_pin_status = 1;                //Last read status of the stop pin, start at 1 to ensure buzzer is off
int encoderPos = 1;                     //Current encoder position
int encoder0PinALast;                   //Used to decode rotory encoder, last value
int encoder0PinNow;                     //Used to decode rotory encoder, current value
char posStr[4];                         //Char array to store encoderPos as a string  
char tmp_string[16];
int enc_pin_status;                     //Last read status of the encoder button
int sd_detect_pin_status = true;               //Last read status of the SD detect pin
int scroll_direction=1;                 //Direction of title scroll, 1 right, -1 left
unsigned long previousMillis = 0;       //Previous Millis value
unsigned long currentMillis;            //Current Millis value
const long interval = 1000/3;           //How often to run the display loop, every 1/3 of a second aproximatly 

int bcount = 10;
int buttonState1 = 1;
int buttonState2 = 1;
int lastButtonState1 = 1;
int lastButtonState2 = 1;

// SPI Com: SCK = en = 23, MOSI = rw = 17, CS = di = 16
U8GLIB_ST7920_128X64_1X u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS);

void setup() {  
  pinMode(KILL_PIN, INPUT);             // Set KILL_PIN as an unput
  digitalWrite(KILL_PIN, HIGH);         // turn on pullup resistors
  u8g.setFont(u8g_font_helvR08);        //Set the font for the display
  u8g.setColorIndex(1);   
  
  pinMode(UPBUTTON, INPUT);             
  pinMode(DOWNBUTTON, INPUT);             
  pinMode(ENGAGEBUTTON, INPUT);             
}

//Main arduino loop
void loop() {

  //check if it is time to update the display 
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    //read the kill pin status
    kill_pin_status = digitalRead(KILL_PIN); 

buttonState1 = digitalRead(UPBUTTON);
buttonState2 = digitalRead(DOWNBUTTON);
if (buttonState1 != lastButtonState1) {
  if (buttonState1 == HIGH) {
    bcount++;
  }
  delay(50);
}

lastButtonState1 = buttonState1;

if (buttonState2 != lastButtonState2) {
  if (buttonState2 == HIGH) {
    bcount--;
  }
  delay(50);
}

lastButtonState2 = buttonState2;
  
    //Check if both Kill switch and encoder are pressed, if so switch on buzzer
    if(kill_pin_status) digitalWrite(BEEPER_PIN, LOW); 
    else digitalWrite(BEEPER_PIN, HIGH);

    //Draw new screen
    u8g.firstPage();
    do {  
      draw();
    } while( u8g.nextPage() );
  }
}

//Assemble the display  
void draw(){
  u8g.setColorIndex(0);
  u8g.drawBox(0,0,128,64);
  u8g.setColorIndex(1);
  
  u8g.drawStr( 2, 10, "RRD GLCD TEST");
  u8g.drawStr( 2, 3*9, "Stop pin status:");
  if (kill_pin_status) u8g.drawStr( 84, 3*9, "Open");
  else u8g.drawStr( 84, 3*9, "Closed");

  u8g.drawStr( 2, 6*9, "Encoder value:");
  sprintf (posStr, "%d", bcount);
  u8g.drawStr( 84, 6*9, posStr);
  

  u8g.drawStr( 2, 7*9, "Buzzer:");
  if (kill_pin_status) u8g.drawStr( 84, 7*9, "Off");
  else u8g.drawStr( 84, 7*9, "On");

  u8g.drawFrame(0,0,128,64);
}
