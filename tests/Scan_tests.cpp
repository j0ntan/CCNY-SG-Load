#include <gtest/gtest.h>
#include <Scan.h>

using namespace ::testing;

class ScanInput : public Test {
 public:
  TokenSet result;
};

bool operator==(const TokenSet& lhs, const Token rhs[]) {
  for (uint8_t i = 0; i < lhs.size(); i++)
    if (lhs[i] != rhs[i]) return false;
  return true;
}

bool operator==(const Token lhs[], const TokenSet& rhs) { return rhs == lhs; }

TEST_F(ScanInput, validateInputCharacters) {
  const char NUMERICAL_INPUT[] = {'0', '1', '2', '3', '4',
                                  '5', '6', '7', '8', '9'};
  const char UPPERCASE_LETTER_INPUT[] = {'A', 'B', 'C', 'D'};
  const char LOWERCASE_LETTER_INPUT[] = {'a', 'b', 'c', 'd'};
  const Token NUMERICAL_TOKENS[] = {
      Token::NUM0, Token::NUM1, Token::NUM2, Token::NUM3, Token::NUM4,
      Token::NUM5, Token::NUM6, Token::NUM7, Token::NUM8, Token::NUM9};
  const Token PHASE_TOKENS[] = {Token::AC_phaseA, Token::AC_phaseB,
                                Token::AC_phaseC, Token::DC};

  for (int i = 0; i < 5; i++) {
    InputSequence numerical_sequence;
    numerical_sequence.addInput(NUMERICAL_INPUT[i]);
    result = scan(numerical_sequence);
    ASSERT_FALSE(result.containsInvalid());
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0], NUMERICAL_TOKENS[i]);
  }
  for (int i = 0; i < 4; i++) {
    InputSequence uppercase_letter_sequence;
    uppercase_letter_sequence.addInput(UPPERCASE_LETTER_INPUT[i]);
    result = scan(uppercase_letter_sequence);
    ASSERT_FALSE(result.containsInvalid());
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0], PHASE_TOKENS[i]);

    InputSequence lowercase_letter_sequence;
    lowercase_letter_sequence.addInput(LOWERCASE_LETTER_INPUT[i]);
    result = scan(lowercase_letter_sequence);
    ASSERT_FALSE(result.containsInvalid());
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0], PHASE_TOKENS[i]);
  }
}

TEST_F(ScanInput, validateInitialization) {
  const uint8_t SIZE = 5;
  const Token INITIALIZED_VALUES[] = {Token::INVALID, Token::INVALID,
                                      Token::INVALID, Token::INVALID,
                                      Token::INVALID};
  result = TokenSet{SIZE};
  ASSERT_TRUE(result.containsInvalid());
  ASSERT_EQ(result.size(), SIZE);
  ASSERT_EQ(result, INITIALIZED_VALUES);
}

TEST_F(ScanInput, sampleInputUppercase) {
  const Token EXPECTED[] = {Token::AC_phaseA, Token::AC_phaseB,
                            Token::AC_phaseC, Token::NUM1, Token::NUM0};
  InputSequence inputSequence;
  inputSequence.addInput("ABC10");
  result = scan(inputSequence);
  ASSERT_FALSE(result.containsInvalid());
  ASSERT_EQ(result.size(), 5);
  ASSERT_EQ(result, EXPECTED);
}

TEST_F(ScanInput, sampleInputLowercase) {
  const Token EXPECTED[] = {Token::AC_phaseA, Token::AC_phaseB,
                            Token::AC_phaseC, Token::NUM1, Token::NUM0};
  InputSequence inputSequence;
  inputSequence.addInput("abc10");
  result = scan(inputSequence);
  ASSERT_FALSE(result.containsInvalid());
  ASSERT_EQ(result.size(), 5);
  ASSERT_EQ(result, EXPECTED);
}

TEST_F(ScanInput, sampleInputMixedCase) {
  const Token EXPECTED[] = {Token::AC_phaseA, Token::AC_phaseB,
                            Token::AC_phaseC, Token::NUM1, Token::NUM0};
  InputSequence inputSequence;
  inputSequence.addInput("aBc10");
  result = scan(inputSequence);
  ASSERT_FALSE(result.containsInvalid());
  ASSERT_EQ(result.size(), 5);
  ASSERT_EQ(result, EXPECTED);
}

TEST_F(ScanInput, sampleInputInvalidLetter) {
  const Token EXPECTED[] = {Token::AC_phaseA, Token::AC_phaseC, Token::INVALID,
                            Token::INVALID};
  InputSequence inputSequence;
  inputSequence.addInput("ACm0");
  result = scan(inputSequence);
  ASSERT_TRUE(result.containsInvalid());
  ASSERT_EQ(result.size(), 4);
  ASSERT_EQ(result, EXPECTED);
}

TEST_F(ScanInput, sampleInputInvalidChar) {
  const Token EXPECTED[] = {Token::AC_phaseC, Token::INVALID, Token::INVALID,
                            Token::INVALID, Token::INVALID};
  InputSequence inputSequence;
  inputSequence.addInput("C.3/0");
  result = scan(inputSequence);
  ASSERT_TRUE(result.containsInvalid());
  ASSERT_EQ(result.size(), 5);
  ASSERT_EQ(result, EXPECTED);
}
