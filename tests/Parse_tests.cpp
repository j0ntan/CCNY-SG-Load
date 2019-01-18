#include <gtest/gtest.h>
#include <RelayState.h>
#include <Parse.h>
#include <Scan.h>
#include <string>
#include <algorithm>

using namespace ::testing;

std::string full_input("A16B16C16D2");
std::string single_phase_input("A5");
std::string unbalanced_input("A1B2C3D1");
std::string balanced_input("ABC4D2");
std::string phases_unordered_error_input("C12A14B16D2");
std::string missing_AC_number_error_input("ABC");
std::string missing_DC_number_error_input("A4D");
std::string single_number_error_input("6");
std::string single_phase_error_input("B");
std::string begins_with_number_error_input("6ABC3D2");
std::string AC_out_of_range_error_input("ABC17D0");
std::string DC_out_of_range_error_input("A2D5");
std::string lower_case_error_input("abc4d2");
std::string mixed_case_error_input("A2b3c2D0");

class testParseAnalysis : public Test {
 public:
  ParseAnalysis result;
  ParseAnalysis expected;
};

void compareParseIndices(const ParseAnalysis& p1, const ParseAnalysis& p2) {
  EXPECT_EQ(p1.position_phaseA, p2.position_phaseA);
  EXPECT_EQ(p1.position_phaseB, p2.position_phaseB);
  EXPECT_EQ(p1.position_phaseC, p2.position_phaseC);
  EXPECT_EQ(p1.position_DC, p2.position_DC);
  EXPECT_EQ(p1.count_phaseA, p2.count_phaseA);
  EXPECT_EQ(p1.count_phaseB, p2.count_phaseB);
  EXPECT_EQ(p1.count_phaseC, p2.count_phaseC);
  EXPECT_EQ(p1.count_DC, p2.count_DC);
  ASSERT_EQ(p1.numericals_size, p2.numericals_size);

  for (int i = 0; i < p1.numericals_size; i++)
    EXPECT_EQ(p1.numerical_values[i], p2.numerical_values[i]);
}

TEST_F(testParseAnalysis, indexFullInput) {
  InputSequence full_sequence;
  full_sequence.addInput(full_input.c_str());
  result = analyzeTokens(scan(full_sequence));  // A16B16C16D2

  expected.position_phaseA = 0;
  expected.position_phaseB = 3;
  expected.position_phaseC = 6;
  expected.position_DC = 9;
  expected.count_phaseA = 1;
  expected.count_phaseB = 1;
  expected.count_phaseC = 1;
  expected.count_DC = 1;
  expected.numerical_values = new int[full_input.length()];
  const int ARRAY_NUMERICALS[] = {-1, 1, 6, -1, 1, 6, -1, 1, 6, -1, 2};
  std::copy(std::begin(ARRAY_NUMERICALS), std::end(ARRAY_NUMERICALS),
            expected.numerical_values);
  expected.numericals_size = static_cast<uint8_t>(full_input.length());

  compareParseIndices(result, expected);
}

TEST_F(testParseAnalysis, indexSinglePhaseInput) {
  InputSequence single_phase_sequence;
  single_phase_sequence.addInput(single_phase_input.c_str());
  result = analyzeTokens(scan(single_phase_sequence));  // A5

  expected.position_phaseA = 0;
  expected.count_phaseA = 1;
  expected.numerical_values = new int[single_phase_input.length()];
  const int ARRAY_NUMERICALS[] = {-1, 5};
  std::copy(std::begin(ARRAY_NUMERICALS), std::end(ARRAY_NUMERICALS),
            expected.numerical_values);
  expected.numericals_size = static_cast<uint8_t>(single_phase_input.length());

  compareParseIndices(result, expected);
}

TEST_F(testParseAnalysis, indexPhasesUnorderedInput) {
  InputSequence phases_unordered_sequence;
  phases_unordered_sequence.addInput(phases_unordered_error_input.c_str());
  result = analyzeTokens(scan(phases_unordered_sequence));  // C12A14B16D2

  expected.position_phaseA = 3;
  expected.position_phaseB = 6;
  expected.position_phaseC = 0;
  expected.position_DC = 9;
  expected.count_phaseA = 1;
  expected.count_phaseB = 1;
  expected.count_phaseC = 1;
  expected.count_DC = 1;
  expected.numerical_values = new int[phases_unordered_error_input.length()];
  const int ARRAY_NUMERICALS[] = {-1, 1, 2, -1, 1, 4, -1, 1, 6, -1, 2};
  std::copy(std::begin(ARRAY_NUMERICALS), std::end(ARRAY_NUMERICALS),
            expected.numerical_values);
  expected.numericals_size =
      static_cast<uint8_t>(phases_unordered_error_input.length());

  compareParseIndices(result, expected);
}

