#include <gmock/gmock.h>
#include <mocks/Keypad.h>
#include <mocks/Timer.h>
#include <InputSequence.h>
#include <utils/InputSequence.h>
#include <XBee.h>
#include <Collect.h>
#include <memory>
#include <string.h>
#include <string>

using namespace ::testing;

Keypad* keypad = nullptr;
NiceMock<TimerMock> timerMock;
Timer* timer = &timerMock;

class KeyPressCollection : public Test {
 public:
  std::unique_ptr<NiceMock<KeypadMock>> keypadMock;

  void SetUp() final {
    keypadMock = std::make_unique<NiceMock<KeypadMock>>();
    keypad = keypadMock.get();
  }
};

TEST_F(KeyPressCollection, recordSimpleSequence) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::A))      // Press 'A'
      .WillOnce(Return(Keypad::ButtonID::B))      // Press 'B'
      .WillOnce(Return(Keypad::ButtonID::C))      // Press 'C'
      .WillOnce(Return(Keypad::ButtonID::NUM1))   // Press '1'
      .WillOnce(Return(Keypad::ButtonID::NUM6))   // Press '6'
      .WillOnce(Return(Keypad::ButtonID::HASH));  // Press '#' (end sequence)

  ASSERT_EQ(collectKeypadSequence(), "ABC16");
}

TEST_F(KeyPressCollection, holdingHashEndsSequenceWithoutModification) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::A))      // Press 'A'
      .WillOnce(Return(Keypad::ButtonID::NUM1))   // Press '1'
      .WillOnce(Return(Keypad::ButtonID::HASH));  // Press '#'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(false))  // don't hold 'A'
      .WillOnce(Return(false))  // don't hold '1'
      .WillOnce(Return(true));  // hold '#' (end sequence)

  ASSERT_EQ(collectKeypadSequence(), "A1");
}

TEST_F(KeyPressCollection, holdingLetterEndsSequenceWithoutModification) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::A))     // Press 'A'
      .WillOnce(Return(Keypad::ButtonID::NUM1))  // Press '1'
      .WillOnce(Return(Keypad::ButtonID::A));    // Press 'A'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(false))  // don't hold 'A'
      .WillOnce(Return(false))  // don't hold '1'
      .WillOnce(Return(true));  // hold 'A' (end sequence)

  ASSERT_EQ(collectKeypadSequence(), "A1");
}

TEST_F(KeyPressCollection, holdingNumberSetsBalancedLoadSequence) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::NUM1))   // Press '1'
      .WillOnce(Return(Keypad::ButtonID::NUM6));  // Press '6'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(false))  // don't hold '1'
      .WillOnce(Return(true));  // hold '6' (prepend & end sequence)

  ASSERT_EQ(collectKeypadSequence(), "ABC16");
}

TEST_F(KeyPressCollection, pressingStarRemovesPreviousInput) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::A))      // Press 'A'
      .WillOnce(Return(Keypad::ButtonID::B))      // Press 'B'
      .WillOnce(Return(Keypad::ButtonID::C))      // Press 'C'
      .WillOnce(Return(Keypad::ButtonID::NUM2))   // Press '2'
      .WillOnce(Return(Keypad::ButtonID::STAR))   // Press '*', erase
      .WillOnce(Return(Keypad::ButtonID::NUM1))   // Press '1'
      .WillOnce(Return(Keypad::ButtonID::NUM6))   // Press '6'
      .WillOnce(Return(Keypad::ButtonID::HASH));  // Press '#' (end sequence)

  ASSERT_EQ(collectKeypadSequence(), "ABC16");
}

TEST_F(KeyPressCollection, inputFollowedByHoldingStarCancelsSequence) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::A))      // Press 'A'
      .WillOnce(Return(Keypad::ButtonID::NUM1))   // Press '1'
      .WillOnce(Return(Keypad::ButtonID::STAR));  // Press '*'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(false))  // don't hold 'A'
      .WillOnce(Return(false))  // don't hold '1'
      .WillOnce(Return(true));  // hold '#' (cancel sequence)

  ASSERT_EQ(collectKeypadSequence().length(), 0);
}

TEST_F(KeyPressCollection,
       emptySequenceFollowedByHoldingStarAppliesResetSequence) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::STAR));  // Press '*'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(true));  // hold '*' (reset & end sequence)

  ASSERT_EQ(collectKeypadSequence(), "ABCD0");
}

TEST_F(KeyPressCollection, canApplyResetSequenceAfterErasingInputs) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::A))      // Press 'A'
      .WillOnce(Return(Keypad::ButtonID::NUM7))   // Press '7'
      .WillOnce(Return(Keypad::ButtonID::STAR))   // Press '*' (erase)
      .WillOnce(Return(Keypad::ButtonID::STAR))   // Press '*' (erase again)
      .WillOnce(Return(Keypad::ButtonID::STAR));  // Press '*' (reset)

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(false))  // don't hold 'A'
      .WillOnce(Return(false))  // don't hold '7'
      .WillOnce(Return(false))  // don't hold '*'
      .WillOnce(Return(false))  // don't hold '*'
      .WillOnce(Return(true));  // hold '*' (reset & end sequence)

  ASSERT_EQ(collectKeypadSequence(), "ABCD0");
}

