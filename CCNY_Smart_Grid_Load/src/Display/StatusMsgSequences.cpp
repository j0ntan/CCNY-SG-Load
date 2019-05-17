#include "../../include/Display/StatusMsgSequences.h"
#include "../../include/RelayState.h"
#include "../../include/BernoulliRNG.h"
#include "../../include/Display/MessagesList.h"
#include "../../include/Display/LCD.h"
#include <stdio.h>

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
  snprintf(AC_phases_str + 2, 2, "%d", state.phaseA);
  snprintf(AC_phases_str + 7, 2, "%d", state.phaseA);
  snprintf(AC_phases_str + 12, 2, "%d", state.phaseA);

  char DC_str[Display::COLS + 1] = "D:";
  snprintf(DC_str + 2, 1, "$d", state.DC);

  lcd->printLine(AC_phases_str, 2);
  lcd->printLine(DC_str, 3);
}
