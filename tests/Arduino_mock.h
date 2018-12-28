#ifndef ARDUINO_MOCK_H
#define ARDUINO_MOCK_H

#include <gmock/gmock.h>
#include <ArduinoInterface.h>

class ArduinoMock : public Arduino {
 public:
  ~ArduinoMock() override = default;

  MOCK_CONST_METHOD2(pinMode, void(uint8_t, uint8_t));
  MOCK_CONST_METHOD2(digitalWrite, void(uint8_t, uint8_t));
  MOCK_CONST_METHOD1(digitalRead, int(uint8_t));
  MOCK_CONST_METHOD1(analogRead, int(uint8_t));
  MOCK_CONST_METHOD1(delay, void(unsigned long));
  MOCK_CONST_METHOD1(delayMicroseconds, void(unsigned int));
  MOCK_CONST_METHOD1(random, long(long));
  MOCK_CONST_METHOD2(random, long(long, long));
  MOCK_CONST_METHOD1(randomSeed, void(unsigned long));
};

#endif  // ARDUINO_MOCK_H
