#ifndef COLLECT_H
#define COLLECT_H

#include <stdlib.h>
#include <ctype.h>
#include "Timer.h"
#include "Keypad.h"
#include "XBee.h"
#include "InputSequence.h"

// Globals defined in main application
extern Timer* timer;
extern Keypad* keypad;
extern XBee* xbee;

#define DSPACE_SINGLE_INPUT_BOUND 28

namespace helper {
void actionOnButtonHold(InputSequence& input, const Keypad::ButtonID& pressed);

char ButtonIDToChar(const Keypad::ButtonID& pressed);

void waitForButtonRelease();

void readSerialIntoString(InputSequence& input);

int readNextUniqueByte();

void removeBufferedByteCopies(const int& original);

void followDPSACEManualCommand(const int& commandID, InputSequence& input);

void dSPACESingleInputCommand(const int& commandID, InputSequence& input);

void dSPACEBalancedInputCommand(const int& commandID, InputSequence& input);
}  // namespace helper

//**************************** KEYPAD collection *****************************//
InputSequence recordKeypadSequence() {
  InputSequence keypad_sequence;
  bool sequence_terminated = false;

  do {
    const Keypad::ButtonID pressed = keypad->getButtonID();

    if (pressed == Keypad::ButtonID::MULTIPLE) {
      // ignore multiple button presses
    } else if (keypad->anyButtonHeld()) {
      helper::actionOnButtonHold(keypad_sequence, pressed);
      sequence_terminated = true;
    } else if (pressed == Keypad::ButtonID::HASH) {
      sequence_terminated = true;
    } else if (pressed == Keypad::ButtonID::STAR) {
      keypad_sequence.removeLastInput();
    } else {
      keypad_sequence.addInput(helper::ButtonIDToChar(pressed));
    }

    helper::waitForButtonRelease();
  } while (!sequence_terminated);

  return keypad_sequence;
}

void helper::actionOnButtonHold(InputSequence& input,
                                const Keypad::ButtonID& pressed) {
  switch (pressed) {
    case Keypad::ButtonID::HASH:
    case Keypad::ButtonID::A:
    case Keypad::ButtonID::B:
    case Keypad::ButtonID::C:
    case Keypad::ButtonID::D:
      break;  // do nothing
    case Keypad::ButtonID::STAR:
      if (input.length() > 0)
        input.cancelSequence();
      else
        input.applyResetSequence();
      break;
    default:
      uint8_t amount =
          static_cast<uint8_t>(helper::ButtonIDToChar(pressed) - '0');
      if (input.length() == 1 && isdigit(input[0])) {
        const uint8_t tens_digit = static_cast<uint8_t>(input[0] - '0');
        amount += 10 * tens_digit;
      }
      input.applyBalancedSequence(amount);
  }
}

char helper::ButtonIDToChar(const Keypad::ButtonID& pressed) {
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

void helper::waitForButtonRelease() {
  while (keypad->anyButtonPressed())
    ;  // you do nothing, Jon Snow!!

  timer->delay(300);  // guard time (ms) between consecutive presses
}

//*************************** PC Serial collection ***************************//
InputSequence collectPCSerialData() {
  InputSequence PC_input;

  helper::readSerialIntoString(PC_input);

  return PC_input;
}

void helper::readSerialIntoString(InputSequence& input) {
  while (xbee->hasBufferedData())
    input.addInput(static_cast<char>(xbee->readByte()));
}

void emptyTheBuffer() {
  while (xbee->hasBufferedData()) {
    xbee->readByte();
    timer->delay(5);  // guard time, allow incoming data to fill buffer
  }
}

//************************* dSPACE manual collection *************************//
InputSequence collectDSPACEManualData() {
  InputSequence input;

  do {
    int receivedByte = helper::readNextUniqueByte();
    helper::removeBufferedByteCopies(receivedByte);
    helper::followDPSACEManualCommand(receivedByte, input);
  } while (xbee->hasBufferedData());

  return input;
}

int helper::readNextUniqueByte() { return xbee->readByte(); }

void helper::removeBufferedByteCopies(const int& original) {
  while (original == xbee->peekByte()) {
    xbee->readByte();
    timer->delay(5);  // guard time, allow incoming data to fill buffer
  }
}

void helper::followDPSACEManualCommand(const int& commandID,
                                       InputSequence& input) {
  if (commandID <= DSPACE_SINGLE_INPUT_BOUND)
    helper::dSPACESingleInputCommand(commandID, input);
  else
    helper::dSPACEBalancedInputCommand(commandID, input);
}

void helper::dSPACESingleInputCommand(const int& commandID,
                                      InputSequence& input) {
  const uint8_t STR_SIZE = 3;  // two digits, plus null char
  char str[STR_SIZE] = {0};
  switch (commandID) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
      input.addInput(itoa(commandID, str, 10));
      break;
    case 17:
      input.addInput('A');
      break;
    case 18:
      input.addInput('B');
      break;
    case 19:
      input.addInput('C');
      break;
    case 20:
      input.addInput('D');
      break;
    case 21:  // CANCEL
      input.cancelSequence();
      break;
    case 22:  // BACKSPACE
      input.removeLastInput();
      break;
    case 23:  // RESET
      input.applyResetSequence();
      break;
    default:
      input.addInput('?');
  }  // end switch
}

void helper::dSPACEBalancedInputCommand(const int& commandID,
                                        InputSequence& input) {
  const uint8_t AMOUNT = commandID - (DSPACE_SINGLE_INPUT_BOUND + 1);
  input.applyBalancedSequence(AMOUNT);
}

//************************* dSPACE Load Profile Mode *************************//
unsigned int readProfileNumberFromSerial() {
  static const unsigned int PROFILE_MODE_OFFSET = 46;
  int profile_number = helper::readNextUniqueByte();
  emptyTheBuffer();
  return profile_number - PROFILE_MODE_OFFSET + 1;
}

void createFilename(const unsigned int& profile_number, char* buffer) {
  const char HUNDREDTHS_DIGIT = static_cast<char>('0' + profile_number / 100);
  const char TENS_DIGIT = static_cast<char>('0' + (profile_number / 10) % 10);
  const char ONES_DIGIT = static_cast<char>('0' + profile_number % 10);
  buffer[4] = HUNDREDTHS_DIGIT;
  buffer[5] = TENS_DIGIT;
  buffer[6] = ONES_DIGIT;
}

#endif  // COLLECT_H
