#include "../include/HardwareKeypad.h"

HardwareKeypad::HardwareKeypad(pin row1, pin row2, pin row3, pin row4, pin col1,
                               pin col2, pin col3, pin col4)
    : _ROWS{row1, row2, row3, row4}, _COLS{col1, col2, col3, col4} {
  _initializeKeypadPins();
}

bool HardwareKeypad::anyButtonPressed() const {
  // Internal pull-ups on row pins short to ground on a button press.

  return digitalRead(_ROWS[0]) == 0 || digitalRead(_ROWS[1]) == 0 ||
         digitalRead(_ROWS[2]) == 0 || digitalRead(_ROWS[3]) == 0;
}

bool HardwareKeypad::anyButtonHeld() const {
  // Check if a button is held longer than threshold value.

  milliseconds press_duration = 0;
  while (anyButtonPressed() && press_duration < _HOLD_THRESHOLD) {
    delay(10);
    press_duration += 10;
  }

  return press_duration >= _HOLD_THRESHOLD;
}

Keypad::Button HardwareKeypad::getButton() const {
  // Returns the Keypad::Button corresponding to what is being pressed.

  _Buttons_pressed pressed = _checkAllButtonsPressed();

  return pressed.count > 1 ? Keypad::Button::MULTIPLE
                           : _buttonConvertRowAndCol(pressed.row, pressed.col);
}

void HardwareKeypad::_initializeKeypadPins() const {
  for (number n = 0; n < _MAX_COLS; n++) {
    pinMode(_ROWS[n], INPUT_PULLUP);
    pinMode(_COLS[n], OUTPUT);
    digitalWrite(_COLS[n], LOW);
  }
}

bool HardwareKeypad::_rowIsPressed(number row_N) const {
  return digitalRead(_ROWS[row_N - 1]) == 0;
}

bool HardwareKeypad::_colIsPressed(number col_N, number row_N) const {
  // If a button is pressed its col & row pins are shorted, thus toggling one
  // affects the other.

  bool row_was_toggled = false;

  digitalWrite(_COLS[col_N - 1], HIGH);
  if (digitalRead(_ROWS[row_N - 1])) row_was_toggled = true;
  digitalWrite(_COLS[col_N - 1], LOW);

  return row_was_toggled;
}

bool HardwareKeypad::_thisButtonPressed(number row_N, number col_N) const {
  return _rowIsPressed(row_N) && _colIsPressed(col_N, row_N);
}

HardwareKeypad::_Buttons_pressed HardwareKeypad::_checkAllButtonsPressed()
    const {
  // Check each button to see which are currently being pressed. Returns a count
  // of buttons pressed and the row/col of the last checked & pressed button.

  _Buttons_pressed result;
  for (number row_N = 1; row_N <= _MAX_ROWS; row_N++)
    for (number col_N = 1; col_N <= _MAX_COLS; col_N++)
      if (_thisButtonPressed(row_N, col_N)) {
        result.row = row_N;
        result.col = col_N;
        result.count++;
      }

  return result;
}

Keypad::Button HardwareKeypad::_buttonConvertRowAndCol(number row_N,
                                                       number col_N) const {
  number button_ID = (col_N - 1) + 4 * (row_N - 1);
  switch (button_ID) {
    case 0:
      return Keypad::Button::NUM1;
    case 1:
      return Keypad::Button::NUM2;
    case 2:
      return Keypad::Button::NUM3;
    case 3:
      return Keypad::Button::A;
    case 4:
      return Keypad::Button::NUM4;
    case 5:
      return Keypad::Button::NUM5;
    case 6:
      return Keypad::Button::NUM6;
    case 7:
      return Keypad::Button::B;
    case 8:
      return Keypad::Button::NUM7;
    case 9:
      return Keypad::Button::NUM8;
    case 10:
      return Keypad::Button::NUM9;
    case 11:
      return Keypad::Button::C;
    case 12:
      return Keypad::Button::STAR;
    case 13:
      return Keypad::Button::NUM0;
    case 14:
      return Keypad::Button::HASH;
    case 15:
      return Keypad::Button::D;
    default:
      return Keypad::Button::MULTIPLE;
  }
}
