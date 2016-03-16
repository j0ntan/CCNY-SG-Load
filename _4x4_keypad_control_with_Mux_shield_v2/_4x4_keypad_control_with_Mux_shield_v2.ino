// Difference from origianl is that the input from the
// keypad are read from the Arduino instead of the Mux shield.
// For proper operation, the Mux sheild must not use input
// pullup pins, therefore restricting keypad connection.

// Code is also much cleaner by moving function definitions
// to an external library that I can call upon.


// Keypad library
#include "KeypadInput4x4.h"


// MuxShield libraries
#include "MuxShield.h"
#include "RelayOutput.h"



// LCD libraries
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include "LCD20x04.h"





void setup() {
  Serial.begin(9600);
  initializeKeypadIO();
  initializeMuxShield();
  initializeLatch(3, 5, 9, 13);
  initializeLCD();
  delay(2500);
  //cautionMessage(), delay(3500);
  LCDprintCurrentState(currentState);
}

void loop() {
  if (detectButtonPress()) {
    Serial.println("Pushed");


    // Pressed key rows were found with
    // detectButtonPress(), columns are found next.
    findWhichKeys();

    // Gets the character of the pressed key. For multiple
    // key presses, we get '?', which will be ingored later.
    char keyChar = getKeyChar();
    Serial.print("key pressed: "), Serial.println(keyChar);


    // Stores the input sequence for a maximum of 13 inputs.
    // After 13 inputs, any more inputs will replace the last
    // input in the sequence. Any multiple presses will not be
    // stored in the sequence.
    storeKeyInput(keyChar);

    LCDprintInputSequence(keyChar, inputSequence);

    if (checkForENTER(keyChar)) {
      Serial.println("ENTER");

      if (checkForValidInput()) {
        Serial.println("First input validation passed.");

        // Extracts the desired output state from the input
        // sequence and stores this in an array. The array follows
        // as [Astate, Bstate, Cstate, Dstate]. For example,
        // an input of A 8 C 3 D 4 gives [8,-1,3,4], where
        // -1 means phase B will not change its current state.
        getDesiredState();

        // Makes the appropriate changes to the output current state.
        activateOutput(3, 5, 9);

        // A delay between activating each phase.
        delay(2000);
        LCDprintCurrentState(currentState);
      }
      else
        // Error message will appear in serial window with some description.
        // User notofied on LCD on invalid input.
        LCDprintInvalidInput();


      // Get ready for the next input
      resetSequence();
      LCDprintCurrentState(currentState);

    }
    else if (checkForCANCEL(keyChar)) {
      Serial.println("CANCEL");

      // Get ready for the next input
      resetSequence();
      delay(2000);
      LCDprintCurrentState(currentState);
    }
    else
      ; // Do nothing. Wait for next input

    Serial.println();
    Serial.println();

    //LCDprintCurrentState(currentState);

  }
  else if (Serial.available() > 0) {
    // read and store serial data and use some previous functions.
    // at this point, it's as if the keypad enter key was pressed,
    // so we continue with proceeding functions as above.
    readAllSerialInput();

    if (checkForValidInput()) {
      Serial.println("First input validation passed.");
      LCDprintInputSequence(inputSequence);
      // Extracts the desired output state from the input
      // sequence and stores this in an array. The array follows
      // as [Astate, Bstate, Cstate, Dstate]. For example,
      // an input of A 8 C 3 D 4 gives [8,-1,3,4], where
      // -1 means phase B will not change its current state.
      getDesiredState();


      // Makes the appropriate changes to the output current state.
      activateOutput(3, 5, 9);

      delay(2000);
      LCDprintCurrentState(currentState);
    }
    else; // Error message will appear with some description.

    resetSequence();
    Serial.println();

    //LCDprintCurrentState(currentState);
  }
  else
    ; // Do nothing. Wait for next input

  delay(300);
}






void LCDprintInvalidInput(void) {
  clearRows(0), clearRows(1), clearRows(2), clearRows(3);

  lcd.setCursor(10 - sizeof("Invalid input.") / 2, 1);
  lcd.print("Invalid input.");

  lcd.setCursor(10 - sizeof("Re-enter sequence.") / 2, 2);
  lcd.print("Re-enter sequence.");

  delay(1500);
}






