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
#define BTN_EN1         31
#define BTN_EN2         33
#define BTN_ENC         35
#define BEEPER_PIN      37
#define KILL_PIN        41


int x=0;   //Offset postion of title
int y=0;  
int kill_pin_status = 1;                //Last read status of the stop pin, start at 1 to ensure buzzer is off
int encoderPos = 1;                     //Current encoder position
int encoder0PinALast;                   //Used to decode rotory encoder, last value
int encoder0PinNow;                     //Used to decode rotory encoder, current value
char posStr[4];                         //Char array to store encoderPos as a string  
char tmp_string[16];
int enc_pin_status;                     //Last read status of the encoder button
int scroll_direction=1;                 //Direction of title scroll, 1 right, -1 left
unsigned long previousMillis = 0;       //Previous Millis value
unsigned long currentMillis;            //Current Millis value
const long interval = 1000/3;           //How often to run the display loop, every 1/3 of a second aproximatly 

// SPI Com: SCK = en = 23, MOSI = rw = 17, CS = di = 16
U8GLIB_ST7920_128X64_1X u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS);

void setup() {  
  pinMode(KILL_PIN, INPUT);             // Set KILL_PIN as an unput
  digitalWrite(KILL_PIN, HIGH);         // turn on pullup resistors
  pinMode(BTN_EN1, INPUT);              // Set BTN_EN1 as an unput, half of the encoder
  digitalWrite(BTN_EN1, HIGH);          // turn on pullup resistors
  pinMode(BTN_EN2, INPUT);              // Set BTN_EN2 as an unput, second half of the encoder
  digitalWrite(BTN_EN2, HIGH);          // turn on pullup resistors
  pinMode(BTN_ENC, INPUT);              // Set BTN_ENC as an unput, encoder button
  digitalWrite(BTN_ENC, HIGH);          // turn on pullup resistors
  u8g.setFont(u8g_font_helvR08);        //Set the font for the display
  u8g.setColorIndex(1);   
  
  // Instructs the display to draw with a pixel on. 
}

//Main arduino loop
void loop() {
  // Read the encoder and update encoderPos    
  encoder0PinNow = digitalRead(BTN_EN1);
  if ((encoder0PinALast == LOW) && (encoder0PinNow == HIGH)) {
    if (digitalRead(BTN_EN2) == LOW) {
      encoderPos++;
    } else {
      encoderPos--;
    }
  }
  encoder0PinALast = encoder0PinNow;


  //check if it is time to update the display 
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  }
    //read the kill pin status
    kill_pin_status = digitalRead(KILL_PIN); 
    //read the encoder button status
    enc_pin_status = digitalRead(BTN_ENC);
    //read the SD detect pin status  

    //Check if both Kill switch and encoder are pressed, if so switch on buzzer
    if(kill_pin_status || enc_pin_status) digitalWrite(BEEPER_PIN, LOW); 
    else digitalWrite(BEEPER_PIN, HIGH);


    //Draw new screen
    u8g.firstPage();
    do {  
      if (enc_pin_status == LOW) draw_settings();
      else draw_intro();
    } while( u8g.nextPage() );

    //Update Title position
    x=x+scroll_direction;
    if (x > 40) scroll_direction = -1;
    if (x < 1) scroll_direction = 1;

    y = 5;
}

//Assemble the display  
void draw_settings(){
  u8g.setColorIndex(0);
  u8g.drawBox(0,0,128,64);
  u8g.setColorIndex(1);
  
  u8g.drawStr( 38, 10, "SETTINGS");
  u8g.drawStr( 2, y*9, ">"); 
  u8g.drawStr( 6, 3*9, "Set Distance:");
  if (kill_pin_status) u8g.drawStr( 88, 3*9, "Open");
  else u8g.drawStr( 84, 3*9, "Closed");

  u8g.drawStr( 6, 4*9, "Set Speed:");
  if (enc_pin_status) u8g.drawStr( 88, 4*9, "Open");
  else u8g.drawStr( 88, 4*9, "Closed");

  u8g.drawStr( 6, 6*9, "Set Rotation:");
  sprintf (posStr, "%d", encoderPos);
  u8g.drawStr( 88, 6*9, posStr );

  u8g.drawFrame(0,0,128,64);
}

void draw_intro(){
  u8g.setColorIndex(0);
  u8g.drawBox(0,0,128,64);
  u8g.setColorIndex(1);
  
  u8g.drawStr(2, 1*9 + 1, "PIE Final Project");
  u8g.drawStr(2, 2*9 + 1, "Group 30");
  u8g.drawStr(2, 3*9 + 1, "Camera Slider");
  u8g.drawStr(2, 5*9 + 1, "Press to continue...");
  
  u8g.drawFrame(0,0,128,64);
}
