#ifndef DISPLAY_LCD_H
#define DISPLAY_LCD_H

#include <stdint.h>

class InputSequence;

namespace Display {
class Message;

class LCD {
 public:
  virtual ~LCD() = default;

  virtual void printChar(const char c, const uint8_t row, uint8_t col) = 0;
  virtual void printLine(const char* line, uint8_t row) = 0;
  virtual void printInput(const InputSequence& input, const uint8_t row) = 0;
  virtual void printMsg(const Display::Message& msg) = 0;
};
}  // namespace Display

#endif  // DISPLAY_LCD_H
