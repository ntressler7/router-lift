#include <LiquidCrystal.h>

// Rotary Encoder Declarations
const int CLK = 2, DT = 3;
int counter = 0;
int lastCounter = 0;
int counterMax;
int stateCLK;
int lastStateCLK;

// LCD Declarations
const int RS = 7, EN = 8, D4 = 9, D5 = 10, D6 = 11, D7 = 12;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
uint8_t arrow[8] = {0x0, 0x04, 0x06, 0x1f, 0x06, 0x04, 0x00, 0x00};

// Menu Declarations
String mainMenu[] = {"Manual Mode", "Set Height", "Change Height"};
int mainMenuCounterMax = sizeof(mainMenu) / sizeof(mainMenu[0]) - 1;

int arrowPos;

void setup() {
  Serial.begin(9600);

  // Rotary Encoder Setup
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);

  lastStateCLK = digitalRead(CLK);

  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);

  // LCD Setup
  lcd.begin(16, 2);
  lcd.createChar(1, arrow);

  counter = 0;
  counterMax = mainMenuCounterMax;
  print(0, mainMenu[0]);
  print(1, mainMenu[1]);
  arrowPos = moveArrow(0);
}

void loop() {
  if (counter > lastCounter) {
    if (arrowPos == 0) {
      arrowPos = moveArrow(1);
    } else {
      print(0, mainMenu[1]);
      print(1, mainMenu[2]);
    }
    lastCounter = counter;
  } else if (counter < lastCounter) {
    if (arrowPos == 1) {
      arrowPos = moveArrow(0);
    } else {
      print(0, mainMenu[0]);
      print(1, mainMenu[1]);
    }
    lastCounter = counter;
  }
}

int moveArrow(bool line) {
  if (line) {
    lcd.setCursor(0, 0);
    lcd.print(' ');
    lcd.setCursor(0, 1);
    lcd.write(1);
  } else {
    lcd.setCursor(0, 1);
    lcd.print(' ');
    lcd.setCursor(0, 0);
    lcd.write(1);
  }
  return int(line);
}

void print(bool line, String text) {
  String spaces = "";
  if (text.length() < 15) {
    for (int i = 0; i < (15 - text.length()); i++) {
      spaces += " ";
    }
  }

  if (line) {
    lcd.setCursor(1, 1);
    lcd.print(text + spaces);
  } else {
    lcd.setCursor(1, 0);
    lcd.print(text + spaces);
  }
}

void updateEncoder() {
  stateCLK = digitalRead(CLK);

  if (stateCLK != lastStateCLK && stateCLK == 1) {
    if (digitalRead(DT) != stateCLK) {
      counter--;
    } else {
      counter++;
    }

    counter = constrain(counter, 0, counterMax);
  }

  lastStateCLK = stateCLK;
}
