#include "../include/Encode.h"
#include "../include/RelayState.h"

namespace {
void setPhaseBits(byte& MSB, byte& LSB, const uint8_t& setVal) {
  if (setVal > 8) {
    MSB = MSB << (setVal - 8);
    LSB = 0x00;
  } else {
    MSB = 0xFF;
    LSB = LSB << setVal;
  }
}
}  // namespace

ACRelayBits encode(const RelayState& vals) {
  ACRelayBits result;
  setPhaseBits(result.phaseA_MSB, result.phaseA_LSB, vals.phaseA);
  setPhaseBits(result.phaseB_MSB, result.phaseB_LSB, vals.phaseB);
  setPhaseBits(result.phaseC_MSB, result.phaseC_LSB, vals.phaseC);
  return result;
}
