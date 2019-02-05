#ifndef HARDWAREXBEE_H
#define HARDWAREXBEE_H

#include <HardwareSerial.h>
#include "XBee.h"

extern "C" {
void delay(unsigned long);
}

class HardwareXBee : public XBee {
 public:
  HardwareXBee(HardwareSerial& serial) : _serial(serial) {}
  ~HardwareXBee() final = default;

  HardwareXBee() = delete;
  HardwareXBee(const HardwareXBee&) = delete;
  HardwareXBee& operator=(const HardwareXBee& rhs) = delete;

  int bytesAvailable() const final { return _serial.available(); }
  bool hasBufferedData() const final { return _serial.available() > 0; }
  void clearBuffer() const final {
    while (hasBufferedData()) {
      readByte();
      delay(5);  // guard time, allow incoming data to fill buffer
    }
  }
  int readByte() const final { return _serial.read(); }
  int peekByte() const final { return _serial.peek(); }

 private:
  HardwareSerial& _serial;
};

#endif  // HARDWAREXBEE_H
