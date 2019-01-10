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

  uint8_t last_pressed_row = 0, last_pressed_col = 0, pressed_count = 0;
  _pollEachButton(last_pressed_col, last_pressed_col, pressed_count);

  if (pressed_count > 1)
    return Keypad::ButtonID::MULTIPLE;
  else
    return Keypad::mapping[last_pressed_row][last_pressed_col];
}

void HardwareKeypad::_pollEachButton(uint8_t& last_pressed_row,
                                     uint8_t& last_pressed_col,
                                     uint8_t& pressed_count) const {
  for (uint8_t row = 0; row < Keypad::MAX_ROWS; ++row) {
    for (uint8_t col = 0; col < Keypad::MAX_COLS; ++col) {
      const bool first_reading = ROWS[row]->read();
      COLS[col]->set();
      const bool button_pressed = ROWS[row]->read() != first_reading;
      COLS[col]->clear();

      if (button_pressed) {
        last_pressed_row = row;
        last_pressed_col = col;
        ++pressed_count;
      }
    }
  }
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
