#ifndef MONITOR_H
#define MONITOR_H

#include <stdint.h>
#include "ArduinoInterface.h"
#include "XBee.h"
#include "Keypad.h"

#define MAX_INPUT_LENGTH \
  11  // we assume that user will not send more than this amount of characters
      // in an input sequence
#define DSPACE_MANUAL_MODE_BOUND 45

extern Arduino* arduino;
extern Keypad* keypad;
extern XBee* xbee;

// function wrappers for improving sketch readability

bool keypadButtonWasPressed() { return keypad->anyButtonPressed(); }

inline bool receivedPCSerialData() {
  const uint8_t SERIAL_TIMEOUT_MILLISECONDS = 20;
  uint8_t current_byte_count = 0, millisec_elapsed = 0;
  do {
    current_byte_count = xbee->bytesAvailable();
    arduino->delay(2);
    millisec_elapsed += 2;
  } while (current_byte_count != xbee->bytesAvailable() &&
           millisec_elapsed <= SERIAL_TIMEOUT_MILLISECONDS);

  return current_byte_count <= MAX_INPUT_LENGTH &&
         millisec_elapsed <= SERIAL_TIMEOUT_MILLISECONDS;
}

inline bool receivedDSPACEManualData() {
  const uint8_t SERIAL_TIMEOUT_MILLISECONDS = 20;
  uint8_t millisec_elapsed = 0;
  do {
    arduino->delay(2);
    millisec_elapsed += 2;
  } while (xbee->bytesAvailable() <= MAX_INPUT_LENGTH &&
           millisec_elapsed <= SERIAL_TIMEOUT_MILLISECONDS);

  return (xbee->bytesAvailable() > MAX_INPUT_LENGTH ||
          millisec_elapsed > SERIAL_TIMEOUT_MILLISECONDS) &&
         xbee->peekByte() <= DSPACE_MANUAL_MODE_BOUND;
}

inline bool receivedDSPACELoadProfile() {
  const uint8_t SERIAL_TIMEOUT_MILLISECONDS = 20;
  uint8_t millisec_elapsed = 0;
  do {
    arduino->delay(2);
    millisec_elapsed += 2;
  } while (xbee->bytesAvailable() <= MAX_INPUT_LENGTH &&
           millisec_elapsed <= SERIAL_TIMEOUT_MILLISECONDS);

  return (xbee->bytesAvailable() > MAX_INPUT_LENGTH ||
          millisec_elapsed > SERIAL_TIMEOUT_MILLISECONDS) &&
         xbee->peekByte() > DSPACE_MANUAL_MODE_BOUND;
}

#undef MAX_INPUT_LENGTH
#undef DSPACE_MANUAL_MODE_BOUND
#endif  // MONITOR_H
