#include <gmock/gmock.h>
#include <Keypad_mock.h>
#include <Timer_mock.h>
#define F(string_literal) string_literal
#include <Collect.h>
#include <memory>
#include <string.h>
#include <string>

using namespace ::testing;

Keypad* keypad = nullptr;
Timer* timer = nullptr;

class CollectKeypad : public Test {
 public:
  std::unique_ptr<NiceMock<KeypadMock>> keypadMock;
  std::unique_ptr<NiceMock<TimerMock>> timerMock;
  InputSequence emptySequence;
  InputSequence fullACSequence;
  InputSequence resetSequence;

  void SetUp() final {
    keypadMock = std::make_unique<NiceMock<KeypadMock>>();
    keypad = keypadMock.get();
    timerMock = std::make_unique<NiceMock<TimerMock>>();
    timer = timerMock.get();

    fullACSequence.addInput("ABC16");
    resetSequence.addInput("ABCD0");
  }
};

bool operator==(const InputSequence& lhs, const InputSequence& rhs) {
  if (lhs.length() == rhs.length()) {
    const uint8_t length = lhs.length();
    bool are_equal = true;
    for (uint8_t i = 0; i < length && are_equal; ++i) {
      are_equal = lhs[i] == rhs[i];
    }
    return are_equal;
  } else
    return false;
}

bool operator==(const InputSequence& lhs, const char* rhs) {
  InputSequence temp;
  temp.addInput(rhs);
  return lhs == temp;
}

TEST_F(CollectKeypad, holdHashDoesNothing) {
  InputSequence expected = fullACSequence;
  helper::actionOnButtonHold(fullACSequence, Keypad::ButtonID::HASH);
  ASSERT_EQ(fullACSequence, expected);
}

TEST_F(CollectKeypad, holdLetterDoesNothing) {
  InputSequence expected = fullACSequence;
  helper::actionOnButtonHold(fullACSequence, Keypad::ButtonID::A);
  ASSERT_EQ(fullACSequence, expected);
}

TEST_F(CollectKeypad, holdStarCancels) {
  helper::actionOnButtonHold(fullACSequence, Keypad::ButtonID::STAR);
  ASSERT_EQ(fullACSequence, emptySequence);
}

TEST_F(CollectKeypad, holdStarResets) {
  helper::actionOnButtonHold(emptySequence, Keypad::ButtonID::STAR);
  ASSERT_EQ(emptySequence, resetSequence);
}

TEST_F(CollectKeypad, holdNumPrepends3Phases) {
  helper::actionOnButtonHold(emptySequence, Keypad::ButtonID::NUM2);
  ASSERT_EQ(emptySequence, "ABC2");
}

TEST_F(CollectKeypad, recordSimpleSequence) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::A))      // Press 'A'
      .WillOnce(Return(Keypad::ButtonID::B))      // Press 'B'
      .WillOnce(Return(Keypad::ButtonID::C))      // Press 'C'
      .WillOnce(Return(Keypad::ButtonID::NUM1))   // Press '1'
      .WillOnce(Return(Keypad::ButtonID::NUM6))   // Press '6'
      .WillOnce(Return(Keypad::ButtonID::HASH));  // Press '#' (end sequence)

  ASSERT_EQ(recordKeypadSequence(), fullACSequence);
}

TEST_F(CollectKeypad, recordBalancedSequenceWithHold) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::NUM1))   // Press '1'
      .WillOnce(Return(Keypad::ButtonID::NUM6));  // Press '6'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(false))  // don't hold '1'
      .WillOnce(Return(true));  // hold '6' (prepend & end sequence)

  ASSERT_EQ(recordKeypadSequence(), fullACSequence);
}

TEST_F(CollectKeypad, recordSequenceHoldPhaseEndsSequence) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::A))     // Press 'A'
      .WillOnce(Return(Keypad::ButtonID::NUM1))  // Press '1'
      .WillOnce(Return(Keypad::ButtonID::B))     // Press 'B'
      .WillOnce(Return(Keypad::ButtonID::NUM2))  // Press '2'
      .WillOnce(Return(Keypad::ButtonID::C));    // Press 'C'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(false))  // don't hold 'A'
      .WillOnce(Return(false))  // don't hold '1'
      .WillOnce(Return(false))  // don't hold 'B'
      .WillOnce(Return(false))  // don't hold '2'
      .WillOnce(Return(true));  // hold 'C' (end sequence)

  ASSERT_EQ(recordKeypadSequence(), "A1B2");
}

TEST_F(CollectKeypad, recordSequenceEraseOnce) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::A))      // Press 'A'
      .WillOnce(Return(Keypad::ButtonID::B))      // Press 'B'
      .WillOnce(Return(Keypad::ButtonID::C))      // Press 'C'
      .WillOnce(Return(Keypad::ButtonID::NUM2))   // Press '2'
      .WillOnce(Return(Keypad::ButtonID::STAR))   // Press '*', erase
      .WillOnce(Return(Keypad::ButtonID::NUM1))   // Press '1'
      .WillOnce(Return(Keypad::ButtonID::NUM6))   // Press '6'
      .WillOnce(Return(Keypad::ButtonID::HASH));  // Press '#' (end sequence)

  ASSERT_EQ(recordKeypadSequence(), fullACSequence);
}

