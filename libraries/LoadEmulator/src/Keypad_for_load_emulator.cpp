#include "Keypad_for_load_emulator.h"

#include "Arduino.h"

Keypad::
Keypad() {
  /*
  Assign default values of Ardiuno row & column pins to private class
  variables. */
  _row1 = ROW1_PIN;
  _row2 = ROW2_PIN;
  _row3 = ROW3_PIN;
  _row4 = ROW4_PIN;
  _col1 = COL1_PIN;
  _col2 = COL2_PIN;
  _col3 = COL3_PIN;
  _col4 = COL4_PIN;
}

void Keypad::
begin() {
  /*
  Assign input pullup pins on Arduino for keypad row pins. */
  pinMode(_row1, INPUT_PULLUP);
  pinMode(_row2, INPUT_PULLUP);
  pinMode(_row3, INPUT_PULLUP);
  pinMode(_row4, INPUT_PULLUP);

  /*
  Assign output pins on Arduino for keypad column pins & set them
  to a LOW initial/idle state. */
  pinMode(_col1, OUTPUT);
  pinMode(_col2, OUTPUT);
  pinMode(_col3, OUTPUT);
  pinMode(_col4, OUTPUT);
  digitalWrite(_col1, 0);
  digitalWrite(_col2, 0);
  digitalWrite(_col3, 0);
  digitalWrite(_col4, 0);

  /*
  Stores an array of chars in the order: [0,1,...,9,A,B,C,D,*,#]. These
  are the characters that are printed on the physical keypad. */
  for (int i = 0; i < 10; i++)
    _printed_keys[i] = char(i + int('0'));
  for (int i = 0; i < 4; i++)
    _printed_keys[i + 10] = char(i + int('A'));
  _printed_keys[14] = '*';
  _printed_keys[15] = '#';

  Serial.println(F("Keypad: Has begun."));
}

void Keypad::
begin(int R1, int R2, int R3, int R4,
      int C1, int C2, int C3, int C4) {
  /*
  Assigns non-default values to row & column pins. Useful when testing
  some code and using some convenient pins on the Arduino. */
  _row1 = R1;
  _row2 = R2;
  _row3 = R3;
  _row4 = R4;
  _col1 = C1;
  _col2 = C2;
  _col3 = C3;
  _col4 = C4;

  /*
  Assign input pullup pins on Arduino for keypad row pins. */
  pinMode(_row1, INPUT_PULLUP);
  pinMode(_row2, INPUT_PULLUP);
  pinMode(_row3, INPUT_PULLUP);
  pinMode(_row4, INPUT_PULLUP);
  
  /*
  Assign output pins on Arduino for keypad column pins & set them
  to a LOW initial/idle state. */
  pinMode(_col1, OUTPUT);
  pinMode(_col2, OUTPUT);
  pinMode(_col3, OUTPUT);
  pinMode(_col4, OUTPUT);
  digitalWrite(_col1, 0);
  digitalWrite(_col2, 0);
  digitalWrite(_col3, 0);
  digitalWrite(_col4, 0);
  
  /*
  Stores an array of chars in the order: [0,1,...,9,A,B,C,D,*,#]. These
  are the characters that are printed on the physical keypad. */
  for (int i = 0; i < 10; i++)
    _printed_keys[i] = char(i + int('0'));
  for (int i = 0; i < 4; i++)
    _printed_keys[i + 10] = char(i + int('A'));
  _printed_keys[14] = '*';
  _printed_keys[15] = '#';
  
  Serial.println(F("Keypad: Has begun."));
}

bool Keypad::
isPressed(void) {
  /*
  This function returns true when a key or multiple keys are pressed.
  Refer to report for a description of how a press is detected. */
  _key_is_pressed = !digitalRead(_row1) || !digitalRead(_row2) ||
                    !digitalRead(_row3) || !digitalRead(_row4);
  return _key_is_pressed; 
}

char Keypad::
getKey() {
  /*
  This function returns a character that corresponds to the key that was
  pressed. When multiple keys are pressed, the character '!' is
  returned. */
  bool keys_are_pressed[4][4] = { {0,0,0,0}, {0,0,0,0},
                                  {0,0,0,0}, {0,0,0,0} };  

  /*
  Keypad pins are toggled to determine how many keys were pressed. */
  toggle_keys(keys_are_pressed);
  
  if (_keypress_count > 1) {
    Serial.println(F("Multiple"));
    _pressed_key_char = '!'; 
  }  
  else {
    _pressed_key_char = find_single_key();
  }

  // Detect a button HOLD event
  heldStatus = false;
  pause_until_threshold();

  Serial.print(F("Keypad: Pressed "));
  Serial.println(_pressed_key_char);
  Serial.println();
  return _pressed_key_char;
}

void Keypad::
toggle_keys(bool (&keys_are_pressed)[4][4]) {
  /*
  This function toggles certain keypad output pins to determine which
  keys have been pressed. Refer to the report for a detailed description
  of this algorithm. */
  int rows[4] = {_row1, _row2, _row3, _row4};
  int cols[4] = {_col1, _col2, _col3, _col4};
  _keypress_count = 0;

  // Toggling occurs.
  for (int row_index = 0; row_index < 4; row_index++) {
    if (digitalRead(rows[row_index]) == 0)
      for (int col_index = 0; col_index < 4; col_index++) {
        digitalWrite(cols[col_index], HIGH);
        if (digitalRead(rows[row_index]) == true) {
          keys_are_pressed[row_index][col_index] = true;
          _row_pressed = row_index;
          _col_pressed = col_index;
          _keypress_count++;
        }
        digitalWrite(cols[col_index],  LOW);
      }
  }

  delay(SENSITIVITY);
}

void Keypad::
pause_until_threshold() {
  /*
  This function detects the event where a button has been held for a
  longer time than the threshold value. At this event, a flag is set true
  and the pressed key character is returned by the calling function. */
  _ms_held = 0;
  while (isPressed()) {
    delay(10);
    _ms_held += 10; // keeps track of time in 10 ms increments
    if (_ms_held == HOLD_THRESHOLD) {
      Serial.println(F("Keypad: Button held."));
      heldStatus = true;
      break;
    }
  }
}

char Keypad::
find_single_key() {
  /*
  This fucntion determines the character corresponding to a single keypad
  press using the row and column as coordinates and a mapping to the
  printed character on the keypad. Refer to report for a visual
  description of this mapping. */
  switch (_row_pressed*4 + _col_pressed) {
    case 0: return '1';
    case 1: return '2';
    case 2: return '3';
    case 3: return 'A';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return 'B';
    case 8: return '7';
    case 9: return '8';
    case 10: return '9';
    case 11: return 'C';
    case 12: return '*';
    case 13: return '0';
    case 14: return '#';
    case 15: return 'D';
    default: return '?';  // case should not occur
  }
}
