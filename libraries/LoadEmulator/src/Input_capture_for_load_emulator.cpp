#include "Input_capture_for_load_emulator.h"

InputCapture::
InputCapture() {
  reset_keypad_vars();
  reset_shared_vars();
}

// Keypad functions
void InputCapture::
setKeypadActive() {
  // Set proper variabels to begin taking input.
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
      inputString = "A*";
      stillTakingKeypresses = false;
      Serial.println(F("Input capture: Pressed CANCEL."));
      captureStatus  = "                     ";
      captureStatus += "Pressed CANCEL.      ";
      captureStatus += "Resetting input.     ";
      captureStatus += "                     ";
    }
    else {
      if (inputString.length() > 0) {
        // Erase last input from string
        Serial.println(F("Input capture: Pressed ERASE."));
        inputString.remove(inputString.length() - 1);
        captureStatus  = "Erased last input.  ";
        captureStatus += "Current sequence:     ";
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
    stillTakingKeypresses = false;

    Serial.println(F("Input capture: Pressed ENTER."));
    captureStatus  = "                    ";
    captureStatus += "Pressed ENTER.      ";
    captureStatus += "                    ";
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
}

// Serial monitor functions
void InputCapture::
captureSerialMonitor(void) {
  delay(6); // allow time for serial buffer to fill
  inputString = "";
  while (Serial.available() > 0) {
    inputString += char(Serial.read());
  }
  int inputSize = inputString.length();
  Serial.print(F("Input capture: Received serial input "));
  Serial.println(inputString);
  captureStatus  = "                    ";
  captureStatus += "     Received a     ";
  captureStatus += "    serial input.   ";
  captureStatus += "                    ";
}

// XBee functions
void InputCapture::
linkXBee(HardwareSerial *serial) {
  _HardSerial = serial;
  _HardSerial->begin(9600); // keep default baud rate for now
}

bool InputCapture::
XBeeGotData(void) {
  if (_HardSerial->available() > 0) {
    Serial.println(F("XBee: Incoming data."));

    return true;
  }
  else return false;
}

void InputCapture::
captureXBee(void) {
  delay(6); // allow time for buffer to receive XBee data
  inputString = "";
  while(_HardSerial->available() > 1) {
    inputString += char(_HardSerial->read());
    delay(1); // allow time for serial read execution
  }
  _HardSerial->read(); // read & discard the \CRLF byte from buffer

  Serial.print(F("XBee: The received data is \""));
  Serial.print(inputString);
  Serial.println(F("\"")); Serial.println();

  captureStatus  = "                    ";
  captureStatus += " XBee received:     ";
  captureStatus += inputString;
  for (int i = 0; i < (20 - inputString.length()); i++) {
    captureStatus += " ";
  }
  captureStatus += "                    ";

  send_confirmation();
}

void InputCapture::
send_confirmation() {
  // Not yet implemented
  _HardSerial->println("Input was received.");
}

// shared functions
void InputCapture::
reset_shared_vars() {
  _Reset_flag = false;
  _counter = 0;
  _numbers_outof_range = false;
}

bool InputCapture::
isValid() {
  reset_shared_vars();
  int _A_count = 0, _B_count = 0;
  int _C_count = 0, _D_count = 0;
  int _A_pos, _B_pos, _C_pos, _D_pos;
  bool _illegal_char_exists = false;
  char _illegal_char;

  while (_counter < inputString.length()) {
    switch (inputString[_counter]) {
      case '0':  check_numbers_range(0); break;
      case '1':  check_numbers_range(1); break;
      case '2':  check_numbers_range(2); break;
      case '3':  check_numbers_range(3); break;
      case '4':  check_numbers_range(4); break;
      case '5':  check_numbers_range(5); break;
      case '6':  check_numbers_range(6); break;
      case '7':  check_numbers_range(7); break;
      case '8':  check_numbers_range(8); break;
      case '9':  check_numbers_range(9); break;
      case 'A': _A_pos = _counter; _A_count++; break;
      case 'B': _B_pos = _counter; _B_count++; break;
      case 'C': _C_pos = _counter; _C_count++; break;
      case 'D': _D_pos = _counter; _D_count++; break;
      case '*': _Reset_flag = true; break;
      default:
        _illegal_char_exists = true;
        _illegal_char = inputString[_counter];
        _counter = inputString.length();
        break;
    }
    _counter++;
  }


  if (_illegal_char_exists == true) {
    Serial.print(F("Input capture: Found the illegal character '"));
    Serial.print(_illegal_char);
    Serial.println(F("' in the input."));
    captureStatus  = "      Error in      ";
    captureStatus += "    serial input.   ";
    captureStatus += "  Canceling input.  ";
    captureStatus += "                    ";

    return false;
  }

  else if (_Reset_flag && inputString.length() == 1) {
    Serial.print(F("Input capture: Pressed RESET. "));
    Serial.println(F("Resetting all relays."));
    captureStatus  = "   Pressed RESET.   ";
    captureStatus += "     Resetting      ";
    captureStatus += "     all relays.    ";
    captureStatus += "                    ";

    return true;
  }

  else if (_Reset_flag && inputString.length() > 1) {
    Serial.print(F("Input capture: Pressed Cancel."));
    Serial.println(F("Nothing to change."));
    captureStatus  = "   Pressed Cancel.  ";
    captureStatus += "  Resetting input   ";
    captureStatus += "     sequence.      ";
    captureStatus += "                    ";
  }

  else if (inputString.length() > 11) {
    Serial.println(F("Input capture: Too many inputs."));
    captureStatus  = "                    ";
    captureStatus += "  Too many inputs.  ";
    captureStatus += "                    ";
    captureStatus += "                    ";

    return false;
  }

  // Do not allow inputs of 17, 18, 19 (0-16 is allowed).
  // Do not allow inputs greater than 19.
  // Two digit inputs must have value greater than 9, i.e. '1' vs '01'.
  else if (_numbers_outof_range == true) {
    // Error message already set and printed to monitor.
    return false;
  }

  // First input must be a letter or *.
  else if (inputString[0] != 'A' && inputString[0] != 'B' &&
           inputString[0] != 'C' && inputString[0] != 'D' &&
           inputString[0] != '*') {
    Serial.println(F("Input capture: First type A,B,C,D or *"));
    captureStatus  = "                    ";
    captureStatus += "     First type     ";
    captureStatus += "    A,B,C,D or *.   ";
    captureStatus += "                    ";

    return false;
  }

  // Must follow: A before B, before C, before D
  else if ( ((_A_pos > _B_pos) && _B_count > 0) ||
            ((_A_pos > _C_pos) && _C_count > 0) ||
            ((_A_pos > _D_pos) && _D_count > 0) ||
            ((_B_pos > _C_pos) && _C_count > 0) ||
            ((_B_pos > _D_pos) && _D_count > 0) ||
            ((_C_pos > _D_pos) && _D_count > 0) ) {
    Serial.println(F("Input capture: ABCD must be in order."));
    captureStatus  = "                    ";
    captureStatus += "    ABCD must be    ";
    captureStatus += "     in order.      ";
    captureStatus += "                    ";

    return false;
  }

  // Must have a single keypress of A,B,C, or D
  else if ( _A_count > 1 || _B_count > 1 ||
            _C_count > 1 || _D_count > 1 ) {
    Serial.println(F("Input capture: Type A, B, C, or D only once."));
    captureStatus  = "                    ";
    captureStatus += "    Type A, B, C,   ";
    captureStatus += "   or D only once.  ";
    captureStatus += "                    ";

    return false;
  }

  else {
    Serial.println(F("Input capture: Input is validated."));
    captureStatus  = "      Input is      ";
    captureStatus += "     validated.     ";
    captureStatus += "     Activating     ";
    captureStatus += "       output.      ";

    return true;
  }
}

void InputCapture::
check_numbers_range(int num) {
  // Do not allow inputs of 17, 18, 19 (0-16 is allowed).
  if (num == 1 && (_counter != (inputString.length() - 1)) &&
     (inputString[_counter + 1] == '7' ||
      inputString[_counter + 1] == '8' ||
      inputString[_counter + 1] == '9')) {
    Serial.println(F("Input capture: Inputs must be in range 0-16."));
    captureStatus  = "                    ";
    captureStatus += "   Inputs must be   ";
    captureStatus += "   in range 0-16.   ";
    captureStatus += "                    ";
    _numbers_outof_range = true;
  }

  // Do not allow inputs greater than 19.
  else if ((_counter != (inputString.length() - 1))
         && num != 0 && num != 1 &&
           (inputString[_counter + 1] != 'B' &&
            inputString[_counter + 1] != 'C' &&
            inputString[_counter + 1] != 'D')) {
    Serial.println(F("Input capture: Inputs must be in range 0-16."));
    captureStatus  = "                    ";
    captureStatus += "   Inputs must be   ";
    captureStatus += "   in range 0-16.   ";
    captureStatus += "                    ";
    _numbers_outof_range = true;
  }

  // Do not allow leading zeros, i.e. '1' vs '01'.
  else if ((_counter != (inputString.length() - 1)) && num == 0  &&
           (inputString[_counter + 1] != 'B' ||
            inputString[_counter + 1] != 'C' ||
            inputString[_counter + 1] != 'D')) {
    Serial.print(F("Input capture: Incorrect format. "));
    Serial.println(F("Use '6' instead of '06'."));
    captureStatus  = " Incorrect format.  ";
    captureStatus += "  Use '6' instead   ";
    captureStatus += "      of '06'.      ";
    captureStatus += "                    ";
    _numbers_outof_range = true;
  }

  else {}
}
