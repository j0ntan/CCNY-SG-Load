#ifndef ARDUINOINTERFACE_H
#define ARDUINOINTERFACE_H

#include <stdint.h>

#ifndef Arduino_h
#define F(string_literal) string_literal
#endif  // Arduino_h

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
