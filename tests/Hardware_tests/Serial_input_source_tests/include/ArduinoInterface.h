#ifndef ARDUINOINTERFACE_H
#define ARDUINOINTERFACE_H

#include <stdint.h>

#ifndef Arduino_h
#define F(string_literal) string_literal
typedef uint8_t byte;
#define B10000000 128
#define B00000001 1
#endif  // Arduino_h

#define HIGH 0x1
#define LOW 0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

class Arduino {
 public:
  virtual ~Arduino() = default;

  virtual void pinMode(uint8_t pin, uint8_t mode) const = 0;
  virtual void digitalWrite(uint8_t pin, uint8_t value) const = 0;
  virtual int digitalRead(uint8_t pin) const = 0;
  virtual int analogRead(uint8_t pin) const = 0;
  virtual void delay(unsigned long ms) const = 0;
  virtual void delayMicroseconds(unsigned int us) const = 0;

  virtual long random(long upperbound) const = 0;
  virtual long random(long lowerbound, long upperbound) const = 0;
  virtual void randomSeed(unsigned long seed) const = 0;
};

#endif  // ARDUINOINTERFACE_H
