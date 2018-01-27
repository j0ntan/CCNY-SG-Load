// Author: Jonathan Gamboa
// version: ?.?
// Institution: Smart Grid Lab, City College of New York (ST 643)

#include "include/CCNY_Smart_Grid_Load_files.h"

RelayState relay_state;
Arduino* arduino = new HardwareArduino;
Keypad* keypad = new HardwareKeypad(37, 36, 35, 34, 33, 32, 31, 30);
ShiftRegister shiftregister{2, 3, 4};

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
}

void loop() {
  if (keypadButtonWasPressed()) {
    String input_from_keypad = recordKeypadSequence<String>();
    TokenSet tokens = scan(input_from_keypad);
    if (!tokens.containsInvalid()) {
      ParseAnalysis analysis = analyzeTokens(tokens);
      if (hasNoParseErrors(analysis)) {
        parseNewRelayState(analysis, relay_state);
        ACRelayBits bits = encode(relay_state);
      }  // else, parse error reported
    } else
      ;  // report invalid char error
  }
}
