#include "../include/InputSequence.h"
#include "../include/RelayState.h"

namespace {
bool isNotEmpty(const InputSequence& input) { return input.length() != 0; }
}  // namespace

bool isValidSequence(const InputSequence& input) { return isNotEmpty(input); }

RelayState generateRelayState(const InputSequence& input) {}
