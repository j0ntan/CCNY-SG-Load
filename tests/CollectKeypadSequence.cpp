#include <mocks/Keypad.h>
#include <mocks/Timer.h>
#include <mocks/LCD.h>
#include <utils/InputSequence.h>
#include <InputSequence.h>
#include <Collect.h>
#include <memory>

using namespace ::testing;

extern Keypad* keypad;
extern Timer* timer;
extern Display::LCD* lcd;

class KeyPressCollection : public Test {
 public:
  std::unique_ptr<NiceMock<KeypadMock>> keypadMock =
      std::make_unique<NiceMock<KeypadMock>>();
  std::unique_ptr<NiceMock<TimerMock>> timerMock =
      std::make_unique<NiceMock<TimerMock>>();
  std::unique_ptr<NiceMock<LCDMock>> lcdMock =
      std::make_unique<NiceMock<LCDMock>>();

  void SetUp() final {
    keypad = keypadMock.get();
    timer = timerMock.get();
    lcd = lcdMock.get();
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
