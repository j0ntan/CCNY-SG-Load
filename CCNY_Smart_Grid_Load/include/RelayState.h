#ifndef RELAYSTATE_H
#define RELAYSTATE_H

#include <stdint.h>

struct RelayState {
  uint8_t phaseA = 0;
  uint8_t phaseB = 0;
  uint8_t phaseC = 0;
  uint8_t DC = 0;
};

#endif
