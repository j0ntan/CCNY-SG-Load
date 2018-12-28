#include "../include/HardwareArduino.h"

void HardwareArduino::pinMode(uint8_t pin, uint8_t mode) const {
  ::pinMode(pin, mode);
}

void HardwareArduino::digitalWrite(uint8_t pin, uint8_t value) const {
  ::digitalWrite(pin, value);
}

int HardwareArduino::digitalRead(uint8_t pin) const {
  return ::digitalRead(pin);
}

int HardwareArduino::analogRead(uint8_t pin) const { return ::analogRead(pin); }

void HardwareArduino::delay(unsigned long ms) const { ::delay(ms); }

void HardwareArduino::delayMicroseconds(unsigned int us) const {
  ::delayMicroseconds(us);
}

long HardwareArduino::random(long max) const { return ::random(max); }

long HardwareArduino::random(long min, long max) const {
  return ::random(min, max);
}

void HardwareArduino::randomSeed(unsigned long seed) const {
  ::randomSeed(seed);
}
