#ifndef XBEE_H
#define XBEE_H

class XBee {
 public:
  virtual ~XBee() = default;

  virtual int bytesAvailable() const = 0;
  virtual bool hasBufferedData() const = 0;
  virtual int readByte() const = 0;
  virtual int peekByte() const = 0;
};

#endif  // XBEE_H
