#include "Arduino.h"
/*
Include these libraries in main sketch
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "LCD20x04.h"
*/

#define I2C_ADDR 0x27 // <<----- Add your address here. Find it from I2C Scanner
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7



LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);


void clearRows(int row) {
  lcd.setCursor(0, row);
  lcd.print("                    ");
}

void initializeLCD(void) {
  lcd.begin (20, 4); // or (20,4)

  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);

  // Begin writing startup message.
  lcd.setCursor (10 - sizeof("CCNY") / 2, 0); // (column, row)
  lcd.print("CCNY");
  lcd.setCursor (10 - sizeof("Smart Grid Lab") / 2, 1);
  lcd.print("Smart Grid Lab");
  lcd.setCursor (10 - sizeof("Prof. A. Mohamed") / 2, 2);
  lcd.print("Prof. A. Mohamed");
  lcd.setCursor (10 - sizeof("ST 6/643") / 2, 3);
  lcd.print("ST 6/638");
  //delay(6000);
  delay(3000);

  clearRows(0), clearRows(1), clearRows(2), clearRows(3);
  lcd.setCursor(10 - sizeof("Ready for input.") / 2, 1);
  lcd.print("Ready for input.");

  Serial.println("LCD initialized.");
}
/*
void cautionMessage(void) {
  clearRows(0), clearRows(1), clearRows(2), clearRows(3);

  lcd.setCursor (10 - sizeof("Caution!") / 2, 0); // (column, row);
  lcd.print("Caution!");
  delay(2750);

  lcd.setCursor (10 - sizeof("Grizzly bear inside!") / 2, 1);
  lcd.print("Grizzly bear inside!");
  delay(2500);

  lcd.setCursor (10 - sizeof("DO NOT") / 2, 2);
  lcd.print("DO NOT");
  lcd.setCursor (10 - sizeof("OPEN!!!!") / 2, 3);
  lcd.print("OPEN!!!!");
  delay(6000);

  clearRows(0), clearRows(1), clearRows(2), clearRows(3);
}*/

void LCDprintCurrentState(int currentState[4]) {
  clearRows(0), clearRows(1), clearRows(2), clearRows(3);
 

  lcd.setCursor(0, 0);
  lcd.print("Load status:");

  // Print phase A status
  lcd.setCursor(0, 1);
  lcd.print("AC= A:");
  if (currentState[0] < 10)
    lcd.print(currentState[0]), lcd.print("  ");
    //lcd.print(" "), lcd.print(currentState[0]), lcd.print(" ");
  else
    lcd.print(currentState[0]), lcd.print(" ");

  // Print phase B status
  lcd.setCursor(9, 1);
  lcd.print("B:");
  if (currentState[1] < 10)
    lcd.print(currentState[1]), lcd.print("  ");
    //lcd.print(" "), lcd.print(currentState[1]), lcd.print(" ");
  else
    lcd.print(currentState[1]), lcd.print(" ");

  // Print phase C status
  lcd.setCursor(14, 1);
  lcd.print("C:");
  if (currentState[2] < 10)
    lcd.print(currentState[2]), lcd.print("  ");
    //lcd.print(" "), lcd.print(currentState[2]), lcd.print(" ");
  else
    lcd.print(currentState[2]), lcd.print(" ");

  // Print DC status
  lcd.setCursor(0, 2);
  lcd.print("DC= ");
  lcd.print(currentState[3]);
}

void LCDprintInputSequence(char inputChar, char inputSequence[14]) {
  clearRows(0), clearRows(1), clearRows(2), clearRows(3);

  if (inputChar == '#') {
    lcd.setCursor(10 - sizeof("Input sent") / 2, 1);
    lcd.print("Input sent");

    lcd.setCursor(0, 2);
    int i = 0;
    while (inputSequence[i] != '?' && inputSequence[i] != '#') {
      lcd.print(inputSequence[i]);
      i++;
    }
  }
  else if (inputChar == '*') {
    lcd.setCursor(10 - sizeof("CANCEL") / 2, 1);
    lcd.print("CANCEL");
    lcd.setCursor(10 - sizeof("Input reset.") / 2, 2);
    lcd.print("Input reset.");
  }
  else {
    lcd.setCursor(0, 1);
    lcd.print("Input Sequence: ");

    lcd.setCursor(0, 2);
    int i = 0;
    while (inputSequence[i] != '?' && inputSequence[i] != '#') {
      lcd.print(inputSequence[i]);
      i++;
      delay(30);
    }
  }
}

// Use this one after a serial input
void LCDprintInputSequence(char inputSequence[14]) {
  clearRows(0), clearRows(1), clearRows(2), clearRows(3);

  lcd.setCursor(0, 1);
  lcd.print("Input Sequence: ");

  lcd.setCursor(0, 2);
  int i = 0;
  while (inputSequence[i] != '?' && inputSequence[i] != '#') {
    lcd.print(inputSequence[i]);
    i++;
    delay(80);
  }
}