class XBeeMock : public XBee {
 public:
  ~XBeeMock() override = default;

  MOCK_CONST_METHOD0(bytesAvailable, int());
  MOCK_CONST_METHOD0(hasBufferedData, bool());
  MOCK_CONST_METHOD0(clearBuffer, void());
  MOCK_CONST_METHOD0(readByte, int());
  MOCK_CONST_METHOD0(peekByte, int());
};

XBeeMock xbeeMock;
XBee* xbee = &xbeeMock;

TEST(PCInputCollection, collectSimpleSequence) {
  EXPECT_CALL(xbeeMock, hasBufferedData())
      .WillOnce(Return(true))
      .WillOnce(Return(true))
      .WillOnce(Return(true))
      .WillOnce(Return(true))
      .WillOnce(Return(true))
      .WillOnce(Return(true))
      .WillOnce(Return(false));

  EXPECT_CALL(xbeeMock, readByte())
      .WillOnce(Return(static_cast<int>('A')))
      .WillOnce(Return(static_cast<int>('1')))
      .WillOnce(Return(static_cast<int>('B')))
      .WillOnce(Return(static_cast<int>('2')))
      .WillOnce(Return(static_cast<int>('C')))
      .WillOnce(Return(static_cast<int>('3')));

  ASSERT_EQ(collectPCSequence(), "A1B2C3");
}

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

TEST(dSPACEInputAdditionCommandCollection, recordSimpleTransmission) {
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

TEST(dSPACEModifierCommandCollection, cancelEmptySequence) {
  InSequence s;
  // dSPACE transmission: (CANCEL)
  setExpectationsForSingleInput(21, 5, true);  // transmit CANCEL, 5 times

  ASSERT_EQ(collectDSPACESequence().length(), 0);
}

TEST(dSPACEModifierCommandCollection, cancelNonEmptySequence) {
  InSequence s;
  // dSPACE transmission: AAAAAAAAAAAA00000000(CANCEL)
  setExpectationsForSingleInput(17, 12, false);  // transmit 'A', 12 times
  setExpectationsForSingleInput(0, 8, false);    // transmit '0', 8 times
  setExpectationsForSingleInput(21, 3, true);    // transmit CANCEL, 3 times

  ASSERT_EQ(collectDSPACESequence().length(), 0);
}

TEST(dSPACEModifierCommandCollection, backspaceErasesLastInput) {
  InSequence s;
  // dSPACE transmission: AAAAAAAAAAAA00000000(BACKSPACE)
  setExpectationsForSingleInput(17, 12, false);  // transmit 'A', 12 times
  setExpectationsForSingleInput(0, 8, false);    // transmit '0', 8 times
  setExpectationsForSingleInput(22, 3, true);    // transmit BACKSPACE, 3 times

  ASSERT_EQ(collectDSPACESequence(), "A");
}

TEST(dSPACEModifierCommandCollection, backspaceDoesNotModifyEmptySequence) {
  InSequence s;
  // dSPACE transmission: (BACKSPACE)
  setExpectationsForSingleInput(22, 3, true);  // transmit BACKSPACE, 3 times

  ASSERT_EQ(collectDSPACESequence().length(), 0);
}

TEST(dSPACEModifierCommandCollection, applyResetSequence) {
  InSequence s;
  // dSPACE transmission: (RESET)
  setExpectationsForSingleInput(23, 13, true);  // transmit RESET, 13 times

  ASSERT_EQ(collectDSPACESequence(), "ABCD0");
}

TEST(dSPACEModifierCommandCollection, resetSequenceOverridesPreviousInput) {
  InSequence s;
  // dSPACE transmission: AAAAAAAAAAAA00000000(RESET)
  setExpectationsForSingleInput(17, 12, false);  // transmit 'A', 12 times
  setExpectationsForSingleInput(0, 8, false);    // transmit '0', 8 times
  setExpectationsForSingleInput(23, 3, true);    // transmit RESET, 3 times

  ASSERT_EQ(collectDSPACESequence(), "ABCD0");
}

TEST(dSPACEModifierCommandCollection, handleUnknownCommand) {
  InSequence s;
  // dSPACE transmission: (UNKNOWN)
  setExpectationsForSingleInput(25, 5, true);  // transmit UNKNOWN, 5 times

  ASSERT_EQ(collectDSPACESequence(), "?");
}

TEST(dSPACEModifierCommandCollection, applyBalancedInputSequence) {
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
