#include <mocks/XBee.h>
#include <mocks/Timer.h>
#include <mocks/LCD.h>
#include <utils/InputSequence.h>
#include <InputSequence.h>
#include <Display/MessagesList.h>
#include <Collect.h>
#include <string>

using namespace testing;

extern XBee* xbee;
extern Timer* timer;
extern Display::LCD* lcd;
NiceMock<XBeeMock> xbeeMock;
NiceMock<TimerMock> timerMock;
NiceMock<LCDMock> lcdMock;

void setExpectationsForSingleInput(int input_numerical_value,
                                   int num_of_times_sent, bool is_last_input) {
  EXPECT_CALL(xbeeMock, readByte())
      .WillOnce(Return(input_numerical_value));  // read first byte
  for (int i = 0; i < num_of_times_sent; ++i) {
    EXPECT_CALL(xbeeMock, peekByte())
        .WillOnce(Return(input_numerical_value));  // peek and find a copy
    EXPECT_CALL(xbeeMock, readByte());             // discard copy
    EXPECT_CALL(timerMock, delay(_));
  }
  EXPECT_CALL(xbeeMock, peekByte())
      .WillOnce(Return(-1));  // find different byte or empty buffer

  EXPECT_CALL(xbeeMock, hasBufferedData()).WillOnce(Return(!is_last_input));
}

class dSPACEInputAdditionCommandCollection : public Test {
 public:
  void SetUp() final {
    xbee = &xbeeMock;
    timer = &timerMock;
    lcd = &lcdMock;
  }
};

TEST_F(dSPACEInputAdditionCommandCollection, recordSimpleTransmission) {
  InSequence s;
  // dSPACE transmission:
  // AAAAAAAAAAAA1111111222BBBBBBBBBCCCC44444444444444444DDDDDD11111111111111
  setExpectationsForSingleInput(17, 12, false);  // transmit 'A', 12 times
  setExpectationsForSingleInput(1, 7, false);    // transmit '1', 7 times
  setExpectationsForSingleInput(4, 3, false);    // transmit '4', 3 times
  setExpectationsForSingleInput(18, 9, false);   // transmit 'B', 9 times
  setExpectationsForSingleInput(19, 4, false);   // transmit 'C', 4 times
  setExpectationsForSingleInput(8, 13, false);   // transmit '8', 13 times
  setExpectationsForSingleInput(20, 6, false);   // transmit 'D', 6 times
  setExpectationsForSingleInput(1, 14, true);    // transmit '1', 14 times

  ASSERT_EQ(collectDSPACESequence(), "A14BC8D1");
}

class dSPACEModifierCommandCollection : public Test {
 public:
  void SetUp() final {
    xbee = &xbeeMock;
    timer = &timerMock;
    lcd = &lcdMock;
  }
};

TEST_F(dSPACEModifierCommandCollection, cancelEmptySequence) {
  InSequence s;
  // dSPACE transmission: (CANCEL)
  setExpectationsForSingleInput(21, 5, true);  // transmit CANCEL, 5 times

  ASSERT_EQ(collectDSPACESequence().length(), 0);
}

TEST_F(dSPACEModifierCommandCollection, cancelNonEmptySequence) {
  InSequence s;
  // dSPACE transmission: AAAAAAAAAAAA00000000(CANCEL)
  setExpectationsForSingleInput(17, 12, false);  // transmit 'A', 12 times
  setExpectationsForSingleInput(0, 8, false);    // transmit '0', 8 times
  setExpectationsForSingleInput(21, 3, true);    // transmit CANCEL, 3 times

  ASSERT_EQ(collectDSPACESequence().length(), 0);
}

TEST_F(dSPACEModifierCommandCollection, backspaceErasesLastInput) {
  InSequence s;
  // dSPACE transmission: AAAAAAAAAAAA00000000(BACKSPACE)
  setExpectationsForSingleInput(17, 12, false);  // transmit 'A', 12 times
  setExpectationsForSingleInput(0, 8, false);    // transmit '0', 8 times
  setExpectationsForSingleInput(22, 3, true);    // transmit BACKSPACE, 3 times

  ASSERT_EQ(collectDSPACESequence(), "A");
}

TEST_F(dSPACEModifierCommandCollection, backspaceDoesNotModifyEmptySequence) {
  InSequence s;
  // dSPACE transmission: (BACKSPACE)
  setExpectationsForSingleInput(22, 3, true);  // transmit BACKSPACE, 3 times

  ASSERT_EQ(collectDSPACESequence().length(), 0);
}

TEST_F(dSPACEModifierCommandCollection, applyResetSequence) {
  InSequence s;
  // dSPACE transmission: (RESET)
  setExpectationsForSingleInput(23, 13, true);  // transmit RESET, 13 times

  ASSERT_EQ(collectDSPACESequence(), "ABCD0");
}

TEST_F(dSPACEModifierCommandCollection, resetSequenceOverridesPreviousInput) {
  InSequence s;
  // dSPACE transmission: AAAAAAAAAAAA00000000(RESET)
  setExpectationsForSingleInput(17, 12, false);  // transmit 'A', 12 times
  setExpectationsForSingleInput(0, 8, false);    // transmit '0', 8 times
  setExpectationsForSingleInput(23, 3, true);    // transmit RESET, 3 times

  ASSERT_EQ(collectDSPACESequence(), "ABCD0");
}

TEST_F(dSPACEModifierCommandCollection, handleUnknownCommand) {
  InSequence s;
  // dSPACE transmission: (UNKNOWN)
  setExpectationsForSingleInput(25, 5, true);  // transmit UNKNOWN, 5 times

  ASSERT_EQ(collectDSPACESequence(), "?");
}

TEST_F(dSPACEModifierCommandCollection, applyBalancedInputSequence) {
  InSequence s;
  // dSPACE transmission: (BALANCED, 11)
  setExpectationsForSingleInput(40, 8,
                                true);  // transmit BALANCED(11), 8 times

  ASSERT_EQ(collectDSPACESequence(), "ABC11");
}

TEST(dSPACELoadProfile, createFilenames0to999) {
  for (unsigned int number = 0; number < 1000; number++) {
    std::string file_number;
    if (number < 10)
      file_number += "00";
    else if (number < 100)
      file_number += "0";
    file_number += std::to_string(number);
    const std::string expected = "PRFL" + file_number + ".txt";

    char filename[12] = {"PRFL000.txt"};
    createFilename(number, filename);

    ASSERT_STREQ(filename, expected.c_str());
  }
}
