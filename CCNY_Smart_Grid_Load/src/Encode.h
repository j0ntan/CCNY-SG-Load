#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>

class RelayState;

typedef uint8_t byte;

struct ACRelayBits {
  byte phaseA_MSB = 0xFF;
  byte phaseA_LSB = 0xFF;
  byte phaseB_MSB = 0xFF;
  byte phaseB_LSB = 0xFF;
  byte phaseC_MSB = 0xFF;
  byte phaseC_LSB = 0xFF;
};

ACRelayBits encode(const RelayState& vals);

#endif  // ENCODE_H
