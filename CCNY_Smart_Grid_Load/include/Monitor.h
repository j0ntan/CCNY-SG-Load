#ifndef MONITOR_H
#define MONITOR_H

#include "ArduinoInterface.h"
#include "Keypad.h"

extern Keypad* keypad;

// function wrappers for improving sketch readability

bool keypadButtonWasPressed() { return keypad->anyButtonPressed(); }

#endif  // MONITOR_H
