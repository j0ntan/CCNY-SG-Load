#include <gmock/gmock.h>
#include <InputSequence.h>
#include <vector>
#include <numeric>
#include <algorithm>

TEST(validateInputSequence, isInitiallyEmpty) {
  InputSequence inputSequence;
  ASSERT_EQ(inputSequence.length(), 0);
}

TEST(validateInputSequence, canAddToEmptySequence) {
  InputSequence inputSequence;
  inputSequence.addInput('a');
}

TEST(validateInputSequence, addingInputIncreasesLength) {
  InputSequence inputSequence;
  for (uint8_t i = 0, count = 0; i < 5; ++i) {
    inputSequence.addInput('a');
    ASSERT_EQ(inputSequence.length(), ++count);
  }
}

TEST(validateInputSequence, addingMoreThan12InputsDoesNotFail) {
  InputSequence inputSequence;
  for (int i = 0; i < 15; ++i) inputSequence.addInput('a');
}

TEST(validateInputSequence, addingInputStringIncreasesLength) {
  InputSequence inputSequence;
  ASSERT_EQ(0, inputSequence.length());
  inputSequence.addInput("abc");
  ASSERT_EQ(3, inputSequence.length());
}

TEST(validateInputSequence, canAccessAndMatchIndeces0To10) {
  InputSequence inputSequence;
  for (uint8_t i = 0; i < 11; ++i) {
    const char input = static_cast<char>('a' + i);
    inputSequence.addInput(input);
    ASSERT_EQ(input, inputSequence[i]);
  }
}

TEST(validateInputSequence, canModifyAtIndeces0To10) {
  InputSequence inputSequence;
  std::vector<char> inputs(11);
  std::iota(inputs.begin(), inputs.end(), '1');
  std::random_shuffle(inputs.begin(), inputs.end());

  for (uint8_t i = 0; i < 11; ++i) inputSequence.addInput('0');

  for (uint8_t i = 0; i < 11; ++i) {
    ASSERT_EQ('0', inputSequence[i]);
    inputSequence[i] = inputs[i];
    ASSERT_EQ(inputs[i], inputSequence[i]);
  }
}

TEST(validateInputSequence, accessIndexOfEmptySeqDoesNotFail) {
  InputSequence inputSequence;
  inputSequence[0];   // smallest index
  inputSequence[11];  // largest index
  inputSequence[12];  // above largest index
}

TEST(validateInputSequence, indexOfEmptySeqModifiesFirstInput) {
  InputSequence inputSequence;

  ASSERT_EQ(0, inputSequence.length());
  ASSERT_EQ('\0', inputSequence[5]);

  inputSequence[5] = 'z';

  ASSERT_EQ(1, inputSequence.length());
  ASSERT_EQ('z', inputSequence[0]);
  ASSERT_EQ(inputSequence[0], inputSequence[5]);
}

TEST(validateInputSequence, accessIndexGreaterThamLengthDoesNotFail) {
  InputSequence inputSequence;
  inputSequence.addInput('a');
  inputSequence.addInput('a');
  inputSequence.addInput('a');
  inputSequence[5];
}

TEST(validateInputSequence, invalidIndexReturnsFirstChar) {
  InputSequence inputSequence;
  inputSequence.addInput('a');
  inputSequence.addInput('b');
  inputSequence.addInput('c');
  ASSERT_EQ('a', inputSequence[5]);

  inputSequence[5] = 'z';
  ASSERT_EQ('z', inputSequence[5]);
}

TEST(validateInputSequence, adding12thOrMoreInputOverwritesLastInput) {
  InputSequence inputSequence;
  for (int i = 0; i < 11; ++i)
    inputSequence.addInput(static_cast<char>('a' + i));

  ASSERT_EQ('k', inputSequence[10]);
  for (int i = 0; i < 3; ++i) {
    const char new_letter = static_cast<char>('a' + 5 + i);
    inputSequence.addInput(new_letter);
    ASSERT_EQ(11, inputSequence.length());
    ASSERT_EQ(new_letter, inputSequence[10]);
  }
}

TEST(validateInputSequence, addingTooLargeInputStringIsPartialAddition) {
  InputSequence inputSequence;
  for (int i = 0; i < 9; ++i) inputSequence.addInput('a');
  inputSequence.addInput("dogs");
  ASSERT_EQ(11, inputSequence.length());
  ASSERT_EQ('d', inputSequence[9]);
  ASSERT_EQ('o', inputSequence[10]);
}

TEST(validateInputSequence, addingInputStringToFullSeqDoesNotModifyIt) {
  InputSequence inputSequence;
  for (int i = 0; i < 11; ++i)
    inputSequence.addInput(static_cast<char>('a' + i));
  inputSequence.addInput("doesNotFit");
  ASSERT_EQ(11, inputSequence.length());
  for (int i = 0; i < 11; ++i)
    ASSERT_EQ(static_cast<char>('a' + i), inputSequence[i]);
}

TEST(validateInputSequence, removeLastInputDecreasesLength) {
  InputSequence inputSequence;
  inputSequence.addInput('a');
  inputSequence.addInput('b');
  inputSequence.addInput('c');
  ASSERT_EQ(3, inputSequence.length());
  inputSequence.removeLastInput();
  ASSERT_EQ(2, inputSequence.length());
}

