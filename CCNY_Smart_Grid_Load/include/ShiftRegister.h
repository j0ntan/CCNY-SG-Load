#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#include "Arduino.h"

class ShiftRegister {
 public:
  typedef uint8_t pin;

  ShiftRegister(const pin serialDataInput, const pin shiftRegisterClock,
                const pin storageRegisterClock);

  ShiftRegister() = delete;
  ShiftRegister(const ShiftRegister &) = delete;
  ShiftRegister &operator=(const ShiftRegister &rhs) = delete;

  void shiftByte(const byte &data) const;
  void updateOutput() const;
  void outputByte(const byte &data) const;

 private:
  void _initPins() const;
  void _initOutputs() const;
  void _ticShiftRegisterClock() const;
  void _ticStorageRegisterClock() const;

  // shift register pins
  const pin _SERIAL_DATA_INPUT;
  const pin _SHIFT_REGISTER_CLOCK;
  const pin _STORAGE_REGISTER_CLOCK;

  // clock periods in microseconds
  static const unsigned int _SHIFT_CLOCK_DELAY = 1000;
  static const unsigned int _STORAGE_CLOCK_DELAY = 1000;
};

#endif  // SHIFT_REGISTER_H
