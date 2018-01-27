#ifndef KEYPAD_MOCK_H
#define KEYPAD_MOCK_H

#include <gmock/gmock.h>
#include <Keypad.h>

class KeypadMock : public Keypad {
 public:
  ~KeypadMock() override = default;

  MOCK_CONST_METHOD0(anyButtonPressed, bool());
  MOCK_CONST_METHOD0(anyButtonHeld, bool());
  MOCK_CONST_METHOD0(getButton, Keypad::Button());
};

#endif  // KEYPAD_MOCK_H
