#ifndef HARDWAREIO_H
#define HARDWAREIO_H

#include "DigitalIO.h"
#include <stdint.h>

class HardwareInput : public DigitalInput {
 public:
  HardwareInput(uint8_t pin, bool enable_pullup = true);
  ~HardwareInput() final = default;

  bool read() const final;

 private:
  const uint8_t pin;
};

class HardwareOutput : public DigitalOutput {
 public:
  HardwareOutput(uint8_t pin);
  ~HardwareOutput() final = default;

  void set() const final;
  void clear() const final;

 private:
  const uint8_t pin;
};

#endif  // HARDWAREIO_H
