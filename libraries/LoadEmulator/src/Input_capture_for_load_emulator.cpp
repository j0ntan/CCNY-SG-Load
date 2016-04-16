#include "Input_capture_for_load_emulator.h"

InputCapture::
InputCapture() {
  reset_keypad_vars();
}

// Keypad functions
void InputCapture::
setKeypadActive() {
  reset_keypad_vars();
  stillTakingKeypresses = true;
}

void InputCapture::
captureKeypress(bool heldStatus, char keypress) {
  /*
  NOTE: keypress is returned by getKEY function, which also sets
  heldStatus. It seems logical to have keypress as the first parameter,
  but in that case, the first event of a button hold was not detected.
  It seems that Arduino compiler evaluates right-to-left, so that having
  keypress to right results in correct execution. */

  if (keypress == '*') {
    if (heldStatus == true) {
      // CANCEL input sequence
      reset_keypad_vars();
      Serial.println(F("Input seq: Pressed CANCEL."));
      captureStatus  = "                     ";
      captureStatus += "Pressed CANCEL.      ";
      captureStatus += "Resetting input.     ";
      captureStatus += "                     ";
    }
    else {
      if (inputString.length() > 0) {
        // Erase last input from string
        Serial.println(F("Input seq: Pressed ERASE."));
        inputString.remove(inputString.length() - 1);
        captureStatus  = "Erased last input.  ";
        captureStatus += "Input sequence:     ";
        captureStatus += inputString;
        captureStatus += "                    ";
        captureStatus += "                    ";
      }
      else {
        // Cannot erase from empty string
        Serial.println(F("Input seq: Cannot ERASE."));
        captureStatus  = "                    ";
        captureStatus += "    Cannot erase    ";
        captureStatus += "   empty sequence.  ";
        captureStatus += "                    ";
      }
    }
  }
  else if (keypress == '#') {
    // ENTER finishes input sequence
    reset_keypad_vars();
    Serial.println(F("Input seq: Pressed ENTER."));
    captureStatus  = "                    ";
    captureStatus += "Pressed ENTER.      ";
    captureStatus += "Activating relays.  ";
    captureStatus += "                    ";
  }
  else {
    // Accept input, not exceeding 11 characters
    if (inputString.length() == 11) {
      inputString[10] = keypress;
    }
    else{
      inputString += keypress;
    }
    Serial.println(F("Input seq: Got an input."));
    captureStatus  = "                    ";
    captureStatus += "Input sequence:     ";
    captureStatus += inputString;
    captureStatus += "                    ";
    captureStatus += "                    ";
  }
}

void InputCapture::
reset_keypad_vars(void) {
  stillTakingKeypresses = false;

  inputString = "";
  captureStatus = "";
}
