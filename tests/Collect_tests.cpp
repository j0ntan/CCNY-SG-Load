#include <gmock/gmock.h>
#include <Keypad_mock.h>
#include <Timer_mock.h>
#include <InputSequence.h>
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

  ASSERT_EQ(collectKeypadSequence(), fullACSequence);
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

  ASSERT_EQ(collectKeypadSequence(), fullACSequence);
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

  ASSERT_EQ(collectKeypadSequence(), fullACSequence);
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

  ASSERT_EQ(emptySequence, collectKeypadSequence());
}

TEST_F(KeyPressCollection,
       emptySequenceFollowedByHoldingStarAppliesResetSequence) {
  EXPECT_CALL(*keypadMock, getButtonID())
      .WillOnce(Return(Keypad::ButtonID::STAR));  // Press '*'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(true));  // hold '*' (reset & end sequence)

  ASSERT_EQ(collectKeypadSequence(), resetSequence);
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

  ASSERT_EQ(collectKeypadSequence(), resetSequence);
}

/*
XBee* xbee = nullptr;  // unused, but needed to compile

TEST(dSPACEInputAdditionCommandCollection, addNums0To16ToSequence) {
  for (int command = 0; command <= 16; command++) {
    const std::string cmd_str = std::to_string(command);
    InputSequence sequence;

    helper::dSPACESingleInputCommand(command, sequence);
    ASSERT_EQ(sequence, cmd_str.c_str());
  }
}

TEST(dSPACEInputAdditionCommandCollection, addEachPhaseToSequence) {
  const char phases[] = {'A', 'B', 'C', 'D'};
  for (int i = 0; i < 4; ++i) {
    const std::string phase_str(1, phases[i]);
    const int command = 17 + i;
    InputSequence sequence;

    helper::dSPACESingleInputCommand(command, sequence);
    ASSERT_EQ(sequence, phase_str.c_str());
  }
}

TEST(dSPACEInputAdditionCommandCollection, addPhaseThenNumToSequence) {
  const int command_add_phaseA = 17;
  const int command_add_num0 = 0;
  InputSequence sequence;

  helper::dSPACESingleInputCommand(command_add_phaseA, sequence);
  helper::dSPACESingleInputCommand(command_add_num0, sequence);
  ASSERT_EQ(sequence, "A0");
}

TEST(dSPACEInputAdditionCommandCollection, addNumThenPhaseToSequence) {
  const int command_add_num8 = 8;
  const int command_add_phaseB = 18;
  InputSequence sequence;

  helper::dSPACESingleInputCommand(command_add_num8, sequence);
  helper::dSPACESingleInputCommand(command_add_phaseB, sequence);
  ASSERT_EQ(sequence, "8B");
}

TEST(dSPACEModifierCommandCollection, cancelEmptySequence) {
  const int command_cancel = 21;
  InputSequence sequence;

  helper::dSPACESingleInputCommand(command_cancel, sequence);
  ASSERT_EQ(sequence.length(), 0);
}

TEST(dSPACEModifierCommandCollection, cancelNonEmptySequence) {
  const int command_add_phaseA = 17;
  const int command_add_num0 = 0;
  const int command_cancel = 21;
  InputSequence sequence;

  helper::dSPACESingleInputCommand(command_add_phaseA, sequence);
  helper::dSPACESingleInputCommand(command_add_num0, sequence);
  helper::dSPACESingleInputCommand(command_cancel, sequence);
  ASSERT_EQ(sequence.length(), 0);
}

TEST(dSPACEModifierCommandCollection, backspaceErasesLastInput) {
  const int command_add_phaseA = 17;
  const int command_add_num0 = 0;
  const int command_backspace = 22;
  InputSequence sequence;

  helper::dSPACESingleInputCommand(command_add_phaseA, sequence);
  helper::dSPACESingleInputCommand(command_add_num0, sequence);
  helper::dSPACESingleInputCommand(command_backspace, sequence);
  ASSERT_EQ(sequence, "A");
}

TEST(dSPACEModifierCommandCollection, backspaceDoesNotModifyEmptySequence) {
  const int command_backspace = 22;
  InputSequence sequence;

  helper::dSPACESingleInputCommand(command_backspace, sequence);
  ASSERT_EQ(sequence.length(), 0);
}

TEST(dSPACEModifierCommandCollection, applyResetSequence) {
  const int command_reset = 23;
  InputSequence sequence;

  helper::dSPACESingleInputCommand(command_reset, sequence);
  ASSERT_EQ(sequence, "ABCD0");
}

TEST(dSPACEModifierCommandCollection, resetSequenceOverridesPreviousInput) {
  const int command_add_phaseA = 17;
  const int command_add_num0 = 0;
  const int command_reset = 23;
  InputSequence sequence;

  helper::dSPACESingleInputCommand(command_add_phaseA, sequence);
  helper::dSPACESingleInputCommand(command_add_num0, sequence);
  helper::dSPACESingleInputCommand(command_reset, sequence);
  ASSERT_EQ(sequence, "ABCD0");
}

TEST(dSPACEModifierCommandCollection, handleUnknownCommand) {
  const int command_unknown = 25;
  InputSequence sequence;

  helper::dSPACESingleInputCommand(command_unknown, sequence);
  ASSERT_EQ(sequence, "?");
}

TEST(dSPACEModifierCommandCollection, apply0To16balancedInputSequence) {
  const int MODE_BEGINS = 29;
  for (int i = 0; i <= 16; i++) {
    const int command = MODE_BEGINS + i;
    const std::string balanced_sequence = "ABC" + std::to_string(i);
    InputSequence sequence;

    helper::dSPACEBalancedInputCommand(command, sequence);
    ASSERT_EQ(sequence, balanced_sequence.c_str());
  }
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
*/
