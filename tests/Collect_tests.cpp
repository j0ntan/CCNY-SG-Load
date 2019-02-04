#include <gmock/gmock.h>
#include <Keypad_mock.h>
#include <Timer_mock.h>
#include <Collect.h>
#include <memory>
#include <string.h>
#include <string>

using namespace ::testing;

Keypad* keypad = nullptr;
Timer* timer = nullptr;

class KeyPressCollection : public Test {
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

TEST_F(KeyPressCollection, recordSimpleSequence) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::A))      // Press 'A'
      .WillOnce(Return(Keypad::ButtonID::B))      // Press 'B'
      .WillOnce(Return(Keypad::ButtonID::C))      // Press 'C'
      .WillOnce(Return(Keypad::ButtonID::NUM1))   // Press '1'
      .WillOnce(Return(Keypad::ButtonID::NUM6))   // Press '6'
      .WillOnce(Return(Keypad::ButtonID::HASH));  // Press '#' (end sequence)

  ASSERT_EQ(recordKeypadSequence(), fullACSequence);
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

  ASSERT_EQ(recordKeypadSequence(), "A1");
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

  ASSERT_EQ(recordKeypadSequence(), "A1");
}

TEST_F(KeyPressCollection, holdingNumberSetsBalancedLoadSequence) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::NUM1))   // Press '1'
      .WillOnce(Return(Keypad::ButtonID::NUM6));  // Press '6'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(false))  // don't hold '1'
      .WillOnce(Return(true));  // hold '6' (prepend & end sequence)

  ASSERT_EQ(recordKeypadSequence(), fullACSequence);
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

  ASSERT_EQ(recordKeypadSequence(), fullACSequence);
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

  ASSERT_EQ(emptySequence, recordKeypadSequence());
}

TEST_F(KeyPressCollection,
       emptySequenceFollowedByHoldingStarAppliesResetSequence) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::STAR));  // Press '*'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(true));  // hold '*' (reset & end sequence)

  ASSERT_EQ(recordKeypadSequence(), resetSequence);
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

  ASSERT_EQ(recordKeypadSequence(), resetSequence);
}

XBee* xbee = nullptr;  // unused, but needed to compile

class dSPACECollection : public Test {
 public:
  InputSequence actual, expected;
};

TEST_F(dSPACECollection, singleInputCommandAddNums0To16) {
  for (int command = 0; command <= 16; command++) {
    expected.addInput(std::to_string(command).c_str());
    helper::dSPACESingleInputCommand(command, actual);
    ASSERT_EQ(actual, expected);

    // clear sequences for next iteration
    actual.cancelSequence();
    expected.cancelSequence();
  }
}

TEST_F(dSPACECollection, singleInputCommandAddPhases) {
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

TEST_F(dSPACECollection, singleInputCommandAddNumAfterPhase) {
  expected.addInput("A0");
  helper::dSPACESingleInputCommand(17, actual);
  helper::dSPACESingleInputCommand(0, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(dSPACECollection, singleInputCommandAddPhaseAfterNum) {
  expected.addInput("8B");
  helper::dSPACESingleInputCommand(8, actual);
  helper::dSPACESingleInputCommand(18, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(dSPACECollection, singleInputCommandCancelSequence) {
  helper::dSPACESingleInputCommand(21, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(dSPACECollection, singleInputCommandCancelSequenceAfterInput) {
  helper::dSPACESingleInputCommand(17, actual);
  helper::dSPACESingleInputCommand(0, actual);
  helper::dSPACESingleInputCommand(21, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(dSPACECollection, singleInputCommandBackspaceAfterInput) {
  expected.addInput('A');
  helper::dSPACESingleInputCommand(17, actual);
  helper::dSPACESingleInputCommand(0, actual);
  helper::dSPACESingleInputCommand(22, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(dSPACECollection, singleInputCommandBackspaceEmptySequence) {
  helper::dSPACESingleInputCommand(22, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(dSPACECollection, singleInputCommandApplyResetSequence) {
  expected.addInput("ABCD0");
  helper::dSPACESingleInputCommand(23, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(dSPACECollection, singleInputCommandResetAfterInput) {
  expected.addInput("ABCD0");
  helper::dSPACESingleInputCommand(17, actual);
  helper::dSPACESingleInputCommand(0, actual);
  helper::dSPACESingleInputCommand(23, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(dSPACECollection, singleInputCommandHandleUnknownCommand) {
  expected.addInput('?');
  helper::dSPACESingleInputCommand(25, actual);
  ASSERT_EQ(actual, expected);
}

TEST_F(dSPACECollection, balancedInputCommandAddNums0To16) {
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
