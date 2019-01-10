#ifndef HARDWAREKEYPAD_H
#define HARDWAREKEYPAD_H

#include "Keypad.h"

class DigitalInput;
class DigitalOutput;

class HardwareKeypad : public Keypad {
 public:
  typedef uint8_t number;

  HardwareKeypad(const DigitalInput* input_pins[],
                 const DigitalOutput* output_pins[]);
  ~HardwareKeypad() final = default;

  HardwareKeypad() = delete;
  HardwareKeypad(const HardwareKeypad&) = delete;
  HardwareKeypad& operator=(const HardwareKeypad&) = delete;

  // check keypad buttons status
  bool anyButtonPressed() const final;
  bool anyButtonHeld() const final;

  // retrieve pressed key
  Keypad::ButtonID getButtonID() const final;

 private:
  struct _Buttons_pressed {
    number row = 0;
    number col = 0;
    number count = 0;
  };

  void _pollEachButton(uint8_t& last_pressed_row, uint8_t& last_pressed_col,
                       uint8_t& pressed_count) const;
  bool _rowIsPressed(number row) const;
  bool _colIsPressed(number col, number row) const;
  bool _thisButtonPressed(number row, number col) const;
  _Buttons_pressed _checkAllButtonsPressed() const;

  const DigitalInput** ROWS;
  const DigitalOutput** COLS;
};

#endif  // HARDWAREKEYPAD_H
