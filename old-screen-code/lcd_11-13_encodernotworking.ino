#include <U8glib.h>
#include <SPI.h>

#define DOGLCD_CS       45
#define DOGLCD_MOSI     47
#define DOGLCD_SCK      43
#define ENC_CLK         8 //31
#define ENC_DT          9 //33
#define ENC_SW          10 //35

            //Last read status of the stop pin, start at 1 to ensure buzzer is off
int encoderPos = 1;                     //Current encoder position
int currentencoder;                   //Used to decode rotory encoder, current value
int lastencoder;                     //Used to decode rotory encoder, last value
int encoderbuttonstatus;                     //Last read status of the encoder button
      //How often to run the display loop, every 1/3 of a second aproximatly 

U8GLIB_ST7920_128X64_1X u8g(DOGLCD_SCK, DOGLCD_MOSI, DOGLCD_CS);

void setup() {
  // put your setup code here, to run once:
     // turn on pullup resistors
  pinMode(ENC_CLK, INPUT);              // Set BTN_EN1 as an unput, half of the encoder
 //digitalWrite(BTN_EN1, HIGH);          // turn on pullup resistors
  pinMode(ENC_DT, INPUT);              // Set BTN_EN2 as an unput, second half of the encoder
  //digitalWrite(BTN_EN2, HIGH);          // turn on pullup resistors
  pinMode(ENC_SW, INPUT_PULLUP);              // Set BTN_ENC as an unput, encoder button
  //digitalWrite(BTN_ENC, HIGH);          // turn on pullup resistors
  u8g.setFont(u8g_font_helvR08);        //Set the font for the display
  u8g.setColorIndex(1);  

  Serial.begin(9600);

  lastencoder = digitalRead(ENC_CLK);
}

void loop() {
 /*encoder0PinNow = digitalRead(BTN_EN1);
  if ((encoder0PinALast == LOW) && (encoder0PinNow == HIGH)) {
    if (digitalRead(BTN_EN2) == LOW) {
      encoderPos++;
      Serial.print(encoderPos);
    } else {
      encoderPos--;
      Serial.print(encoderPos);
    }
  }
  encoder0PinALast = encoder0PinNow;*/
  currentencoder = digitalRead(ENC_CLK);
  Serial.print(currentencoder);

  if (currentencoder != lastencoder  && currentencoder == 0){
    if (digitalRead(ENC_DT) == 0) {
      encoderPos --;
    } else {
      // Encoder is rotating CW so increment
      encoderPos ++;
    }

   currentencoder = lastencoder;

  }
  
  u8g.firstPage();
  do {
  draw();
  } while(u8g.nextPage());
}

 void draw(){
  u8g.setColorIndex(0); // set pixel to clear
  u8g.drawBox(0,0,128,64); // draw a box that is the width and height of the entire display, aka clear the display
  u8g.setColorIndex(1); // set pixel to draw/white (everything after this will actually fill in a pixel)

  u8g.drawStr( 2, 5*9, "Encoder value:");
  //sprintf (posStr, "%d", encoderPos);
  //u8g.drawStr( 84, 5*9, posStr );

  u8g.drawFrame(0,0,128,64);
}