TEST(validateInputSequence, removeLastInputDoesRemove) {
  InputSequence inputSequence;
  inputSequence.addInput('a');
  inputSequence.addInput('b');
  inputSequence.addInput('c');
  ASSERT_EQ('c', inputSequence[inputSequence.length() - 1]);
  inputSequence.removeLastInput();
  ASSERT_EQ('b', inputSequence[inputSequence.length() - 1]);
}

TEST(validateInputSequence, removeLastInputOfEmptySequenceDoesNotFail) {
  InputSequence inputSequence;
  ASSERT_EQ(0, inputSequence.length());
  inputSequence.removeLastInput();
  ASSERT_EQ(0, inputSequence.length());
}

TEST(validateInputSequence, canCancelSequenceInDifferentStates) {
  InputSequence emptySequence;
  emptySequence.cancelSequence();

  InputSequence notFullSequence;
  notFullSequence.addInput('a');
  notFullSequence.addInput('a');
  notFullSequence.addInput('a');
  notFullSequence.cancelSequence();

  InputSequence fullSequence;
  for (int i = 0; i < 12; ++i) fullSequence.addInput('a');
  fullSequence.cancelSequence();
}

TEST(validateInputSequence, canceledSequenceRemovesAllInputs) {
  InputSequence inputSequence;
  inputSequence.addInput('a');
  inputSequence.addInput('b');
  inputSequence.addInput('c');

  ASSERT_NE(0, inputSequence.length());
  inputSequence.cancelSequence();
  ASSERT_EQ(0, inputSequence.length());
}

TEST(validateInputSequence, canApplyResetSequenceToSeqInDifferentStates) {
  InputSequence emptySequence;
  emptySequence.applyResetSequence();

  InputSequence notFullSequence;
  notFullSequence.addInput('a');
  notFullSequence.addInput('a');
  notFullSequence.addInput('a');
  notFullSequence.applyResetSequence();

  InputSequence fullSequence;
  for (int i = 0; i < 12; ++i) fullSequence.addInput('a');
  fullSequence.applyResetSequence();
}

TEST(validateInputSequence, resetSequenceHasCorrectInputs) {
  InputSequence inputSequence;
  for (int i = 0; i < 12; ++i) inputSequence.addInput('a');

  inputSequence.applyResetSequence();
  ASSERT_EQ(5, inputSequence.length());
  ASSERT_EQ('A', inputSequence[0]);
  ASSERT_EQ('B', inputSequence[1]);
  ASSERT_EQ('C', inputSequence[2]);
  ASSERT_EQ('D', inputSequence[3]);
  ASSERT_EQ('0', inputSequence[4]);
}

TEST(validateInputSequence, canApplyBalancedSequenceToSeqInDifferentStates) {
  InputSequence emptySequence;
  emptySequence.applyBalancedSequence(0);

  InputSequence notFullSequence;
  notFullSequence.addInput('a');
  notFullSequence.addInput('a');
  notFullSequence.addInput('a');
  notFullSequence.applyBalancedSequence(0);

  InputSequence fullSequence;
  for (int i = 0; i < 12; ++i) fullSequence.addInput('a');
  fullSequence.applyBalancedSequence(0);
}

TEST(validateInputSequence, applyBalancedSequenceWithSingleDigit) {
  InputSequence inputSequence;
  inputSequence.applyBalancedSequence(4);
  ASSERT_EQ(4, inputSequence.length());
  ASSERT_EQ('A', inputSequence[0]);
  ASSERT_EQ('B', inputSequence[1]);
  ASSERT_EQ('C', inputSequence[2]);
  ASSERT_EQ('4', inputSequence[3]);
}

TEST(validateInputSequence, applyBalancedSequenceWithDoubleDigit) {
  InputSequence inputSequence;
  inputSequence.applyBalancedSequence(14);
  ASSERT_EQ(5, inputSequence.length());
  ASSERT_EQ('A', inputSequence[0]);
  ASSERT_EQ('B', inputSequence[1]);
  ASSERT_EQ('C', inputSequence[2]);
  ASSERT_EQ('1', inputSequence[3]);
  ASSERT_EQ('4', inputSequence[4]);
}

TEST(validateInputSequence, balancedSequenceLimitedTo99) {
  InputSequence inputSequence;
  inputSequence.applyBalancedSequence(123);
  ASSERT_EQ(5, inputSequence.length());
  ASSERT_EQ('A', inputSequence[0]);
  ASSERT_EQ('B', inputSequence[1]);
  ASSERT_EQ('C', inputSequence[2]);
  ASSERT_EQ('9', inputSequence[3]);
  ASSERT_EQ('9', inputSequence[4]);
}

TEST(validateInputSequence, canCopyConstructAndAssign) {
  InputSequence cat;
  cat.addInput('c');
  cat.addInput('a');
  cat.addInput('t');
  InputSequence copy_cat{cat};
  ASSERT_EQ(cat[0], copy_cat[0]);
  ASSERT_EQ(cat[1], copy_cat[1]);
  ASSERT_EQ(cat[2], copy_cat[2]);

  InputSequence dog;
  dog.addInput('d');
  dog.addInput('o');
  dog.addInput('g');
  dog = cat;
  ASSERT_EQ(cat[0], dog[0]);
  ASSERT_EQ(cat[1], dog[1]);
  ASSERT_EQ(cat[2], dog[2]);
}
