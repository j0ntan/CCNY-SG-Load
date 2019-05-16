#ifndef DISPLAY_HARDWARELCD_H
#define DISPLAY_HARDWARELCD_H

#include "LCD.h"
#include <LiquidCrystal_I2C.h>

namespace Display {
class HardwareLCD : public LCD {
 public:
  HardwareLCD(uint8_t I2C_address);
  ~HardwareLCD() final = default;

  HardwareLCD(const HardwareLCD& lcd) = delete;

  void printChar(const char c, const uint8_t row, uint8_t col) final;
  void printLine(const char* line, uint8_t row) final;
  void printInput(const InputSequence& input, const uint8_t row) final;
  void printMsg(const Display::Message& msg) final;

 private:
  LiquidCrystal_I2C hw_lcd;
};
}  // namespace Display

#endif  // DISPLAY_HARDWARELCD_H
