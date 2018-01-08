#ifndef OUTPUT_H
#define OUTPUT_H

#include "Encode.h"
#include "ShiftRegister.h"

#define DC_Relay_Pin1 22
#define DC_Relay_Pin2 23

extern ShiftRegister shiftregister;

namespace helper {
void outputBitsToShiftRegisters(const ACRelayBits&);
void outputToDCRelays(const uint8_t&);
}  // namespace helper

void outputAllRelays(const ACRelayBits& ACvalues, const uint8_t& DCvalue) {
  helper::outputBitsToShiftRegisters(ACvalues);
  helper::outputToDCRelays(DCvalue);
}

void helper::outputBitsToShiftRegisters(const ACRelayBits& bits) {
  shiftregister.shiftByte(bits.phaseC_MSB);
  shiftregister.shiftByte(bits.phaseC_LSB);
  shiftregister.shiftByte(bits.phaseB_MSB);
  shiftregister.shiftByte(bits.phaseB_LSB);
  shiftregister.shiftByte(bits.phaseA_MSB);
  shiftregister.shiftByte(bits.phaseA_LSB);
  shiftregister.updateOutput();
}

void helper::outputToDCRelays(const uint8_t& DCvalue) {
  switch (DCvalue) {
    case 0:
      digitalWrite(DC_Relay_Pin1, LOW);
      digitalWrite(DC_Relay_Pin2, LOW);
      break;
    case 1:
      digitalWrite(DC_Relay_Pin1, HIGH);
      digitalWrite(DC_Relay_Pin2, LOW);
      break;
    case 2:
      digitalWrite(DC_Relay_Pin1, HIGH);
      digitalWrite(DC_Relay_Pin2, HIGH);
      break;
  }
}

#endif  // OUTPUT_H
