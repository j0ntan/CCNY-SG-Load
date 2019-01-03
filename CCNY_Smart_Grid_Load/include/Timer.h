#ifndef TIMER_H
#define TIMER_H

class Timer {
 public:
  typedef unsigned long milliseconds;
  typedef unsigned long microseconds;

  virtual ~Timer() = default;

  virtual void delay(milliseconds) const = 0;
  virtual void delayMicroseconds(microseconds) const = 0;
};

#endif  // TIMER_H
