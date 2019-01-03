#ifndef TIMER_MOCK_H
#define TIMER_MOCK_H

#include <gmock/gmock.h>
#include <Timer.h>

class TimerMock : public Timer {
 public:
  ~TimerMock() override = default;

  MOCK_CONST_METHOD1(delay, void(milliseconds));
  MOCK_CONST_METHOD1(delayMicroseconds, void(microseconds));
};

#endif  // TIMER_MOCK_H
