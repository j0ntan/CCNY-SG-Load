#include <gmock/gmock.h>
#include <Arduino_mock.h>
#include <Keypad_mock.h>
#include <StringDouble.h>
#include <Collect.h>
#include <memory>

using namespace ::testing;

Arduino* arduino = nullptr;
Keypad* keypad = nullptr;

class CollectKeypad : public Test {
 public:
  std::unique_ptr<NiceMock<ArduinoMock>> arduinoMock;
  std::unique_ptr<NiceMock<KeypadMock>> keypadMock;
  StringDouble emptySequence;
  StringDouble fullACSequence{"ABC16"};
  StringDouble resetSequence{"ABCD0"};

  void SetUp() final {
    arduinoMock = std::make_unique<NiceMock<ArduinoMock>>();
    arduino = arduinoMock.get();
    keypadMock = std::make_unique<NiceMock<KeypadMock>>();
    keypad = keypadMock.get();
  }
};

TEST_F(CollectKeypad, eraseLastInputInSequence) {
  helper::eraseLastInSequence(fullACSequence);
  ASSERT_EQ(fullACSequence, "ABC1");
}

TEST_F(CollectKeypad, noChangeOnEraseEmptySequence) {
  StringDouble pre_erase{emptySequence};
  helper::eraseLastInSequence(emptySequence);
  StringDouble post_erase{emptySequence};
  ASSERT_EQ(pre_erase, post_erase);
}

TEST_F(CollectKeypad, addCorrespondingCharToSequence) {
  const Keypad::Button BUTTONS[] = {
      Keypad::Button::NUM0, Keypad::Button::NUM1,    Keypad::Button::NUM2,
      Keypad::Button::NUM3, Keypad::Button::NUM4,    Keypad::Button::NUM5,
      Keypad::Button::NUM6, Keypad::Button::NUM7,    Keypad::Button::NUM8,
      Keypad::Button::NUM9, Keypad::Button::A,       Keypad::Button::B,
      Keypad::Button::C,    Keypad::Button::D,       Keypad::Button::STAR,
      Keypad::Button::HASH, Keypad::Button::MULTIPLE};
  const char* EQUIVALENT[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8",
                              "9", "A", "B", "C", "D", "",  "",  ""};

  for (int i = 0; i < 17; i++) {
    emptySequence.remove(0);
    helper::addCharToSequence(emptySequence, BUTTONS[i]);
    EXPECT_EQ(emptySequence, EQUIVALENT[i]);
  }
}

TEST_F(CollectKeypad, cancelSequence) {
  helper::cancelSequence(fullACSequence);
  ASSERT_EQ(fullACSequence, emptySequence);
}

TEST_F(CollectKeypad, resetSequence) {
  helper::applyResetSequence(emptySequence);
  ASSERT_EQ(emptySequence, resetSequence);
}

TEST_F(CollectKeypad, holdHashDoesNothing) {
  StringDouble expected = fullACSequence;
  helper::actionOnButtonHold(fullACSequence, Keypad::Button::HASH);
  ASSERT_EQ(fullACSequence, expected);
}

TEST_F(CollectKeypad, holdLetterDoesNothing) {
  StringDouble expected = fullACSequence;
  helper::actionOnButtonHold(fullACSequence, Keypad::Button::A);
  ASSERT_EQ(fullACSequence, expected);
}

TEST_F(CollectKeypad, holdStarCancels) {
  helper::actionOnButtonHold(fullACSequence, Keypad::Button::STAR);
  ASSERT_EQ(fullACSequence, emptySequence);
}

TEST_F(CollectKeypad, holdStarResets) {
  helper::actionOnButtonHold(emptySequence, Keypad::Button::STAR);
  ASSERT_EQ(emptySequence, resetSequence);
}

TEST_F(CollectKeypad, holdNumPrepends3Phases) {
  helper::actionOnButtonHold(emptySequence, Keypad::Button::NUM2);
  ASSERT_EQ(emptySequence, "ABC2");
}

TEST_F(CollectKeypad, recordSimpleSequence) {
  EXPECT_CALL(*keypadMock, getButton())
      .WillOnce(Return(Keypad::Button::A))      // Press 'A'
      .WillOnce(Return(Keypad::Button::B))      // Press 'B'
      .WillOnce(Return(Keypad::Button::C))      // Press 'C'
      .WillOnce(Return(Keypad::Button::NUM1))   // Press '1'
      .WillOnce(Return(Keypad::Button::NUM6))   // Press '6'
      .WillOnce(Return(Keypad::Button::HASH));  // Press '#' (end sequence)

  ASSERT_EQ(recordKeypadSequence<StringDouble>(), fullACSequence);
}

