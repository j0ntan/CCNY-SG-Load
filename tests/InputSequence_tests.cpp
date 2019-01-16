#include <gmock/gmock.h>
#include <InputSequence.h>
#include <vector>
#include <numeric>
#include <algorithm>

TEST(testInputSequence, initiallyEmpty) {
  InputSequence inputSequence;
  ASSERT_EQ(inputSequence.length(), 0);
}

TEST(testInputSequence, canAddToEmptySequence) {
  InputSequence inputSequence;
  inputSequence.addInput('a');
}

TEST(testInputSequence, addingInputIncreasesLength) {
  InputSequence inputSequence;
  for (uint8_t i = 0, count = 0; i < 5; ++i) {
    inputSequence.addInput('a');
    ASSERT_EQ(inputSequence.length(), ++count);
  }
}

TEST(testInputSequence, addingMoreThan12InputsDoesNotFail) {
  InputSequence inputSequence;
  for (int i = 0; i < 15; ++i) inputSequence.addInput('a');
}

TEST(testInputSequence, addingInputStringIncreasesLength) {
  InputSequence inputSequence;
  ASSERT_EQ(0, inputSequence.length());
  inputSequence.addInput("abc");
  ASSERT_EQ(3, inputSequence.length());
}

TEST(testInputSequence, canAccessAndMatchIndeces0To11) {
  InputSequence inputSequence;
  for (uint8_t i = 0; i < 12; ++i) {
    const char input = static_cast<char>('a' + i);
    inputSequence.addInput(input);
    ASSERT_EQ(input, inputSequence[i]);
  }
}

TEST(testInputSequence, canModifyAtIndeces0To11) {
  InputSequence inputSequence;
  std::vector<char> inputs(12);
  std::iota(inputs.begin(), inputs.end(), '1');
  std::random_shuffle(inputs.begin(), inputs.end());

  for (uint8_t i = 0; i < 12; ++i) inputSequence.addInput('0');

  for (uint8_t i = 0; i < 12; ++i) {
    ASSERT_EQ('0', inputSequence[i]);
    inputSequence[i] = inputs[i];
    ASSERT_EQ(inputs[i], inputSequence[i]);
  }
}

TEST(testInputSequence, accessIndexOfEmptySeqDoesNotFail) {
  InputSequence inputSequence;
  inputSequence[0];   // smallest index
  inputSequence[11];  // largest index
  inputSequence[12];  // above largest index
}

TEST(testInputSequence, indexOfEmptySeqModifiesFirstInput) {
  InputSequence inputSequence;

  ASSERT_EQ(0, inputSequence.length());
  ASSERT_EQ('\0', inputSequence[5]);

  inputSequence[5] = 'z';

  ASSERT_EQ(1, inputSequence.length());
  ASSERT_EQ('z', inputSequence[0]);
  ASSERT_EQ(inputSequence[0], inputSequence[5]);
}

TEST(testInputSequence, accessIndexGreaterThamLengthDoesNotFail) {
  InputSequence inputSequence;
  inputSequence.addInput('a');
  inputSequence.addInput('a');
  inputSequence.addInput('a');
  inputSequence[5];
}

TEST(testInputSequence, invalidIndexReturnsFirstChar) {
  InputSequence inputSequence;
  inputSequence.addInput('a');
  inputSequence.addInput('b');
  inputSequence.addInput('c');
  ASSERT_EQ('a', inputSequence[5]);

  inputSequence[5] = 'z';
  ASSERT_EQ('z', inputSequence[5]);
}

TEST(testInputSequence, adding13thOrMoreInputOverwritesLastInput) {
  InputSequence inputSequence;
  for (int i = 0; i < 12; ++i)
    inputSequence.addInput(static_cast<char>('a' + i));

  ASSERT_EQ('l', inputSequence[11]);
  for (int i = 0; i < 3; ++i) {
    const char new_letter = static_cast<char>('a' + 5 + i);
    inputSequence.addInput(new_letter);
    ASSERT_EQ(12, inputSequence.length());
    ASSERT_EQ(new_letter, inputSequence[11]);
  }
}

