#ifndef OUTPUT_H
#define OUTPUT_H

#include "DigitalIO.h"
#include "Encode.h"
#include "ShiftRegister.h"

extern ShiftRegister shiftregister;

namespace helper {
void outputBitsToShiftRegisters(const ACRelayBits&);
void outputToDCRelays(const uint8_t&, const DigitalOutput*,
                      const DigitalOutput*);
}  // namespace helper

void outputAllRelays(const ACRelayBits& ACvalues, const uint8_t& DCvalue,
                     const DigitalOutput* DC_relay1_output,
                     const DigitalOutput* DC_relay2_output) {
  helper::outputBitsToShiftRegisters(ACvalues);
  helper::outputToDCRelays(DCvalue, DC_relay1_output, DC_relay2_output);
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

void helper::outputToDCRelays(const uint8_t& DCvalue,
                              const DigitalOutput* DC_relay1_output,
                              const DigitalOutput* DC_relay2_output) {
  switch (DCvalue) {
    case 0:
      DC_relay1_output->clear();
      DC_relay2_output->clear();
      break;
    case 1:
      DC_relay1_output->set();
      DC_relay2_output->clear();
      break;
    case 2:
      DC_relay1_output->set();
      DC_relay2_output->set();
      break;
  }
}

#endif  // OUTPUT_H
