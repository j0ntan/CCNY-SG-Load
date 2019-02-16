#include <gtest/gtest.h>
#include <InputSequence.h>
#include "InputSequence_utils.h"
#include <RelayStateGenerator.h>

TEST(ValidateSequence, emptySequenceIsNotValid) {
  ASSERT_FALSE(isValidSequence(InputSequence{}));
}

TEST(ValidateSequence, simpleSequenceIsValid) {
  ASSERT_TRUE(isValidSequence(initialized_Sequence("A1")));
}
