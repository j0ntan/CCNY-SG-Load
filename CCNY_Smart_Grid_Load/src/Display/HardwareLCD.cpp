#include "../../include/Display/HardwareLCD.h"
#include "../../include/Display/Message.h"
#include "../../include/InputSequence.h"
#include "../../include/Timer.h"
#include "../../include/Keypad.h"
#include "../../include/XBee.h"
#include <avr/pgmspace.h>
#include <string.h>

// Globals defined in main application
extern Timer* timer;
extern Keypad* keypad;
extern XBee* xbee;

namespace {
void pause_display(Display::Duration duration) {
  const unsigned int TIME_INTERVAL = 100;
  unsigned int time_elapsed = 0;
  do {
    timer->delay(TIME_INTERVAL);
    time_elapsed += TIME_INTERVAL;
  } while (time_elapsed < static_cast<unsigned int>(duration) &&
           !xbee->hasBufferedData() && !keypad->anyButtonPressed());
}
}  // namespace

Display::HardwareLCD::HardwareLCD(uint8_t I2C_address) : hw_lcd{I2C_address} {
  hw_lcd.begin(Display::COLS, Display::ROWS);
}

void Display::HardwareLCD::printChar(const char c, const uint8_t row,
                                     uint8_t col) {
  hw_lcd.setCursor(col, row);
  hw_lcd.print(c);
}

void Display::HardwareLCD::printLine(const char* line, uint8_t row) {
  const uint8_t LENGTH = strlen(line);
  hw_lcd.setCursor(0, row);
  for (uint8_t i = 0; i < Display::COLS + 1; ++i)
    if (i < LENGTH)
      hw_lcd.print(line[i]);
    else  // print blank chars after line
      hw_lcd.print(' ');
}

void Display::HardwareLCD::printInput(const InputSequence& input,
                                      const uint8_t row) {
  char str[Display::COLS + 1] = {};
  for (uint8_t i = 0; i < input.length() && i < Display::COLS; ++i)
    str[i] = input[i];
  printLine(str, row);
}

void Display::HardwareLCD::printMsg(const Display::Message& msg) {
  hw_lcd.clear();

  // retrieve from PROGMEM
  Display::Message temp{};
  memcpy_P(&temp, &msg, sizeof temp);

  hw_lcd.print(temp.str[0]);
  hw_lcd.print(temp.str[1]);
  hw_lcd.print(temp.str[2]);
  hw_lcd.print(temp.str[3]);

  if (temp.duration != Display::Duration::STATIC) pause_display(temp.duration);
}
