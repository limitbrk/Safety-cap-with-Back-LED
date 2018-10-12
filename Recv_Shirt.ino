#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif
#define PIN 6
#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

int left[15][12] = {  
   {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
   {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
   {0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
   {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
   {0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
   {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };

int right[15][12] = {  
   {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
   {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
   {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
   {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0},
   {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
   {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };

  
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(12, 15, PIN,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

boolean turn_blink = true;
boolean alert_blink = true;
int turn_stat=0xae09;
int x = matrix.width();

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);
  matrix.begin();
  matrix.setBrightness(30);
  matrix.setTextWrap(false);
  matrix.setTextColor(matrix.Color(255, 0, 0));
}


void loop() {
  if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();
    turn_stat=value;
    Serial.println(turn_stat,HEX);
    mySwitch.resetAvailable();
  }
  drawLogo(turn_stat,turn_blink);
      turn_blink = !turn_blink;
  drawAlert(alert_blink);
      alert_blink = !alert_blink;
  matrix.show();
  delay(100);
}

void drawLogo(int stat,boolean bnk) {
  matrix.fillScreen(matrix.Color(0, 0, 0));
  if (stat == 0xAE01){
   for(int row = 0; row < 15; row++)
    for(int column = 0; column < 12; column++) 
     if(left[row][column] == 1)
        if(bnk)
          matrix.drawPixel(column,row, matrix.Color(255, 140, 0));
  }
  else if (stat == 0xAE02){
   for(int row = 0; row < 15; row++)
    for(int column = 0; column < 12; column++) 
     if(right[row][column] == 1)
        if(bnk)
          matrix.drawPixel(column,row, matrix.Color(255, 140, 0));
  }
  else if (stat == 0xAE08){
    matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F("ITE  KMITL"));
  if(x>-18)
    matrix.setTextColor(matrix.Color(0, 102, 255));
  else
    matrix.setTextColor(matrix.Color(255, 140, 0));
  if(--x < -60) {
    x = matrix.width();
  }
  matrix.show();
  }
  else if (stat == 0xAE09){}
  }
   
void drawAlert(boolean bnk) {   
  for(int row = 12; row < 15; row++) {
    for(int column = 0; column < 12; column++) 
      if(bnk)
          matrix.drawPixel(column,row, matrix.Color(255,0, 0));
      else
          matrix.drawPixel(column,row, matrix.Color(0,0, 0));
        
   }
   
}
