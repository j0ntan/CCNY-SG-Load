#include "../include/InputSequence.h"
#include "../include/XBee.h"
#include "../include/Timer.h"
#include "../include/Display/LCD.h"
#include "../include/Display/MessagesList.h"
#include <stdint.h>
#include <stdio.h>

// Globals defined in main application
extern XBee* xbee;
extern Timer* timer;
extern Display::LCD* lcd;

namespace {
const uint8_t DSPACE_SINGLE_INPUT_BOUND = 28;

int readNextUniqueByte() { return xbee->readByte(); }

void removeBufferedByteCopies(const int& original) {
  while (original == xbee->peekByte()) {
    xbee->readByte();
    timer->delay(5);  // guard time, allow incoming data to fill buffer
  }
}

void dSPACESingleInputCommand(const int& commandID, InputSequence& input) {
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
      snprintf(str, STR_SIZE, "%d", commandID);
      input.addInput(str);
      lcd->printMsg(Display::dspace_sequence);
      lcd->printInput(input, 2);
      break;
    case 17:
      input.addInput('A');
      lcd->printMsg(Display::dspace_sequence);
      lcd->printInput(input, 2);
      break;
    case 18:
      input.addInput('B');
      lcd->printMsg(Display::dspace_sequence);
      lcd->printInput(input, 2);
      break;
    case 19:
      input.addInput('C');
      lcd->printMsg(Display::dspace_sequence);
      lcd->printInput(input, 2);
      break;
    case 20:
      input.addInput('D');
      lcd->printMsg(Display::dspace_sequence);
      lcd->printInput(input, 2);
      break;
    case 21:  // CANCEL
      input.cancelSequence();
      lcd->printMsg(Display::cancel_sequence);
      break;
    case 22:  // BACKSPACE
      input.removeLastInput();
      lcd->printMsg(Display::dspace_sequence);
      lcd->printInput(input, 2);
      break;
    case 23:  // RESET
      input.applyResetSequence();
      lcd->printMsg(Display::load_reset);
      break;
    default:
      input.addInput('?');
      lcd->printMsg(Display::dspace_sequence);
      lcd->printInput(input, 2);
  }  // end switch
}

void dSPACEBalancedInputCommand(const int& commandID, InputSequence& input) {
  const uint8_t AMOUNT = commandID - (DSPACE_SINGLE_INPUT_BOUND + 1);
  input.applyBalancedSequence(AMOUNT);
}

void followDPSACEManualCommand(const int& commandID, InputSequence& input) {
  if (commandID <= DSPACE_SINGLE_INPUT_BOUND)
    dSPACESingleInputCommand(commandID, input);
  else
    dSPACEBalancedInputCommand(commandID, input);
}
}  // namespace

InputSequence collectDSPACESequence() {
  InputSequence dSPACE_sequence;
  do {
    int receivedByte = readNextUniqueByte();
    removeBufferedByteCopies(receivedByte);
    followDPSACEManualCommand(receivedByte, dSPACE_sequence);
  } while (xbee->hasBufferedData());
  return dSPACE_sequence;
}

unsigned int readRequestedProfileNumber() {
  static const unsigned int PROFILE_MODE_OFFSET = 46;
  int profile_number = readNextUniqueByte();
  xbee->clearBuffer();
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
