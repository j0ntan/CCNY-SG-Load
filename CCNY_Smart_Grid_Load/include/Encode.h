#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>
#include "ArduinoInterface.h"
#include "RelayState.h"

struct ACRelayBits {
  byte phaseA_MSB = 0xFF;
  byte phaseA_LSB = 0xFF;
  byte phaseB_MSB = 0xFF;
  byte phaseB_LSB = 0xFF;
  byte phaseC_MSB = 0xFF;
  byte phaseC_LSB = 0xFF;
};

namespace helper {
void setPhaseBits(byte& MSB, byte& LSB, const uint8_t& setVal) {
  if (setVal > 8) {
    MSB = MSB << (setVal - 8);
    LSB = 0x00;
  } else {
    MSB = 0xFF;
    LSB = LSB << setVal;
  }
}
}  // namespace helper

ACRelayBits encode(const RelayState& vals) {
  ACRelayBits result;
  helper::setPhaseBits(result.phaseA_MSB, result.phaseA_LSB, vals.phaseA);
  helper::setPhaseBits(result.phaseB_MSB, result.phaseB_LSB, vals.phaseB);
  helper::setPhaseBits(result.phaseC_MSB, result.phaseC_LSB, vals.phaseC);
  return result;
}

#endif  // ENCODE_H
