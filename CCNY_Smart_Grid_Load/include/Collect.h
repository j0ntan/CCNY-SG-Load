#ifndef COLLECT_H
#define COLLECT_H

#include "ArduinoInterface.h"
#include "Timer.h"
#include "Keypad.h"
#include "XBee.h"

#define MAX_INPUT_LENGTH 11  // largest input is 'A16B16C16D2', currently
#define DSPACE_SINGLE_INPUT_BOUND 28

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

int readNextUniqueByte();

void removeBufferedByteCopies(const int& original);

template <class StringT>
void followDPSACEManualCommand(const int& commandID, StringT& input);

template <class StringT>
void dSPACESingleInputCommand(const int& commandID, StringT& input);

template <class StringT>
void dSPACEBalancedInputCommand(const int& commandID, StringT& input);
}  // namespace helper

extern Arduino* arduino;

//**************************** KEYPAD collection *****************************//
extern Keypad* keypad;
extern Timer* timer;

template <class StringT>
StringT recordKeypadSequence() {
  StringT input;
  input.reserve(MAX_INPUT_LENGTH);
  bool sequence_terminated = false;

  do {
    const Keypad::Button pressed = keypad->getButton();

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

  timer->delay(300);  // guard time (ms) between consecutive presses
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
    timer->delay(5);  // guard time, allow incoming data to fill buffer
  }
}

//************************* dSPACE manual collection *************************//
template <class StringT>
StringT collectDSPACEManualData() {
  StringT input;
  input.reserve(MAX_INPUT_LENGTH);

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

template <class StringT>
void helper::followDPSACEManualCommand(const int& commandID, StringT& input) {
  if (commandID <= DSPACE_SINGLE_INPUT_BOUND)
    helper::dSPACESingleInputCommand(commandID, input);
  else
    helper::dSPACEBalancedInputCommand(commandID, input);
}

template <class StringT>
void helper::dSPACESingleInputCommand(const int& commandID, StringT& input) {
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
      input += itoa(commandID, str, 10);
      break;
    case 17:
      input += 'A';
      break;
    case 18:
      input += 'B';
      break;
    case 19:
      input += 'C';
      break;
    case 20:
      input += 'D';
      break;
    case 21:  // CANCEL
      helper::cancelSequence(input);
      break;
    case 22:  // BACKSPACE
      helper::eraseLastInSequence(input);
      break;
    case 23:  // RESET
      helper::applyResetSequence(input);
      break;
    default:
      input = "?";
  }  // end switch
}

template <class StringT>
void helper::dSPACEBalancedInputCommand(const int& commandID, StringT& input) {
  const uint8_t STR_SIZE = 3;
  char str[STR_SIZE] = {0};
  const int RELAYS_ON = commandID - (DSPACE_SINGLE_INPUT_BOUND + 1);

  helper::prependACphases(input);
  input += itoa(RELAYS_ON, str, 10);
}

//************************* dSPACE Load Profile Mode *************************//
unsigned int readProfileNumberFromSerial() {
  static const unsigned int PROFILE_MODE_OFFSET = 46;
  int profile_number = helper::readNextUniqueByte();
  emptyTheBuffer();
  return profile_number - PROFILE_MODE_OFFSET + 1;
}

template <class StringT>
StringT createFilename(const unsigned int& profile_number) {
  const unsigned int HUNDREDTHS_DIGIT = profile_number / 100;
  const unsigned int TENS_DIGIT = (profile_number / 10) % 10;
  const unsigned int ONES_DIGIT = profile_number % 10;
  return StringT(F("PRFL")) + StringT(HUNDREDTHS_DIGIT) + StringT(TENS_DIGIT) +
         StringT(ONES_DIGIT) + StringT(F(".txt"));
}

template <class StringT>
bool lineIsComment(const StringT& line) {
  return line.length() >= 2 && line[0] == '/' && line[1] == '/';
}

template <class StringT>
StringT extractProfileInput(const StringT& profileStr) {
  static const unsigned int MAX_LENGTH = 11;
  StringT input_sequence;
  input_sequence.reserve(MAX_LENGTH);

  int phase_begins = 0;
  int phase_ends = profileStr.indexOf(' ', phase_begins);
  StringT A_phase =
      StringT(F("A")) + profileStr.substring(phase_begins, phase_ends);

  phase_begins = phase_ends + 1;
  phase_ends = profileStr.indexOf(' ', phase_begins);
  StringT B_phase =
      StringT(F("B")) + profileStr.substring(phase_begins, phase_ends);

  phase_begins = phase_ends + 1;
  phase_ends = profileStr.indexOf(' ', phase_begins);
  StringT C_phase =
      StringT(F("C")) + profileStr.substring(phase_begins, phase_ends);

  return A_phase + B_phase + C_phase;
}

template <class StringT>
unsigned long extractProfileDuration(const StringT& line) {
  int position = line.lastIndexOf(' ') + 1;
  StringT duration = line.substring(position);
  return static_cast<unsigned long>(duration.toInt());
}

#endif  // COLLECT_H
