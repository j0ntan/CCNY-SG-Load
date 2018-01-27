// Author: Jonathan Gamboa
// version: ?.?
// Institution: Smart Grid Lab, City College of New York (ST 643)

#include "include/CCNY_Smart_Grid_Load_files.h"

Arduino* arduino = new HardwareArduino;
Keypad* keypad = new HardwareKeypad(37, 36, 35, 34, 33, 32, 31, 30);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
}

void loop() {
  if (keypadButtonWasPressed()) {
    String input_from_keypad = recordKeypadSequence<String>();
  }
}
