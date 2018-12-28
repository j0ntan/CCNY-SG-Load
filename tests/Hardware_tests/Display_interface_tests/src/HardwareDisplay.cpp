#include "../include/HardwareDisplay.h"

HardwareDisplay::HardwareDisplay(uint8_t lcdAddress)
    : _lcd(LiquidCrystal_I2C(lcdAddress)) {
  _lcd.begin(20, 4);
}

void HardwareDisplay::clear() { _lcd.clear(); }

void HardwareDisplay::setCursor(uint8_t col, uint8_t row) {
  _lcd.setCursor(col, row);
}

void HardwareDisplay::print(const String& message) { _lcd.print(message); }

void HardwareDisplay::print(const char& message) { _lcd.print(message); }

void HardwareDisplay::print(const int& message) { _lcd.print(message); }
