#ifndef HARDWAREDISPLAY_H
#define HARDWAREDISPLAY_H

#include "Display.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class HardwareDisplay : public Display<String> {
 public:
  HardwareDisplay(uint8_t lcdAddress);
  ~HardwareDisplay() final = default;

  HardwareDisplay(const HardwareDisplay&) = delete;
  HardwareDisplay& operator=(const HardwareDisplay& rhs) = delete;

  void clear() final;
  void setCursor(uint8_t col, uint8_t row) final;
  void print(const String& message) final;
  void print(const char& message) final;
  void print(const int& message) final;

 private:
  LiquidCrystal_I2C _lcd;
};

#endif  // HARDWAREDISPLAY_H
