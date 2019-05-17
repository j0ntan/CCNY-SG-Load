#include <gtest/gtest.h>
#include <RelayState.h>
#include <InputSequence.h>
#include "InputSequence_utils.h"
#include <RelayStateGenerator.h>

bool operator==(const RelayState& lhs, const RelayState& rhs) {
  return lhs.phaseA == rhs.phaseA && lhs.phaseB == rhs.phaseB &&
         lhs.phaseC == rhs.phaseC && lhs.DC == rhs.DC;
}

bool operator!=(const RelayState& lhs, const RelayState& rhs) {
  return !(lhs == rhs);
}

TEST(ValidateSequence, emptySequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(InputSequence{}));
}

TEST(ValidateSequence, simpleSequenceIsValid) {
  ASSERT_TRUE(isValidSequence(initialized_Sequence("A1")));
}

TEST(ValidateSequence, lowercaseSequenceIsValid) {
  ASSERT_TRUE(isValidSequence(initialized_Sequence("abc1")));
}

TEST(ValidateSequence, uppercaseSequenceIsValid) {
  ASSERT_TRUE(isValidSequence(initialized_Sequence("ABC1")));
}

TEST(ValidateSequence, mixedcaseSequenceIsValid) {
  ASSERT_TRUE(isValidSequence(initialized_Sequence("aBc1")));
}

TEST(ValidateSequence, invalidLetterSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("e1")));
  ASSERT_FALSE(isValidSequence(initialized_Sequence("Z1")));
}

TEST(ValidateSequence, invalidSymbolSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("a.1")));
  ASSERT_FALSE(isValidSequence(initialized_Sequence("a1!")));
  ASSERT_FALSE(isValidSequence(initialized_Sequence("_a1")));
}

TEST(ValidateSequence, noNumberSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("abc")));
}

TEST(ValidateSequence, noPhaseSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("123")));
}

TEST(ValidateSequence, numberFirstSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("1A")));
}

TEST(ValidateSequence, phaseLastSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("A1B")));
}

TEST(ValidateSequence, repeatedPhaseSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("A1BB2")));
}

TEST(ValidateSequence, phasesOutOfOrderSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("B1AC2")));
  ASSERT_FALSE(isValidSequence(initialized_Sequence("D1C1")));
}

TEST(ValidateSequence, leadingZeroSequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(initialized_Sequence("A01")));
}

TEST(ValidateSequence, outOfRangeNumbersSequenceIsNotValid) {
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