TEST_F(CollectKeypad, recordSequenceWithCancel) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::A))      // Press 'A'
      .WillOnce(Return(Keypad::ButtonID::NUM3))   // Press '3'
      .WillOnce(Return(Keypad::ButtonID::STAR));  // Press '*'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(false))  // don't hold 'A'
      .WillOnce(Return(false))  // don't hold '3'
      .WillOnce(Return(true));  // hold '*' (cancel & end sequence)

  ASSERT_EQ(recordKeypadSequence(), emptySequence);
}

TEST_F(CollectKeypad, recordSequenceReset) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::STAR));  // Press '*'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(true));  // hold '*' (reset & end sequence)

  ASSERT_EQ(recordKeypadSequence(), resetSequence);
}

TEST_F(CollectKeypad, recordSequenceEraseAllThenReset) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::A))      // Press 'A'
      .WillOnce(Return(Keypad::ButtonID::NUM7))   // Press '7'
      .WillOnce(Return(Keypad::ButtonID::STAR))   // Press '*' (erase)
      .WillOnce(Return(Keypad::ButtonID::STAR))   // Press '*' (erase again)
      .WillOnce(Return(Keypad::ButtonID::STAR));  // Press '*'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(false))  // don't hold 'A'
      .WillOnce(Return(false))  // don't hold '7'
      .WillOnce(Return(false))  // don't hold '*'
      .WillOnce(Return(false))  // don't hold '*'
      .WillOnce(Return(true));  // hold '*' (reset & end sequence)

  ASSERT_EQ(recordKeypadSequence(), resetSequence);
}

XBee* xbee = nullptr;  // unused, but needed to compile

class CollectDSPACE : public Test {
 public:
  InputSequence actual, expected;
};

TEST_F(CollectDSPACE, singleInputCommandAddNums0To16) {
  for (int command = 0; command <= 16; command++) {
    expected.addInput(std::to_string(command).c_str());
    helper::dSPACESingleInputCommand(command, actual);
    ASSERT_EQ(actual, expected);

    // clear sequences for next iteration
    actual.cancelSequence();
    expected.cancelSequence();
  }
}

TEST_F(CollectDSPACE, singleInputCommandAddPhases) {
  expected.addInput('A');
  helper::dSPACESingleInputCommand(17, actual);
  ASSERT_EQ(actual, expected);
  actual.cancelSequence();
  expected.cancelSequence();

  expected.addInput('B');
  helper::dSPACESingleInputCommand(18, actual);
  ASSERT_EQ(actual, expected);
  actual.cancelSequence();
  expected.cancelSequence();

  expected.addInput('C');
  helper::dSPACESingleInputCommand(19, actual);
  ASSERT_EQ(actual, expected);
  actual.cancelSequence();
  expected.cancelSequence();

  expected.addInput('D');
  helper::dSPACESingleInputCommand(20, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(CollectDSPACE, singleInputCommandAddNumAfterPhase) {
  expected.addInput("A0");
  helper::dSPACESingleInputCommand(17, actual);
  helper::dSPACESingleInputCommand(0, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(CollectDSPACE, singleInputCommandAddPhaseAfterNum) {
  expected.addInput("8B");
  helper::dSPACESingleInputCommand(8, actual);
  helper::dSPACESingleInputCommand(18, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(CollectDSPACE, singleInputCommandCancelSequence) {
  helper::dSPACESingleInputCommand(21, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(CollectDSPACE, singleInputCommandCancelSequenceAfterInput) {
  helper::dSPACESingleInputCommand(17, actual);
  helper::dSPACESingleInputCommand(0, actual);
  helper::dSPACESingleInputCommand(21, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(CollectDSPACE, singleInputCommandBackspaceAfterInput) {
  expected.addInput('A');
  helper::dSPACESingleInputCommand(17, actual);
  helper::dSPACESingleInputCommand(0, actual);
  helper::dSPACESingleInputCommand(22, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(CollectDSPACE, singleInputCommandBackspaceEmptySequence) {
  helper::dSPACESingleInputCommand(22, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(CollectDSPACE, singleInputCommandApplyResetSequence) {
  expected.addInput("ABCD0");
  helper::dSPACESingleInputCommand(23, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(CollectDSPACE, singleInputCommandResetAfterInput) {
  expected.addInput("ABCD0");
  helper::dSPACESingleInputCommand(17, actual);
  helper::dSPACESingleInputCommand(0, actual);
  helper::dSPACESingleInputCommand(23, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(CollectDSPACE, singleInputCommandHandleUnknownCommand) {
  expected.addInput('?');
  helper::dSPACESingleInputCommand(25, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(CollectDSPACE, balancedInputCommandAddNums0To16) {
  const int MODE_BEGINS = 29, MODE_ENDS = 45;
  for (int command = MODE_BEGINS; command <= MODE_ENDS; command++) {
    expected.addInput(("ABC" + std::to_string(command - MODE_BEGINS)).c_str());
    helper::dSPACEBalancedInputCommand(command, actual);
    ASSERT_EQ(actual, expected);

    // clear sequence for next iteration
    actual.cancelSequence();
    expected.cancelSequence();
  }
}

TEST(dSPACELoadProfile, createFilename0to999) {
  for (unsigned int number = 0; number < 1000; number++) {
    char filename[12] = {"PRFL000.txt"};
    createFilename(number, filename);
    std::string expected{"PRFL"};
    if (number < 10)
      expected += "00";
    else if (number < 100)
      expected += "0";
    expected += std::to_string(number) + ".txt";

    ASSERT_EQ(0, strcmp(filename, expected.c_str()));
  }
}
