#include "../include/HardwareRNG.h"
#include "Arduino.h"

// argument 'p' is not used, p was hard-coded as 0.2 in the vals array
HardwareRNG::HardwareRNG(double p) : BernoulliRNG{p} {}

bool HardwareRNG::operator()() const {
  randomSeed(analogRead(0));
  return vals[random(SIZE)];
}
