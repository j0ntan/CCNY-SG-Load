#ifndef COLLECT_H
#define COLLECT_H

#include "ArduinoInterface.h"
#include "Keypad.h"
#include "XBee.h"

#define MAX_INPUT_LENGTH 11  // largest input is 'A16B16C16D2', currently

namespace helper {
template <class StringT>
void actionOnButtonHold(StringT& input, const Keypad::Button& pressed);

template <class StringT>
void eraseLastInSequence(StringT& input);

template <class StringT>
void addCharToSequence(StringT& input, const Keypad::Button& pressed);

void waitForButtonRelease();

template <class StringT>
void cancelSequence(StringT& input);

template <class StringT>
void applyResetSequence(StringT& input);

template <class StringT>
void prependACphases(StringT& input);

template <class StringT>
void readSerialIntoString(StringT& input);
}  // namespace helper

extern Arduino* arduino;

//**************************** KEYPAD collection *****************************//
extern Keypad* keypad;

template <class StringT>
StringT recordKeypadSequence() {
  StringT input;
  input.reserve(MAX_INPUT_LENGTH);
  bool sequence_terminated = false;
  Keypad::Button pressed;

  do {
    pressed = keypad->getButton();

    if (pressed == Keypad::Button::MULTIPLE) {
      // ignore multiple button presses
    } else if (keypad->anyButtonHeld()) {
      helper::actionOnButtonHold(input, pressed);
      sequence_terminated = true;
    } else if (pressed == Keypad::Button::HASH) {
      sequence_terminated = true;
    } else if (pressed == Keypad::Button::STAR) {
      helper::eraseLastInSequence(input);
    } else {
      helper::addCharToSequence(input, pressed);
    }

    helper::waitForButtonRelease();
  } while (!sequence_terminated);

  return input;
}

template <class StringT>
void helper::actionOnButtonHold(StringT& input, const Keypad::Button& pressed) {
  switch (pressed) {
    case Keypad::Button::HASH:
    case Keypad::Button::A:
    case Keypad::Button::B:
    case Keypad::Button::C:
    case Keypad::Button::D:
      break;  // do nothing
    case Keypad::Button::STAR:
      if (input.length() > 0)
        helper::cancelSequence(input);
      else
        helper::applyResetSequence(input);
      break;
    default:
      helper::prependACphases(input);
      helper::addCharToSequence(input, pressed);
  }
}

template <class StringT>
void helper::eraseLastInSequence(StringT& input) {
  if (input.length() > 0) input.remove(input.length() - 1);
  // else, nothing to erase
}

template <class StringT>
void helper::addCharToSequence(StringT& input, const Keypad::Button& pressed) {
  switch (pressed) {
    case Keypad::Button::NUM0:
      input += '0';
      break;
    case Keypad::Button::NUM1:
      input += '1';
      break;
    case Keypad::Button::NUM2:
      input += '2';
      break;
    case Keypad::Button::NUM3:
      input += '3';
      break;
    case Keypad::Button::NUM4:
      input += '4';
      break;
    case Keypad::Button::NUM5:
      input += '5';
      break;
    case Keypad::Button::NUM6:
      input += '6';
      break;
    case Keypad::Button::NUM7:
      input += '7';
      break;
    case Keypad::Button::NUM8:
      input += '8';
      break;
    case Keypad::Button::NUM9:
      input += '9';
      break;
    case Keypad::Button::A:
      input += 'A';
      break;
    case Keypad::Button::B:
      input += 'B';
      break;
    case Keypad::Button::C:
      input += 'C';
      break;
    case Keypad::Button::D:
      input += 'D';
      break;
    default:
      break;  // does NOT add anything
  }
}

void helper::waitForButtonRelease() {
  while (keypad->anyButtonPressed())
    ;  // you do nothing, Jon Snow!!

  arduino->delay(300);  // guard time (ms) between consecutive presses
}

template <class StringT>
void helper::cancelSequence(StringT& input) {
  input.remove(0);
}

template <class StringT>
void helper::applyResetSequence(StringT& input) {
  input = StringT(F("ABCD0"));
}

template <class StringT>
void helper::prependACphases(StringT& input) {
  input = StringT(F("ABC")) + input;
}

//*************************** PC Serial collection ***************************//
extern XBee* xbee;
template <class StringT>
StringT collectPCSerialData() {
  StringT input;
  input.reserve(MAX_INPUT_LENGTH);

  helper::readSerialIntoString(input);

  return input;
}

template <class StringT>
void helper::readSerialIntoString(StringT& input) {
  while (xbee->hasBufferedData()) input += static_cast<char>(xbee->readByte());
}

void emptyTheBuffer() {
  while (xbee->hasBufferedData()) {
    xbee->readByte();
    arduino->delay(5);  // guard time, allow incoming data to fill buffer
  }
}

#endif  // COLLECT_H
