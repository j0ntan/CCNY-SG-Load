#include "../include/RelayState.h"
#include "../include/ShiftRegister.h"
#include "../include/DigitalIO.h"
#include "../include/Encode.h"

// Globals defined in main application
extern ShiftRegister shiftregister;
extern DigitalOutput* relay1_output;
extern DigitalOutput* relay2_output;

namespace {
void outputBitsToShiftRegisters(const ACRelayBits& bits) {
  shiftregister.shiftByte(bits.phaseC_MSB);
  shiftregister.shiftByte(bits.phaseC_LSB);
  shiftregister.shiftByte(bits.phaseB_MSB);
  shiftregister.shiftByte(bits.phaseB_LSB);
  shiftregister.shiftByte(bits.phaseA_MSB);
  shiftregister.shiftByte(bits.phaseA_LSB);
  shiftregister.updateOutput();
}

void outputToDCRelays(const uint8_t& DCvalue) {
  switch (DCvalue) {
    case 0:
      relay1_output->clear();
      relay2_output->clear();
      break;
    case 1:
      relay1_output->set();
      relay2_output->clear();
      break;
    case 2:
      relay1_output->set();
      relay2_output->set();
      break;
  }
}
}  // namespace

void outputNewRelayState(const RelayState& newRelayState) {
  outputBitsToShiftRegisters(encode(newRelayState));
  outputToDCRelays(newRelayState.DC);
}
