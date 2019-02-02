#include "../include/ShiftRegister.h"
#include "../include/Timer.h"
#include "../include/DigitalIO.h"

// Globals defined in main application
extern Timer* timer;

ShiftRegister::ShiftRegister(const DigitalOutput* serialDataInput,
                             const DigitalOutput* shiftRegisterClock,
                             const DigitalOutput* storageRegisterClock)
    : _SERIAL_DATA_INPUT(serialDataInput),
      _SHIFT_REGISTER_CLOCK(shiftRegisterClock),
      _STORAGE_REGISTER_CLOCK(storageRegisterClock) {
  _initPins();
  _initOutputs();
}

void ShiftRegister::shiftByte(const byte& data) const {
  for (uint8_t offset = 0; offset < 8; offset++) {
    // bits are output from MSB (bit 7) to LSB (bit 0)
    const byte data_shifted = data >> (7 - offset);
    const bool bit_value_HIGH = data_shifted & 0x01;
    if (bit_value_HIGH)
      _SERIAL_DATA_INPUT->set();
    else
      _SERIAL_DATA_INPUT->clear();
    _ticShiftRegisterClock();
  }
}

void ShiftRegister::updateOutput() const { _ticStorageRegisterClock(); }

void ShiftRegister::outputByte(const byte& data) const {
  shiftByte(data);
  updateOutput();
}

void ShiftRegister::_initPins() const {
  // set outputs to idle state
  _SERIAL_DATA_INPUT->clear();
  _SHIFT_REGISTER_CLOCK->clear();
  _STORAGE_REGISTER_CLOCK->clear();
}

void ShiftRegister::_initOutputs() const {
  // relay inputs are active-LOW, initialize all relays to be OFF
  for (uint8_t i = 0; i < 6; i++) shiftByte(0xFF);
  updateOutput();
}

void ShiftRegister::_ticShiftRegisterClock() const {
  _SHIFT_REGISTER_CLOCK->set();
  timer->delayMicroseconds(_SHIFT_CLOCK_DELAY);
  _SHIFT_REGISTER_CLOCK->clear();
  timer->delayMicroseconds(_SHIFT_CLOCK_DELAY);
}

void ShiftRegister::_ticStorageRegisterClock() const {
  _STORAGE_REGISTER_CLOCK->set();
  timer->delayMicroseconds(_STORAGE_CLOCK_DELAY);
  _STORAGE_REGISTER_CLOCK->clear();
  timer->delayMicroseconds(_STORAGE_CLOCK_DELAY);
}
