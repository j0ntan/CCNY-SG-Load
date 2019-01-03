#include "../include/ShiftRegister.h"
#include "../include/Timer.h"

extern Timer *timer;

ShiftRegister::ShiftRegister(const pin serialDataInput,
                             const pin shiftRegisterClock,
                             const pin storageRegisterClock)
    : _SERIAL_DATA_INPUT(serialDataInput),
      _SHIFT_REGISTER_CLOCK(shiftRegisterClock),
      _STORAGE_REGISTER_CLOCK(storageRegisterClock) {
  _initPins();
  _initOutputs();
}

void ShiftRegister::shiftByte(const byte &data) const {
  byte output_bit = 0, shifted_byte = 0;
  for (uint8_t offset = 0; offset < 8; offset++) {
    // bits are output in reverse
    shifted_byte = data >> (7 - offset);
    output_bit = shifted_byte & B00000001;

    digitalWrite(_SERIAL_DATA_INPUT, output_bit);
    _ticShiftRegisterClock();
  }
}

void ShiftRegister::updateOutput() const { _ticStorageRegisterClock(); }

void ShiftRegister::outputByte(const byte &data) const {
  shiftByte(data);
  updateOutput();
}

void ShiftRegister::_initPins() const {
  pinMode(_SERIAL_DATA_INPUT, OUTPUT);
  pinMode(_SHIFT_REGISTER_CLOCK, OUTPUT);
  pinMode(_STORAGE_REGISTER_CLOCK, OUTPUT);

  // set pins to idle state
  digitalWrite(_SERIAL_DATA_INPUT, LOW);
  digitalWrite(_SHIFT_REGISTER_CLOCK, LOW);
  digitalWrite(_STORAGE_REGISTER_CLOCK, LOW);
}

void ShiftRegister::_initOutputs() const {
  // relay inputs are active-LOW, initialize all relays to be OFF
  for (uint8_t i = 0; i < 6; i++) shiftByte(0xFF);
  updateOutput();
}

void ShiftRegister::_ticShiftRegisterClock() const {
  digitalWrite(_SHIFT_REGISTER_CLOCK, HIGH);
  timer->delayMicroseconds(_SHIFT_CLOCK_DELAY);
  digitalWrite(_SHIFT_REGISTER_CLOCK, LOW);
  timer->delayMicroseconds(_SHIFT_CLOCK_DELAY);
}

void ShiftRegister::_ticStorageRegisterClock() const {
  digitalWrite(_STORAGE_REGISTER_CLOCK, HIGH);
  timer->delayMicroseconds(_STORAGE_CLOCK_DELAY);
  digitalWrite(_STORAGE_REGISTER_CLOCK, LOW);
  timer->delayMicroseconds(_STORAGE_CLOCK_DELAY);
}