class testParseErrorCheck : public Test {
 public:
  ParseAnalysis analysis;
  void prepareParseIndex(const std::string& input) {
    InputSequence inputSequence;
    inputSequence.addInput(input.c_str());
    analysis = analyzeTokens(scan(inputSequence));
  }
};

TEST_F(testParseErrorCheck, checkFullInput) {
  prepareParseIndex(full_input);

  ASSERT_TRUE(hasNoParseErrors(analysis));
}

TEST_F(testParseErrorCheck, checkSinglePhaseInput) {
  prepareParseIndex(single_phase_input);

  ASSERT_TRUE(hasNoParseErrors(analysis));
}

TEST_F(testParseErrorCheck, checkUnbalancedInput) {
  prepareParseIndex(unbalanced_input);

  ASSERT_TRUE(hasNoParseErrors(analysis));
}

TEST_F(testParseErrorCheck, checkBalancedInput) {
  prepareParseIndex(balanced_input);

  ASSERT_TRUE(hasNoParseErrors(analysis));
}

TEST_F(testParseErrorCheck, checkLowerCaseInput) {
  prepareParseIndex(lower_case_error_input);

  ASSERT_TRUE(hasNoParseErrors(analysis));
}

TEST_F(testParseErrorCheck, checkMixedCaseInput) {
  prepareParseIndex(mixed_case_error_input);

  ASSERT_TRUE(hasNoParseErrors(analysis));
}

TEST_F(testParseErrorCheck, checkPhasesUnorderedErrorInput) {
  prepareParseIndex(phases_unordered_error_input);

  ASSERT_FALSE(hasNoParseErrors(analysis));
}

TEST_F(testParseErrorCheck, checkMissingACNumberErrorInput) {
  prepareParseIndex(missing_AC_number_error_input);

  ASSERT_FALSE(hasNoParseErrors(analysis));
}

TEST_F(testParseErrorCheck, checkMissingDCNumberErrorInput) {
  prepareParseIndex(missing_DC_number_error_input);

  ASSERT_FALSE(hasNoParseErrors(analysis));
}

TEST_F(testParseErrorCheck, checkSingleNumberErrorInput) {
  prepareParseIndex(single_number_error_input);

  ASSERT_FALSE(hasNoParseErrors(analysis));
}

TEST_F(testParseErrorCheck, checkSinglePhaseErrorInput) {
  prepareParseIndex(single_phase_error_input);

  ASSERT_FALSE(hasNoParseErrors(analysis));
}

TEST_F(testParseErrorCheck, checkBeginsWithNumberErrorInput) {
  prepareParseIndex(begins_with_number_error_input);

  ASSERT_FALSE(hasNoParseErrors(analysis));
}

TEST_F(testParseErrorCheck, checkACOutOfRangeErrorInput) {
  prepareParseIndex(AC_out_of_range_error_input);

  ASSERT_FALSE(hasNoParseErrors(analysis));
}

TEST_F(testParseErrorCheck, checkDCOutOfRangeErrorInput) {
  prepareParseIndex(DC_out_of_range_error_input);

  ASSERT_FALSE(hasNoParseErrors(analysis));
}

class testParseTokens : public Test {
 public:
  RelayState expected_relay_state;
  RelayState previous_relay_state{5, 5, 5, 1};
  ParseAnalysis analysis;
  void prepareParseAnalysis(const std::string& input) {
    InputSequence inputSequence;
    inputSequence.addInput(input.c_str());
    analysis = analyzeTokens(scan(inputSequence));
  }
};

bool operator==(const RelayState& lhs, const RelayState& rhs) {
  return lhs.phaseA == rhs.phaseA && lhs.phaseB == rhs.phaseB &&
         lhs.phaseC == rhs.phaseC && lhs.DC == rhs.DC;
}

TEST_F(testParseTokens, parseFullInput) {
  prepareParseAnalysis(full_input);  // A16B16C16D2
  parseNewRelayState(analysis, previous_relay_state);
  expected_relay_state = {16, 16, 16, 2};
  ASSERT_EQ(previous_relay_state, expected_relay_state);
}

TEST_F(testParseTokens, parseSinglePhaseInput) {
  prepareParseAnalysis(std::string("A8"));
  parseNewRelayState(analysis, previous_relay_state);
  expected_relay_state = {8, 5, 5, 1};
  ASSERT_EQ(previous_relay_state, expected_relay_state);
}

TEST_F(testParseTokens, parseBalancedInput) {
  prepareParseAnalysis(balanced_input);  // ABC4D2
  parseNewRelayState(analysis, previous_relay_state);
  expected_relay_state = {4, 4, 4, 2};
  ASSERT_EQ(previous_relay_state, expected_relay_state);
}
