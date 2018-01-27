#include <gtest/gtest.h>
#include <Scan.h>
#include <StringDouble.h>

using namespace ::testing;

class ScanInput : public Test {
 public:
  TokenSet result;
  StringDouble input;
};

bool operator==(const TokenSet& lhs, const Token rhs[]) {
  for (uint8_t i = 0; i < lhs.size(); i++)
    if (lhs[i] != rhs[i]) return false;
  return true;
}

bool operator==(const Token lhs[], const TokenSet& rhs) { return rhs == lhs; }

TEST_F(ScanInput, validateInputCharacters) {
  const StringDouble NUMERICAL_INPUT[] = {StringDouble("0"), StringDouble("1"),
                                          StringDouble("2"), StringDouble("3"),
                                          StringDouble("4"), StringDouble("5"),
                                          StringDouble("6"), StringDouble("7"),
                                          StringDouble("8"), StringDouble("9")};
  const StringDouble UPPERCASE_LETTER_INPUT[] = {
      StringDouble("A"), StringDouble("B"), StringDouble("C"),
      StringDouble("D")};
  const StringDouble LOWERCASE_LETTER_INPUT[] = {
      StringDouble("a"), StringDouble("b"), StringDouble("c"),
      StringDouble("d")};
  const Token NUMERICAL_TOKENS[] = {
      Token::NUM0, Token::NUM1, Token::NUM2, Token::NUM3, Token::NUM4,
      Token::NUM5, Token::NUM6, Token::NUM7, Token::NUM8, Token::NUM9};
  const Token PHASE_TOKENS[] = {Token::AC_phaseA, Token::AC_phaseB,
                                Token::AC_phaseC, Token::DC};

  for (int i = 0; i < 5; i++) {
    result = scan(NUMERICAL_INPUT[i]);
    ASSERT_FALSE(result.containsInvalid());
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0], NUMERICAL_TOKENS[i]);
  }
  for (int i = 0; i < 4; i++) {
    result = scan(UPPERCASE_LETTER_INPUT[i]);
    ASSERT_FALSE(result.containsInvalid());
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0], PHASE_TOKENS[i]);

    result = scan(LOWERCASE_LETTER_INPUT[i]);
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
  result = scan(StringDouble("ABC10"));
  ASSERT_FALSE(result.containsInvalid());
  ASSERT_EQ(result.size(), 5);
  ASSERT_EQ(result, EXPECTED);
}

TEST_F(ScanInput, sampleInputLowercase) {
  const Token EXPECTED[] = {Token::AC_phaseA, Token::AC_phaseB,
                            Token::AC_phaseC, Token::NUM1, Token::NUM0};
  result = scan(std::string("abc10"));
  ASSERT_FALSE(result.containsInvalid());
  ASSERT_EQ(result.size(), 5);
  ASSERT_EQ(result, EXPECTED);
}

TEST_F(ScanInput, sampleInputMixedCase) {
  const Token EXPECTED[] = {Token::AC_phaseA, Token::AC_phaseB,
                            Token::AC_phaseC, Token::NUM1, Token::NUM0};
  result = scan(std::string("aBc10"));
  ASSERT_FALSE(result.containsInvalid());
  ASSERT_EQ(result.size(), 5);
  ASSERT_EQ(result, EXPECTED);
}

TEST_F(ScanInput, sampleInputInvalidLetter) {
  const Token EXPECTED[] = {Token::AC_phaseA, Token::AC_phaseC, Token::INVALID,
                            Token::INVALID};
  result = scan(std::string("ACm0"));
  ASSERT_TRUE(result.containsInvalid());
  ASSERT_EQ(result.size(), 4);
  ASSERT_EQ(result, EXPECTED);
}

TEST_F(ScanInput, sampleInputInvalidChar) {
  const Token EXPECTED[] = {Token::AC_phaseC, Token::INVALID, Token::INVALID,
                            Token::INVALID, Token::INVALID};
  result = scan(std::string("C.3/0"));
  ASSERT_TRUE(result.containsInvalid());
  ASSERT_EQ(result.size(), 5);
  ASSERT_EQ(result, EXPECTED);
}
