#include "../include/HardwareTimer.h"
#include <Arduino.h>

void HardwareTimer::delay(Timer::milliseconds ms) const { ::delay(ms); }

void HardwareTimer::delayMicroseconds(Timer::microseconds us) const {
  ::delayMicroseconds(us);
}
