#include "../../include/Display/StatusMsgSequences.h"
#include "../../include/RelayState.h"
#include "../../include/BernoulliRNG.h"
#include "../../include/Display/MessagesList.h"
#include "../../include/Display/LCD.h"
#include <stdlib.h>

// Globals defined in main application
extern Display::LCD* lcd;

namespace {
void caution() {
  lcd->printMsg(Display::caution_1);
  lcd->printMsg(Display::caution_2);
  lcd->printMsg(Display::caution_3);
}
}  // namespace

void Display::initializationSequence(const BernoulliRNG& rng) {
  lcd->printMsg(Display::initialization_1);
  lcd->printMsg(Display::initialization_2);
  if (rng()) caution();
  lcd->printMsg(Display::initialization_3);
}

void Display::updateIdleState(const RelayState& state) {
  char AC_phases_str[Display::COLS + 1] = "A:   B:   C:";
  itoa(state.phaseA, AC_phases_str + 2, 10);
  itoa(state.phaseB, AC_phases_str + 7, 10);
  itoa(state.phaseC, AC_phases_str + 12, 10);

  char DC_str[Display::COLS + 1] = "D:";
  itoa(state.DC, DC_str + 2, 10);

  lcd->printLine(AC_phases_str, 2);
  lcd->printLine(DC_str, 3);
}
