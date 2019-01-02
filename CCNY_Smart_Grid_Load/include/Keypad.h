#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

class Keypad {
 public:
  typedef uint16_t milliseconds;

  virtual ~Keypad() = default;

  virtual bool anyButtonPressed() const = 0;
  virtual bool anyButtonHeld() const = 0;

  static const uint8_t MAX_ROWS = 4, MAX_COLS = 4;
  enum class Button {
    NUM0,
    NUM1,
    NUM2,
    NUM3,
    NUM4,
    NUM5,
    NUM6,
    NUM7,
    NUM8,
    NUM9,
    A,
    B,
    C,
    D,
    HASH,
    STAR,
    MULTIPLE
  };
  virtual Keypad::Button getButton() const = 0;

 protected:
  const milliseconds HOLD_THRESHOLD = 500;
};

#endif  // KEYPAD_H
