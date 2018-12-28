#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

template <class StringT>
class Display {
 public:
  virtual ~Display() = default;

  virtual void clear() = 0;
  virtual void setCursor(uint8_t, uint8_t) = 0;
  virtual void print(const StringT&) = 0;
  virtual void print(const char&) = 0;
  virtual void print(const int&) = 0;
};

#endif  // DISPLAY_H
