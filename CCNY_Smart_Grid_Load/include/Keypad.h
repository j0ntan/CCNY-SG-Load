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
  enum class ButtonID {
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
  virtual Keypad::ButtonID getButtonID() const = 0;

 protected:
  const milliseconds HOLD_THRESHOLD = 500;
  using ID = Keypad::ButtonID;
  const Keypad::ButtonID mapping[Keypad::MAX_ROWS][Keypad::MAX_COLS] = {
      {ID::NUM1, ID::NUM2, ID::NUM3, ID::A},
      {ID::NUM4, ID::NUM5, ID::NUM6, ID::B},
      {ID::NUM7, ID::NUM8, ID::NUM9, ID::C},
      {ID::STAR, ID::NUM0, ID::HASH, ID::D}};
};

#endif  // KEYPAD_H
