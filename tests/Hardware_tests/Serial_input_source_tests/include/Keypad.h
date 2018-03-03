#ifndef KEYPAD_H
#define KEYPAD_H

class Keypad {
 public:
  virtual ~Keypad() = default;

  virtual bool anyButtonPressed() const = 0;
  virtual bool anyButtonHeld() const = 0;

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
};

#endif  // KEYPAD_H
