#ifndef HARDWAREKEYPAD_H
#define HARDWAREKEYPAD_H

#include "Keypad.h"

class DigitalInput;
class DigitalOutput;

class HardwareKeypad : public Keypad {
 public:
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
  void _pollEachButton(uint8_t& last_pressed_row, uint8_t& last_pressed_col,
                       uint8_t& pressed_count) const;

  const DigitalInput** ROWS;
  const DigitalOutput** COLS;
};

#endif  // HARDWAREKEYPAD_H