TEST(testInputSequence, addingTooLargeInputStringIsPartialAddition) {
  InputSequence inputSequence;
  for (int i = 0; i < 10; ++i) inputSequence.addInput('a');
  inputSequence.addInput("dogs");
  ASSERT_EQ(12, inputSequence.length());
  ASSERT_EQ('d', inputSequence[10]);
  ASSERT_EQ('o', inputSequence[11]);
}

TEST(testInputSequence, addingInputStringToFullSeqDoesNotModifyIt) {
  InputSequence inputSequence;
  for (int i = 0; i < 12; ++i)
    inputSequence.addInput(static_cast<char>('a' + i));
  inputSequence.addInput("doesNotFit");
  ASSERT_EQ(12, inputSequence.length());
  for (int i = 0; i < 12; ++i)
    ASSERT_EQ(static_cast<char>('a' + i), inputSequence[i]);
}

TEST(testInputSequence, removeLastInputDecreasesLength) {
  InputSequence inputSequence;
  inputSequence.addInput('a');
  inputSequence.addInput('b');
  inputSequence.addInput('c');
  ASSERT_EQ(3, inputSequence.length());
  inputSequence.removeLastInput();
  ASSERT_EQ(2, inputSequence.length());
}

TEST(testInputSequence, removeLastInputDoesRemove) {
  InputSequence inputSequence;
  inputSequence.addInput('a');
  inputSequence.addInput('b');
  inputSequence.addInput('c');
  ASSERT_EQ('c', inputSequence[inputSequence.length() - 1]);
  inputSequence.removeLastInput();
  ASSERT_EQ('b', inputSequence[inputSequence.length() - 1]);
}

TEST(testInputSequence, removeLastInputOfEmptySequenceDoesNotFail) {
  InputSequence inputSequence;
  ASSERT_EQ(0, inputSequence.length());
  inputSequence.removeLastInput();
  ASSERT_EQ(0, inputSequence.length());
}

TEST(testInputSequence, canCancelSequenceInDifferentStates) {
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

TEST(testInputSequence, canceledSequenceRemovesAllInputs) {
  InputSequence inputSequence;
  inputSequence.addInput('a');
  inputSequence.addInput('b');
  inputSequence.addInput('c');

  ASSERT_NE(0, inputSequence.length());
  inputSequence.cancelSequence();
  ASSERT_EQ(0, inputSequence.length());
}

TEST(testInputSequence, canApplyResetSequenceToSeqInDifferentStates) {
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

TEST(testInputSequence, resetSequenceHasCorrectInputs) {
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

TEST(testInputSequence, canApplyBalancedSequenceToSeqInDifferentStates) {
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

TEST(testInputSequence, applyBalancedSequenceWithSingleDigit) {
  InputSequence inputSequence;
  inputSequence.applyBalancedSequence(4);
  ASSERT_EQ(4, inputSequence.length());
  ASSERT_EQ('A', inputSequence[0]);
  ASSERT_EQ('B', inputSequence[1]);
  ASSERT_EQ('C', inputSequence[2]);
  ASSERT_EQ('4', inputSequence[3]);
}

TEST(testInputSequence, applyBalancedSequenceWithDoubleDigit) {
  InputSequence inputSequence;
  inputSequence.applyBalancedSequence(14);
  ASSERT_EQ(5, inputSequence.length());
  ASSERT_EQ('A', inputSequence[0]);
  ASSERT_EQ('B', inputSequence[1]);
  ASSERT_EQ('C', inputSequence[2]);
  ASSERT_EQ('1', inputSequence[3]);
  ASSERT_EQ('4', inputSequence[4]);
}

TEST(testInputSequence, balancedSequenceLimitedTo99) {
  InputSequence inputSequence;
  inputSequence.applyBalancedSequence(123);
  ASSERT_EQ(5, inputSequence.length());
  ASSERT_EQ('A', inputSequence[0]);
  ASSERT_EQ('B', inputSequence[1]);
  ASSERT_EQ('C', inputSequence[2]);
  ASSERT_EQ('9', inputSequence[3]);
  ASSERT_EQ('9', inputSequence[4]);
}

TEST(testInputSequence, canCopyConstructAndAssign) {
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
