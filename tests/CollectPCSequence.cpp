#include <mocks/Timer.h>
#include <mocks/XBee.h>
#include <utils/InputSequence.h>
#include <InputSequence.h>
#include <Collect.h>
#include <memory>

using namespace testing;

extern XBee* xbee;
extern Timer* timer;

class PCInputCollection : public Test {
 public:
  std::unique_ptr<NiceMock<XBeeMock>> xbeeMock =
      std::make_unique<NiceMock<XBeeMock>>();
  std::unique_ptr<NiceMock<TimerMock>> timerMock =
      std::make_unique<NiceMock<TimerMock>>();

  void SetUp() final {
    xbee = xbeeMock.get();
    timer = timerMock.get();
  }
};

TEST_F(PCInputCollection, collectSimpleSequence) {
  EXPECT_CALL(*xbeeMock, hasBufferedData())
      .WillOnce(Return(true))
      .WillOnce(Return(true))
      .WillOnce(Return(true))
      .WillOnce(Return(true))
      .WillOnce(Return(true))
      .WillOnce(Return(true))
      .WillOnce(Return(false));

  EXPECT_CALL(*xbeeMock, readByte())
      .WillOnce(Return(static_cast<int>('A')))
      .WillOnce(Return(static_cast<int>('1')))
      .WillOnce(Return(static_cast<int>('B')))
      .WillOnce(Return(static_cast<int>('2')))
      .WillOnce(Return(static_cast<int>('C')))
      .WillOnce(Return(static_cast<int>('3')));

  ASSERT_EQ(collectPCSequence(), "A1B2C3");
}