TEST_F(CollectKeypad, recordBalancedSequenceWithHold) {
  EXPECT_CALL(*keypadMock, getButton())
      .WillOnce(Return(Keypad::Button::NUM1))   // Press '1'
      .WillOnce(Return(Keypad::Button::NUM6));  // Press '6'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(false))  // don't hold '1'
      .WillOnce(Return(true));  // hold '6' (prepend & end sequence)

  ASSERT_EQ(recordKeypadSequence<StringDouble>(), fullACSequence);
}

TEST_F(CollectKeypad, recordSequenceHoldPhaseEndsSequence) {
  EXPECT_CALL(*keypadMock, getButton())
      .WillOnce(Return(Keypad::Button::A))     // Press 'A'
      .WillOnce(Return(Keypad::Button::NUM1))  // Press '1'
      .WillOnce(Return(Keypad::Button::B))     // Press 'B'
      .WillOnce(Return(Keypad::Button::NUM2))  // Press '2'
      .WillOnce(Return(Keypad::Button::C));    // Press 'C'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(false))  // don't hold 'A'
      .WillOnce(Return(false))  // don't hold '1'
      .WillOnce(Return(false))  // don't hold 'B'
      .WillOnce(Return(false))  // don't hold '2'
      .WillOnce(Return(true));  // hold 'C' (end sequence)

  ASSERT_EQ(recordKeypadSequence<StringDouble>(), "A1B2");
}

TEST_F(CollectKeypad, recordSequenceEraseOnce) {
  EXPECT_CALL(*keypadMock, getButton())
      .WillOnce(Return(Keypad::Button::A))      // Press 'A'
      .WillOnce(Return(Keypad::Button::B))      // Press 'B'
      .WillOnce(Return(Keypad::Button::C))      // Press 'C'
      .WillOnce(Return(Keypad::Button::NUM2))   // Press '2'
      .WillOnce(Return(Keypad::Button::STAR))   // Press '*', erase
      .WillOnce(Return(Keypad::Button::NUM1))   // Press '1'
      .WillOnce(Return(Keypad::Button::NUM6))   // Press '6'
      .WillOnce(Return(Keypad::Button::HASH));  // Press '#' (end sequence)

  ASSERT_EQ(recordKeypadSequence<StringDouble>(), fullACSequence);
}

TEST_F(CollectKeypad, recordSequenceWithCancel) {
  EXPECT_CALL(*keypadMock, getButton())
      .WillOnce(Return(Keypad::Button::A))      // Press 'A'
      .WillOnce(Return(Keypad::Button::NUM3))   // Press '3'
      .WillOnce(Return(Keypad::Button::STAR));  // Press '*'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(false))  // don't hold 'A'
      .WillOnce(Return(false))  // don't hold '3'
      .WillOnce(Return(true));  // hold '*' (cancel & end sequence)

  ASSERT_EQ(recordKeypadSequence<StringDouble>(), emptySequence);
}

TEST_F(CollectKeypad, recordSequenceReset) {
  EXPECT_CALL(*keypadMock, getButton())
      .WillOnce(Return(Keypad::Button::STAR));  // Press '*'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(true));  // hold '*' (reset & end sequence)

  ASSERT_EQ(recordKeypadSequence<StringDouble>(), resetSequence);
}

TEST_F(CollectKeypad, recordSequenceEraseAllThenReset) {
  EXPECT_CALL(*keypadMock, getButton())
      .WillOnce(Return(Keypad::Button::A))      // Press 'A'
      .WillOnce(Return(Keypad::Button::NUM7))   // Press '7'
      .WillOnce(Return(Keypad::Button::STAR))   // Press '*' (erase)
      .WillOnce(Return(Keypad::Button::STAR))   // Press '*' (erase again)
      .WillOnce(Return(Keypad::Button::STAR));  // Press '*'

  EXPECT_CALL(*keypadMock, anyButtonHeld())
      .WillOnce(Return(false))  // don't hold 'A'
      .WillOnce(Return(false))  // don't hold '7'
      .WillOnce(Return(false))  // don't hold '*'
      .WillOnce(Return(false))  // don't hold '*'
      .WillOnce(Return(true));  // hold '*' (reset & end sequence)

  ASSERT_EQ(recordKeypadSequence<StringDouble>(), resetSequence);
}
