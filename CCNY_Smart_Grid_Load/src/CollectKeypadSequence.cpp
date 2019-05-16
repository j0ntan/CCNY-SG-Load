#include "../include/Keypad.h"
#include "../include/InputSequence.h"
#include "../include/Timer.h"
#include "../include/Display/LCD.h"
#include "../include/Display/MessagesList.h"
#include <ctype.h>

// Globals defined in main application
extern Keypad* keypad;
extern Timer* timer;
extern Display::LCD* lcd;

namespace {
char ButtonIDToChar(const Keypad::ButtonID& pressed) {
  switch (pressed) {
    case Keypad::ButtonID::NUM0:
      return '0';
    case Keypad::ButtonID::NUM1:
      return '1';
    case Keypad::ButtonID::NUM2:
      return '2';
    case Keypad::ButtonID::NUM3:
      return '3';
    case Keypad::ButtonID::NUM4:
      return '4';
    case Keypad::ButtonID::NUM5:
      return '5';
    case Keypad::ButtonID::NUM6:
      return '6';
    case Keypad::ButtonID::NUM7:
      return '7';
    case Keypad::ButtonID::NUM8:
      return '8';
    case Keypad::ButtonID::NUM9:
      return '9';
    case Keypad::ButtonID::A:
      return 'A';
    case Keypad::ButtonID::B:
      return 'B';
    case Keypad::ButtonID::C:
      return 'C';
    case Keypad::ButtonID::D:
      return 'D';
    default:
      return '?';
  }
}

void actionOnButtonHold(InputSequence& input, const Keypad::ButtonID& pressed) {
  switch (pressed) {
    case Keypad::ButtonID::HASH:
    case Keypad::ButtonID::A:
    case Keypad::ButtonID::B:
    case Keypad::ButtonID::C:
    case Keypad::ButtonID::D:
      break;  // do nothing
    case Keypad::ButtonID::STAR:
      if (input.length() > 0) {
        input.cancelSequence();
        lcd->printMsg(Display::cancel_sequence);
      } else {
        input.applyResetSequence();
        lcd->printMsg(Display::load_reset);
      }
      break;
    default:
      uint8_t amount = static_cast<uint8_t>(ButtonIDToChar(pressed) - '0');
      if (input.length() == 1 && isdigit(input[0])) {
        const uint8_t tens_digit = static_cast<uint8_t>(input[0] - '0');
        amount += 10 * tens_digit;
      }
      input.applyBalancedSequence(amount);
  }
}

void waitForButtonRelease() {
  while (keypad->anyButtonPressed())
    ;  // you do nothing, Jon Snow!!

  timer->delay(300);  // guard time (ms) between consecutive presses
}
}  // namespace

InputSequence collectKeypadSequence() {
  InputSequence keypad_sequence;
  bool sequence_terminated = false;

  do {
    const Keypad::ButtonID pressed = keypad->getButtonID();

    if (pressed == Keypad::ButtonID::MULTIPLE) {
      // ignore multiple button presses
    } else if (keypad->anyButtonHeld()) {
      actionOnButtonHold(keypad_sequence, pressed);
      sequence_terminated = true;
    } else if (pressed == Keypad::ButtonID::HASH) {
      sequence_terminated = true;
    } else if (pressed == Keypad::ButtonID::STAR) {
      keypad_sequence.removeLastInput();
      lcd->printMsg(Display::keypad_sequence);
      lcd->printInput(keypad_sequence, 2);
    } else {
      keypad_sequence.addInput(ButtonIDToChar(pressed));
      lcd->printMsg(Display::keypad_sequence);
      lcd->printInput(keypad_sequence, 2);
    }

    waitForButtonRelease();
  } while (!sequence_terminated);

  return keypad_sequence;
}
