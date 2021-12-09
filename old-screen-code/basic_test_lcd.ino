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

Sd2Card card;
SdVolume volume;


int x=0;                                //Offset postion of title  
int encoderPos = 1;                     //Current encoder position
int eLast;                   //Used to decode rotory encoder, last value
int eNow;                     //Used to decode rotory encoder, current value
char posStr[4];                         //Char array to store encoderPos as a string  
char tmp_string[16];
int enc_pin_status;                     //Last read status of the encoder button
int scroll_direction=1;                 //Direction of title scroll, 1 right, -1 left
unsigned long previousMillis = 0;       //Previous Millis value
unsigned long currentMillis;            //Current Millis value
const long interval = 1000/4;           //How often to run the display loop, every 1/3 of a second aproximatly 



// SPI Com: SCK = en = 23, MOSI = rw = 17, CS = di = 16
U8GLIB_ST7920_128X64_1X u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS);

void setup() {  
  pinMode(BTN_EN1, INPUT);              // Set BTN_EN1 as an unput, half of the encoder
  digitalWrite(BTN_EN1, HIGH);          // turn on pullup resistors
  pinMode(BTN_EN2, INPUT);              // Set BTN_EN2 as an unput, second half of the encoder
  digitalWrite(BTN_EN2, HIGH);          // turn on pullup resistors
  pinMode(BTN_ENC, INPUT);              // Set BTN_ENC as an unput, encoder button
  digitalWrite(BTN_ENC, HIGH);          // turn on pullup resistors
  u8g.setFont(u8g_font_helvR08);        //Set the font for the display
  u8g.setColorIndex(1);                 // Instructs the display to draw with a pixel on. 
  eLast = digitalRead(BTN_EN1);
  Serial.begin(9600);
}

//Main arduino loop
void loop() {
      eNow = digitalRead(BTN_EN1);
    if ((eLast == LOW) && (eNow == HIGH)) { //dial has been turned
    if (digitalRead(BTN_EN2) == HIGH) { //dial is turning cw
      encoderPos++;
    }
    if (digitalRead(BTN_EN2) == LOW) { //dial is turning ccw
      encoderPos--;
    }
  }
    eLast = eNow;
  //check if it is time to update the display 
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    //read the encoder button status
    enc_pin_status = digitalRead(BTN_ENC);

    //Draw new screen
    u8g.firstPage();
    do {  
      draw();
    } while( u8g.nextPage() );

  }
}

//Assemble the display  
void draw(){
/*  // Read the encoder and update encoderPos    
  encoder0PinNow = digitalRead(BTN_EN1);
  if ((encoder0PinALast == LOW) && (encoder0PinNow == HIGH)) {
    if (digitalRead(BTN_EN2) == LOW) {
      encoderPos++;
    } else{
      encoderPos--;
    }
  }
    encoder0PinALast = encoder0PinNow; */
  u8g.setColorIndex(0);
  u8g.drawBox(0,0,128,64);
  u8g.setColorIndex(1);

  u8g.drawStr( 2, 4*9, "Enc pin status:");
  if (enc_pin_status) u8g.drawStr( 84, 4*9, "Open");
  else u8g.drawStr( 84, 4*9, "Closed");

  u8g.drawStr( 2, 6*9, "Encoder value:");
  sprintf (posStr, "%d", encoderPos);
  u8g.drawStr( 84, 6*9, posStr );
 

  u8g.drawFrame(0,0,128,64);
}
