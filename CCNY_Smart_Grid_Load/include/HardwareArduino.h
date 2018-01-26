#ifndef ARDUINOHW_H
#define ARDUINOHW_H

#include <Arduino.h>
#include "ArduinoInterface.h"

class HardwareArduino : public Arduino {
 public:
  HardwareArduino() = default;
  ~HardwareArduino() final = default;

  HardwareArduino(const HardwareArduino&) = delete;
  HardwareArduino& operator=(const HardwareArduino&) = delete;

  void pinMode(uint8_t pin, uint8_t mode) const final;
  void digitalWrite(uint8_t pin, uint8_t value) const final;
  int digitalRead(uint8_t pin) const final;
  int analogRead(uint8_t pin) const final;
  void delay(unsigned long ms) const final;
  void delayMicroseconds(unsigned int us) const final;

  long random(long upperbound) const final;
  long random(long lowerbound, long upperbound) const final;
  void randomSeed(unsigned long seed) const final;
};

#endif  // ARDUINOHW_H
