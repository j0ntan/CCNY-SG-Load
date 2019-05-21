#ifndef XBEE_MOCK_H
#define XBEE_MOCK_H

#include <gmock/gmock.h>
#include <XBee.h>

class XBeeMock : public XBee {
 public:
  ~XBeeMock() override = default;

  MOCK_CONST_METHOD0(bytesAvailable, int());
  MOCK_CONST_METHOD0(hasBufferedData, bool());
  MOCK_CONST_METHOD0(clearBuffer, void());
  MOCK_CONST_METHOD0(readByte, int());
  MOCK_CONST_METHOD0(peekByte, int());
};

#endif  // XBEE_MOCK_H
