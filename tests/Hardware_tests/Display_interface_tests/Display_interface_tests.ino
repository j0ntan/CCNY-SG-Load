// This sketch tests the display interface using a 20 x 4 character LCD through
// the I2C connection. Prior to running this set of tests, the user should
// determine the I2C address of the LCD device. An I2C scanner sketch in the
// 'utils' directory can be used to determine this address.

#include "include/Display.h"
#include "include/HardwareDisplay.h"

HardwareDisplay display(0x27);

void setup() {
  Serial.begin(9600);

  // begin tests
  printAZero();
  clearTheScreen();
  printMiddleOfScreen();

  Serial.println(F("\n\nDONE TESTING"));
}

void loop() {}

void waitForEnter() {
  Serial.println(F("Press ENTER to continue...\n"));
  bool enterPressed = false;
  do {
    while (Serial.available())
      if (Serial.read() == '\n') enterPressed = true;
  } while (!enterPressed);
}

void printAZero() {
  Serial.println(F("Test: printAZero"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Verify that the LCD is showing a zero."));

  display.print(F("0"));

  waitForEnter();
}

void clearTheScreen() {
  Serial.println(F("Test: clearTheScreen"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Verify that the LCD has been cleared."));

  display.clear();

  waitForEnter();
}

void printMiddleOfScreen() {
  Serial.println(F("Test: printMiddleOfScreen"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Verify printing to the middle rows."));

  display.clear();
  display.setCursor(0, 1);
  display.print(F("Printing to the "));
  display.setCursor(0, 2);
  display.print(F("middle of the screen."));

  waitForEnter();
}
