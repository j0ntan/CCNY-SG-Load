#include <mocks/LCD.h>
#include <RelayState.h>
#include <InputSequence.h>
#include <utils/InputSequence.h>
#include <RelayStateGenerator.h>
#include <Display/MessagesList.h>
#include <stdint.h>

using namespace testing;

const uint8_t InputSequence::SIZE;
extern Display::LCD* lcd;

bool operator==(const RelayState& lhs, const RelayState& rhs) {
  return lhs.phaseA == rhs.phaseA && lhs.phaseB == rhs.phaseB &&
         lhs.phaseC == rhs.phaseC && lhs.DC == rhs.DC;
}

bool operator!=(const RelayState& lhs, const RelayState& rhs) {
  return !(lhs == rhs);
}

class ValidateSequence : public Test {
 public:
  NiceMock<LCDMock> lcdMock;
  void SetUp() final { lcd = &lcdMock; }
};

TEST_F(ValidateSequence, emptySequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(InputSequence{}));
}

TEST_F(ValidateSequence, simpleSequenceIsValid) {
  ASSERT_TRUE(isValidSequence(initialized_Sequence("A1")));
}

TEST_F(ValidateSequence, lowercaseSequenceIsValid) {
  ASSERT_TRUE(isValidSequence(initialized_Sequence("abc1")));
}

TEST_F(ValidateSequence, uppercaseSequenceIsValid) {
  ASSERT_TRUE(isValidSequence(initialized_Sequence("ABC1")));
}

TEST_F(ValidateSequence, mixedcaseSequenceIsValid) {
  ASSERT_TRUE(isValidSequence(initialized_Sequence("aBc1")));
}

TEST_F(ValidateSequence, invalidLetterSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("e1")));
  ASSERT_FALSE(isValidSequence(initialized_Sequence("Z1")));
}

TEST_F(ValidateSequence, invalidSymbolSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("a.1")));
  ASSERT_FALSE(isValidSequence(initialized_Sequence("a1!")));
  ASSERT_FALSE(isValidSequence(initialized_Sequence("_a1")));
}

TEST_F(ValidateSequence, noNumberSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("abc")));
}

TEST_F(ValidateSequence, noPhaseSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("123")));
}

TEST_F(ValidateSequence, numberFirstSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("1A")));
}

TEST_F(ValidateSequence, phaseLastSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("A1B")));
}

TEST_F(ValidateSequence, repeatedPhaseSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("A1BB2")));
}

TEST_F(ValidateSequence, phasesOutOfOrderSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("B1AC2")));
  ASSERT_FALSE(isValidSequence(initialized_Sequence("D1C1")));
}

TEST_F(ValidateSequence, leadingZeroSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("A01")));
}

TEST_F(ValidateSequence, outOfRangeNumbersSequenceIsNotValid) {
  ASSERT_TRUE(isValidSequence(initialized_Sequence("A10")));
  ASSERT_FALSE(isValidSequence(initialized_Sequence("A17")));
  ASSERT_FALSE(isValidSequence(initialized_Sequence("A101")));
  ASSERT_FALSE(isValidSequence(initialized_Sequence("D3")));
  ASSERT_FALSE(isValidSequence(initialized_Sequence("D12")));
}

TEST(recordNewState, simpleSequence) {
  RelayState s1{0, 0, 0, 0};
  recordNewRelayState(initialized_Sequence("A1"), s1);
  ASSERT_EQ(s1, (RelayState{1, 0, 0, 0}));
}

TEST(recordNewState, balancedSequence) {
  RelayState s1{0, 0, 0, 0};
  recordNewRelayState(initialized_Sequence("ABC5"), s1);
  ASSERT_EQ(s1, (RelayState{5, 5, 5, 0}));
}

TEST(recordNewState, unbalancedSequence) {
  RelayState s1{0, 0, 0, 0};
  recordNewRelayState(initialized_Sequence("A1B2C3D1"), s1);
  ASSERT_EQ(s1, (RelayState{1, 2, 3, 1}));
}

TEST(recordNewState, multipleSimpleSequences) {
  RelayState s1{0, 0, 0, 0};
  recordNewRelayState(initialized_Sequence("A1"), s1);
  ASSERT_EQ(s1, (RelayState{1, 0, 0, 0}));
  recordNewRelayState(initialized_Sequence("A8"), s1);
  ASSERT_EQ(s1, (RelayState{8, 0, 0, 0}));
  recordNewRelayState(initialized_Sequence("C6"), s1);
  ASSERT_EQ(s1, (RelayState{8, 0, 6, 0}));
  recordNewRelayState(initialized_Sequence("B10"), s1);
  ASSERT_EQ(s1, (RelayState{8, 10, 6, 0}));
}
