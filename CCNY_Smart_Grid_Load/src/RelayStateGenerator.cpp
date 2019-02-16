#include "../src/ValidTokens.h"
#include "../include/InputSequence.h"
#include "../include/RelayState.h"

namespace {
bool isNotEmpty(const InputSequence& input) { return input.length() != 0; }

bool containsOnlyValidChars(const InputSequence& input) {
  const uint8_t LENGTH = input.length();
  for (uint8_t i = 0; i < LENGTH; ++i)
    if (charToToken(input[i]) == Token::INVALID) return false;
  return true;
}
}  // namespace

bool isValidSequence(const InputSequence& input) {
  return isNotEmpty(input) && containsOnlyValidChars(input);
}

RelayState generateRelayState(const InputSequence& input) {}
