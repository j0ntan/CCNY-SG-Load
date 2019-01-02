#ifndef HARDWAREKEYPAD_H
#define HARDWAREKEYPAD_H

#include "Arduino.h"
#include "Keypad.h"

class HardwareKeypad : public Keypad {
 public:
  typedef uint8_t pin;
  typedef uint8_t number;

  HardwareKeypad(pin Row1, pin Row2, pin Row3, pin Row4, pin Col1, pin Col2,
                 pin Col3, pin Col4);
  ~HardwareKeypad() final = default;

  HardwareKeypad() = delete;
  HardwareKeypad(const HardwareKeypad&) = delete;
  HardwareKeypad& operator=(const HardwareKeypad&) = delete;

  // check keypad buttons status
  bool anyButtonPressed() const final;
  bool anyButtonHeld() const final;

  // retrieve pressed key
  Keypad::Button getButton() const final;

 private:
  struct _Buttons_pressed {
    number row = 0;
    number col = 0;
    number count = 0;
  };

  void _initializeKeypadPins() const;
  bool _rowIsPressed(number row) const;
  bool _colIsPressed(number col, number row) const;
  bool _thisButtonPressed(number row, number col) const;
  _Buttons_pressed _checkAllButtonsPressed() const;
  Keypad::Button _buttonConvertRowAndCol(number row, number col) const;

  const pin _ROWS[Keypad::MAX_ROWS];
  const pin _COLS[Keypad::MAX_COLS];
};

#endif  // HARDWAREKEYPAD_H
