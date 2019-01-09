#ifndef DISPLAYMESSAGES_H
#define DISPLAYMESSAGES_H

#include "Display.h"
#include "ArduinoInterface.h"
#include "Timer.h"
#include "BernoulliRNG.h"
#include "Keypad.h"
#include "XBee.h"
#include "Scan.h"
#include "RelayState.h"

extern Display<String>* display;
extern Timer* timer;
extern Keypad* keypad;
extern XBee* xbee;

namespace displayMessage {

enum class Speed : unsigned int {
  FASTER = 200,
  FAST = 500,
  NORMAL = 1200,
  SLOW = 1800
};

namespace helper {
void pause_display(Speed speed = Speed::NORMAL) {
  // Pause until display time is finished or interrupt occurs.

  const unsigned long TIME_INTERVAL = 100;
  int time_elapsed = 0;
  do {
    timer->delay(TIME_INTERVAL);
    time_elapsed += TIME_INTERVAL;
  } while (time_elapsed < static_cast<int>(speed) && !xbee->hasBufferedData() &&
           !keypad->anyButtonPressed());
}

void clear_char(const uint8_t& row, const uint8_t& col) {
  display->setCursor(col, row);
  display->print(" ");
}

void clear_row(const uint8_t& row) {
  for (uint8_t i = 0; i < 20; i++) clear_char(row, i);
}

void printSpace() { display->print(F(" ")); }
};  // namespace helper

void caution() {
  /*
  Shows a fun little message on the LCD about a bear inside the load :D */

  using namespace helper;

  display->clear();
  display->setCursor(5, 1);
  display->print(F("Caution!!"));
  display->setCursor(4, 2);
  display->print(F("!!Caution!!"));
  pause_display();

  display->clear();
  display->setCursor(4, 1);
  display->print(F("Grizzly bear"));
  display->setCursor(6, 2);
  display->print(F("inside!!"));
  pause_display();

  display->clear();
  display->setCursor(7, 1);
  display->print(F("DO NOT"));
  display->setCursor(7, 2);
  display->print(F("OPEN!!"));
  pause_display();
}

void initialization(const BernoulliRNG& rng) {
  using namespace helper;

  // lab & professor
  display->clear();
  display->setCursor(8, 0);
  display->print(F("CCNY"));
  display->setCursor(3, 1);
  display->print(F("Smart Grid Lab"));
  display->setCursor(2, 2);
  display->print(F("Prof. A. Mohamed"));
  display->setCursor(6, 3);
  display->print(F("ST 6/643"));
  pause_display(Speed::SLOW);

  // code author & current version
  display->clear();
  display->setCursor(0, 1);
  display->print(F("Author: J. Gamboa"));
  display->setCursor(0, 2);
  display->print(F("Code version: ?.?"));
  pause_display(Speed::FASTER);

  // Show 'caution' message at random initializations :)
  if (rng()) caution();

  // final msg
  display->clear();
  display->setCursor(2, 1);
  display->print(F("Ready for input."));
  pause_display(Speed::FASTER);
}

void load_reset() {
  display->clear();
  display->setCursor(4, 1);
  display->print(F("Load RESET"));
  helper::pause_display(Speed::FAST);
}

void keypad_sequence_begin() {
  display->clear();
  display->setCursor(0, 1);
  display->print(F("Keypad sequence:"));
}

template <class StringT>
void sequence_update(const StringT& input) {
  helper::clear_row(2);
  display->setCursor(0, 2);
  display->print(input);
}

void dspace_sequence_begin() {
  display->clear();
  display->setCursor(0, 1);
  display->print(F("dSPACE input:"));
}

void sdFileOpenError() {
  display->clear();
  display->setCursor(0, 0);
  display->print(F("Could not open the"));
  display->setCursor(0, 1);
  display->print(F("chosen file. Check"));
  display->setCursor(0, 2);
  display->print(F("the SD card module"));
  display->setCursor(0, 3);
  display->print(F("or file choice."));
  helper::pause_display(Speed::SLOW);
}

void foundInvalidChar(const String& input, const TokenSet& tokens) {
  uint8_t index = 0;
  while (tokens[index] != Token::INVALID) ++index;

  display->clear();
  display->setCursor(2, 1);
  display->print(F("Cannot interpret"));
  display->setCursor(2, 2);
  display->print(F("the character: "));
  display->print(input[index]);
  display->setCursor(3, 3);
  display->print(F("Repeat input."));
  helper::pause_display();
}

void missing_number() {
  display->clear();
  display->setCursor(7, 1);
  display->print(F("Error"));
  display->setCursor(1, 2);
  display->print(F("Input must contain"));
  display->setCursor(0, 3);
  display->print(F("at least one number."));
  helper::pause_display();
}

void not_numerical_ending() {
  display->clear();
  display->setCursor(7, 1);
  display->print(F("Error"));
  display->setCursor(3, 2);
  display->print(F("Input must end"));
  display->setCursor(3, 3);
  display->print(F("with a number."));
  helper::pause_display();
}

void DC_out_of_range() {
  display->clear();
  display->setCursor(7, 1);
  display->print(F("Error"));
  display->setCursor(1, 2);
  display->print(F("DC value is out of"));
  display->setCursor(2, 3);
  display->print(F("range. Max is 2."));
  helper::pause_display();
}

void AC_out_of_range() {
  display->clear();
  display->setCursor(7, 1);
  display->print(F("Error"));
  display->setCursor(1, 2);
  display->print(F("AC value is out of"));
  display->setCursor(1, 3);
  display->print(F("range. Max is 16."));
  helper::pause_display();
}

void no_starting_phase() {
  display->clear();
  display->setCursor(7, 1);
  display->print(F("Error"));
  display->setCursor(0, 2);
  display->print(F("Input sequence must"));
  display->setCursor(0, 3);
  display->print(F("begin with a phase."));
  helper::pause_display();
}

void phase_repeats() {
  display->clear();
  display->setCursor(7, 1);
  display->print(F("Error"));
  display->setCursor(1, 2);
  display->print(F("Phase is repeated."));
  helper::pause_display();
}

void found_leading_zero() {
  display->clear();
  display->setCursor(7, 1);
  display->print(F("Error"));
  display->setCursor(3, 2);
  display->print(F("Remove leading"));
  display->setCursor(0, 3);
  display->print(F("zeros, i.e. 6 vs. 06"));
  helper::pause_display();
}

void unordered_phases() {
  display->clear();
  display->setCursor(7, 1);
  display->print(F("Error"));
  display->setCursor(3, 2);
  display->print(F("Phases are not"));
  display->setCursor(5, 3);
  display->print(F("in order."));
  helper::pause_display();
}

void loadIdleState(const RelayState& state) {
  using namespace helper;

  display->clear();
  display->setCursor(0, 0);
  display->print(F("Idle state:"));

  display->setCursor(0, 1);
  display->print(F("AC(3-ph) ="));
  display->setCursor(0, 2);
  display->print(F("A:"));
  display->print(state.phaseA);
  if (state.phaseA < 10) printSpace();
  printSpace();
  display->print(F("B:"));
  display->print(state.phaseB);
  if (state.phaseB < 10) printSpace();
  printSpace();
  display->print(F("C:"));
  display->print(state.phaseC);
  if (state.phaseC < 10) printSpace();
  printSpace();

  display->setCursor(0, 3);
  display->print(F("DC:"));
  display->print(state.DC);
}
};  // namespace displayMessage

#endif  // DISPLAYMESSAGES_H
