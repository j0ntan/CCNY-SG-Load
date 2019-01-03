#include "../include/HardwareKeypad.h"
#include "../include/Timer.h"
#include "../include/DigitalIO.h"

extern Timer* timer;

HardwareKeypad::HardwareKeypad(const DigitalInput* input_pins[],
                               const DigitalOutput* output_pins[])
    : ROWS{input_pins}, COLS{output_pins} {}

bool HardwareKeypad::anyButtonPressed() const {
  // Internal pull-ups on row pins short to ground on a button press.

  return !ROWS[0]->read() || !ROWS[1]->read() || !ROWS[2]->read() ||
         !ROWS[3]->read();
}

bool HardwareKeypad::anyButtonHeld() const {
  // Check if a button is held longer than threshold value.

  Keypad::milliseconds press_duration = 0;
  while (anyButtonPressed() && press_duration < Keypad::HOLD_THRESHOLD) {
    timer->delay(10);
    press_duration += 10;
  }

  return press_duration >= Keypad::HOLD_THRESHOLD;
}

Keypad::ButtonID HardwareKeypad::getButtonID() const {
  // Returns the Keypad::ButtonID corresponding to what is being pressed.

  _Buttons_pressed pressed = _checkAllButtonsPressed();

  return pressed.count > 1 ? Keypad::ButtonID::MULTIPLE
                           : _buttonConvertRowAndCol(pressed.row, pressed.col);
}

bool HardwareKeypad::_rowIsPressed(number row_N) const {
  return !ROWS[row_N - 1]->read();
}

bool HardwareKeypad::_colIsPressed(number col_N, number row_N) const {
  // If a button is pressed its col & row pins are shorted, thus toggling one
  // affects the other.

  bool row_was_toggled = false;

  COLS[col_N - 1]->set();
  if (ROWS[row_N - 1]->read()) row_was_toggled = true;
  COLS[col_N - 1]->clear();

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
  for (number row_N = 1; row_N <= Keypad::MAX_ROWS; row_N++)
    for (number col_N = 1; col_N <= Keypad::MAX_COLS; col_N++)
      if (_thisButtonPressed(row_N, col_N)) {
        result.row = row_N;
        result.col = col_N;
        result.count++;
      }

  return result;
}

Keypad::ButtonID HardwareKeypad::_buttonConvertRowAndCol(number row_N,
                                                         number col_N) const {
  number button_ID = (col_N - 1) + 4 * (row_N - 1);
  switch (button_ID) {
    case 0:
      return Keypad::ButtonID::NUM1;
    case 1:
      return Keypad::ButtonID::NUM2;
    case 2:
      return Keypad::ButtonID::NUM3;
    case 3:
      return Keypad::ButtonID::A;
    case 4:
      return Keypad::ButtonID::NUM4;
    case 5:
      return Keypad::ButtonID::NUM5;
    case 6:
      return Keypad::ButtonID::NUM6;
    case 7:
      return Keypad::ButtonID::B;
    case 8:
      return Keypad::ButtonID::NUM7;
    case 9:
      return Keypad::ButtonID::NUM8;
    case 10:
      return Keypad::ButtonID::NUM9;
    case 11:
      return Keypad::ButtonID::C;
    case 12:
      return Keypad::ButtonID::STAR;
    case 13:
      return Keypad::ButtonID::NUM0;
    case 14:
      return Keypad::ButtonID::HASH;
    case 15:
      return Keypad::ButtonID::D;
    default:
      return Keypad::ButtonID::MULTIPLE;
  }
}
