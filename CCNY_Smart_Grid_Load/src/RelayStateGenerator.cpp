#include "../src/ValidTokens.h"
#include "../include/InputSequence.h"
#include "../include/RelayState.h"

class InputAnalytics {
 public:
  uint8_t position_phaseA = InputSequence::SIZE;
  uint8_t position_phaseB = InputSequence::SIZE;
  uint8_t position_phaseC = InputSequence::SIZE;
  uint8_t position_DC = InputSequence::SIZE;

  uint8_t count_phaseA = 0;
  uint8_t count_phaseB = 0;
  uint8_t count_phaseC = 0;
  uint8_t count_DC = 0;

  uint8_t numerical_equivalents[InputSequence::SIZE] = {};
  const uint8_t LENGTH;

  explicit InputAnalytics(const InputSequence& input) : LENGTH{input.length()} {
    // init to sentinel value, InputSequence::SIZE
    for (uint8_t i = 0; i < InputSequence::SIZE; ++i)
      numerical_equivalents[i] = InputSequence::SIZE;

    // analyze each char
    for (uint8_t i = 0; i < LENGTH; ++i) switch (charToToken(input[i])) {
        case Token::NUM0:
        case Token::NUM1:
        case Token::NUM2:
        case Token::NUM3:
        case Token::NUM4:
        case Token::NUM5:
        case Token::NUM6:
        case Token::NUM7:
        case Token::NUM8:
        case Token::NUM9:
          numerical_equivalents[i] = static_cast<uint8_t>(input[i] - '0');
          break;
        case Token::AC_phaseA:
          position_phaseA = i;
          ++count_phaseA;
          break;
        case Token::AC_phaseB:
          position_phaseB = i;
          ++count_phaseB;
          break;
        case Token::AC_phaseC:
          position_phaseC = i;
          ++count_phaseC;
          break;
        case Token::DC:
          position_DC = i;
          ++count_DC;
          break;
        default:
          break;
      }
  }
};

namespace {
bool isNotEmpty(const InputSequence& input) { return input.length() != 0; }

bool containsOnlyValidChars(const InputSequence& input) {
  const uint8_t LENGTH = input.length();
  for (uint8_t i = 0; i < LENGTH; ++i)
    if (charToToken(input[i]) == Token::INVALID) return false;
  return true;
}

bool containsAtLeastOneNumber(const InputAnalytics& analytics) {
  for (uint8_t i = 0; i < analytics.LENGTH; i++)
    if (analytics.numerical_equivalents[i] != InputSequence::SIZE) return true;
  return false;
}

bool containsAtLeastOnePhase(const InputAnalytics& analytics) {
  for (uint8_t i = 0; i < analytics.LENGTH; i++)
    if (analytics.numerical_equivalents[i] == InputSequence::SIZE) return true;
  return false;
}

bool beginsWithPhase(const InputAnalytics& analytics) {
  return analytics.position_phaseA == 0 || analytics.position_phaseB == 0 ||
         analytics.position_phaseC == 0 || analytics.position_DC == 0;
}

bool endsWithNumber(const InputAnalytics& analytics) {
  return analytics.numerical_equivalents[analytics.LENGTH - 1] !=
         InputSequence::SIZE;
}

bool phasesAppearAtMostOnce(const InputAnalytics& analytics) {
  return analytics.count_phaseA <= 1 && analytics.count_phaseB <= 1 &&
         analytics.count_phaseC <= 1 && analytics.count_DC <= 1;
}

bool phasesInOrder(const InputAnalytics& analytics) {
  uint8_t positions[4] = {analytics.position_phaseA, analytics.position_phaseB,
                          analytics.position_phaseC, analytics.position_DC};
  for (uint8_t current_phase = 1; current_phase < 4; ++current_phase)
    for (uint8_t prev_phase = 0; prev_phase < current_phase; ++prev_phase)
      if (positions[prev_phase] != InputSequence::SIZE &&
          positions[prev_phase] > positions[current_phase])
        return false;
  return true;
}

bool hasNoLeadingZeros(const InputAnalytics& analytics) {
  for (uint8_t i = 0; i < analytics.LENGTH; i++)
    if (analytics.numerical_equivalents[i] == 0 && i + 1 < analytics.LENGTH &&
        analytics.numerical_equivalents[i + 1] != InputSequence::SIZE)
      return false;
  return true;
}

bool numbersAreWithinRange(const InputAnalytics& analytics) {
  const uint8_t DC_MAX = 2;
  const uint8_t AC_MAX = 16;

  // check DC range
  if (analytics.count_DC == 1 &&
      (analytics.numerical_equivalents[analytics.position_DC + 1] > DC_MAX ||
       analytics.position_DC != analytics.LENGTH - 2))
    return false;

  // check only single or double consecutive digits
  for (uint8_t i = 1, consecutive_digits = 1; i < analytics.LENGTH; ++i) {
    const bool found_consecutive_pair =
        analytics.numerical_equivalents[i] != InputSequence::SIZE &&
        i + 1 < analytics.LENGTH &&
        analytics.numerical_equivalents[i + 1] != InputSequence::SIZE;
    if (found_consecutive_pair)
      ++consecutive_digits;
    else if (analytics.numerical_equivalents[i] == InputSequence::SIZE)
      consecutive_digits = 1;

    if (consecutive_digits > 2) return false;
  }

  // check double digits for AC range
  for (uint8_t i = 1; i < analytics.LENGTH; i++) {
    const bool found_double_consecutive_digits =
        analytics.numerical_equivalents[i] != InputSequence::SIZE &&
        i + 1 < analytics.LENGTH &&
        analytics.numerical_equivalents[i + 1] != InputSequence::SIZE;
    const bool out_of_range = found_double_consecutive_digits &&
                              (10 * analytics.numerical_equivalents[i] +
                               analytics.numerical_equivalents[i + 1]) > AC_MAX;
    if (found_double_consecutive_digits && out_of_range) return false;
  }

  return true;
}

uint8_t readProceedingValue(const InputAnalytics& analytics, uint8_t start) {
  while (analytics.numerical_equivalents[start] == InputSequence::SIZE) ++start;
  const bool double_digit_value =
      start + 1 < analytics.LENGTH &&
      analytics.numerical_equivalents[start + 1] != InputSequence::SIZE;
  if (double_digit_value)
    return 10 + analytics.numerical_equivalents[start + 1];
  else
    return analytics.numerical_equivalents[start];
}
}  // namespace

bool isValidSequence(const InputSequence& input) {
  InputAnalytics analytics{input};
  return isNotEmpty(input) && containsOnlyValidChars(input) &&
         containsAtLeastOneNumber(analytics) &&
         containsAtLeastOnePhase(analytics) &&
         phasesAppearAtMostOnce(analytics) && beginsWithPhase(analytics) &&
         endsWithNumber(analytics) && phasesInOrder(analytics) &&
         hasNoLeadingZeros(analytics) && numbersAreWithinRange(analytics);
}

void recordNewRelayState(const InputSequence& input, RelayState& relayState) {
  InputAnalytics analytics{input};

  if (analytics.count_phaseA == 1)
    relayState.phaseA =
        readProceedingValue(analytics, analytics.position_phaseA);
  if (analytics.count_phaseB == 1)
    relayState.phaseB =
        readProceedingValue(analytics, analytics.position_phaseB);
  if (analytics.count_phaseC == 1)
    relayState.phaseC =
        readProceedingValue(analytics, analytics.position_phaseC);
  if (analytics.count_DC == 1)
    relayState.DC = readProceedingValue(analytics, analytics.position_DC);
}
