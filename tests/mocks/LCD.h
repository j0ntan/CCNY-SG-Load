#ifndef LCD_MOCK_H
#define LCD_MOCK_H

#include <gmock/gmock.h>
#include <Display/LCD.h>
#include <InputSequence.h>
#include <Display/Message.h>

class LCDMock : public Display::LCD {
 public:
  ~LCDMock() override = default;

  MOCK_METHOD3(printChar, void(const char, const uint8_t, uint8_t));
  MOCK_METHOD2(printLine, void(const char*, uint8_t));
  MOCK_METHOD2(printInput, void(const InputSequence&, const uint8_t));
  MOCK_METHOD1(printMsg, void(const Display::Message&));
};

#endif  // LCD_MOCK_H
