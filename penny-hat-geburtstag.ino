#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "pitches.h"

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

static const uint8_t PROGMEM
heart_bmp[] =
{ B01100110,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100,
  B00011000,
};

int melody[] = {
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4, NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
};

//note durations: 4 = quarter note, 8 = eight note, etc.
int noteDurations[] = {
  8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4, 4, 4, 8, 8, 4, 4, 4, 2, 
};

struct TuneState {
  int currentNote;
};

struct TuneState tuneState;

struct MatrixState {
  int8_t currentX;
};

static char text[] = "Penny hat Geburtstag!!";

struct MatrixState matrixState;
unsigned long matrixWindow;
unsigned long timePerChar = 80;

void setup() {
  
  Serial.begin(9600);
  matrix.begin(0x70);  // pass in the address
  matrix.setTextSize(1);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);

  tuneState.currentNote = -1;
  
  matrixState.currentX = 9;
  matrixWindow = strlen(text) * timePerChar;
}

void printText(unsigned long ms) {
  // ufff...helpless :D
  unsigned long windowMs = ms % windowMs;
  int8_t x = 8 - (windowMs/80);

  if (x != matrixState.currentX && x<=8) {
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print("Penny hat Geburtstag!!");
    matrix.writeDisplay();
  }

  if (x == 127 || x == 112 || x == 97 || x == 82 || x == 67 || x == 52 || x == 37 || x == 22) {
    matrix.clear();
    matrix.drawBitmap(0, 0, heart_bmp, 8, 8, LED_ON);
    matrix.writeDisplay();
  }

  if (x == 120 || x == 105 || x == 90 || x == 75 || x == 60 || x == 45 || x == 30 || x == 15) {
    matrix.clear();
    matrix.writeDisplay();
  }
  matrixState.currentX = x;
}

void playTune(unsigned long ms) {
  unsigned long currentMs = 0;
  int noteDuration;
  int i;
  for (i=0;i<25;i++) {
    noteDuration = 1000 / noteDurations[i];

    currentMs += noteDuration * 1.30;
    if (currentMs >= ms) {
      break;
    }
    
  }
  
  if (i != tuneState.currentNote) {
    tuneState.currentNote = i;
    tone(8, melody[i], noteDuration);
  }
}

void loop() {
  unsigned long ms = millis();
  printText(ms);
  playTune(ms);
//  tone(8, NOTE_C4, 2000);

//
//  for (int8_t x = 8; x > -128; x--) {
//    matrix.clear();
//    matrix.setCursor(x, 0);
//    matrix.print("Penny hat Geburtstag!!");
//    matrix.writeDisplay();
//    delay(80);
//  }
//
//  for (int i = 0; i < 4; i++) {
//    matrix.clear();
//    matrix.writeDisplay();
//    delay(500);
//    matrix.drawBitmap(0, 0, heart_bmp, 8, 8, LED_ON);
//    matrix.writeDisplay();
//    delay(500);
//  }
}
