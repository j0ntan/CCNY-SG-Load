#ifndef HARDWARETIMER_H
#define HARDWARETIMER_H

#include "Timer.h"

class HardwareTimer : public Timer {
 public:
  HardwareTimer() = default;
  ~HardwareTimer() final = default;

  HardwareTimer(const HardwareTimer&) = delete;
  HardwareTimer& operator=(const HardwareTimer&) = delete;

  void delay(Timer::milliseconds ms) const final;
  void delayMicroseconds(Timer::microseconds us) const final;
};

#endif  // HARDWARETIMER_H
