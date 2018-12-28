#ifndef HARDWAREXBEE_H
#define HARDWAREXBEE_H

#include <HardwareSerial.h>
#include "XBee.h"

class HardwareXBee : public XBee {
 public:
  HardwareXBee(HardwareSerial& serial) : _serial(serial) {}
  ~HardwareXBee() final = default;

  HardwareXBee() = delete;
  HardwareXBee(const HardwareXBee&) = delete;
  HardwareXBee& operator=(const HardwareXBee& rhs) = delete;

  int bytesAvailable() const final { return _serial.available(); }
  bool hasBufferedData() const final { return _serial.available() > 0; }
  int readByte() const final { return _serial.read(); }
  int peekByte() const final { return _serial.peek(); }

 private:
  HardwareSerial& _serial;
};

#endif  // HARDWAREXBEE_H
