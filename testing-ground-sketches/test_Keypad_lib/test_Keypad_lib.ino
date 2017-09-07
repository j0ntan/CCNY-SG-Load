#include "Keypad_for_load_emulator.h"

Keypad keypad;

void setup() {
  Serial.begin(9600);

  keypad.set_pins(4,5,6,7,10,11,12,13);
  keypad.initialize();
  //initializeKeypadIO();
  //initializeKeypadIO(2, 3, 4, 5, 6, 7, 8, 9);
}

void loop() {
  if (keypad.check_any_press()) {
    char asdf = keypad.find_pressed_key();
  }
  else;
  //if (Serial.available() > 0) {

    //readAllSerialInput();

    //if (checkForValidInput()) {
      //Serial.println("First input validation passed.");

      // Extracts the desired output state from the input
      // sequence and stores this in an array. The array follows
      // as [Astate, Bstate, Cstate, Dstate]. For example,
      // an input of A 8 C 3 D 4 gives [8,-1,3,4], where
      // -1 means phase B will not change its current state.
      //getDesiredState();

      // Makes the appropriate changes to the output current state.
      //activateOutput(3, 5, 9);
   // }
    //else; // Error message will appear with some description.

    //resetSequence();
    //Serial.println();
//  }
  //else
    //; // Do nothing. Wait for next input

  //delay(180);

}




















