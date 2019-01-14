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
