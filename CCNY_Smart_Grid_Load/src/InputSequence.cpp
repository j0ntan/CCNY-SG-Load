#include "../include/InputSequence.h"
#include <string.h>

InputSequence::InputSequence(const InputSequence& seq) {
  strcpy(buffer, seq.buffer);
}

InputSequence& InputSequence::operator=(const InputSequence& rhs) {
  cancelSequence();
  strcpy(buffer, rhs.buffer);
  return *this;
}

uint8_t InputSequence::length() const { return strlen(buffer); }

char& InputSequence::operator[](uint8_t index) {
  const uint8_t buf_length = length();
  const bool empty_buffer = buf_length == 0;
  const bool within_bounds = index < buf_length;

  if (!empty_buffer && within_bounds)
    return buffer[index];
  else
    return buffer[0];
}

char InputSequence::operator[](uint8_t index) const {
  const uint8_t buf_length = length();
  const bool empty_buffer = buf_length == 0;
  const bool within_bounds = index < buf_length;

  if (!empty_buffer && within_bounds)
    return buffer[index];
  else
    return buffer[0];
}

void InputSequence::applyBalancedSequence(uint8_t amount) {
  cancelSequence();
  strcpy(buffer, "ABC");

  if (amount > 99) amount = 99;  // hard-limited for simplicity

  if (amount > 9) {
    const auto first_digit = static_cast<uint8_t>(amount / 10);
    const auto second_digit = static_cast<uint8_t>(amount % 10);
    addInput(static_cast<char>('0' + first_digit));
    addInput(static_cast<char>('0' + second_digit));
  } else {
    addInput(static_cast<char>('0' + amount));
  }
}

void InputSequence::applyResetSequence() {
  cancelSequence();
  strcpy(buffer, "ABCD0");
}

void InputSequence::cancelSequence() { memset(buffer, 0, InputSequence::SIZE); }

void InputSequence::addInput(char input) {
  const uint8_t buf_length = length();
  const bool empty_buffer = buf_length == 0;
  const bool full_buffer =
      !empty_buffer && (buf_length - 1) == (InputSequence::SIZE - 1 - 1);

  if (empty_buffer) {
    buffer[0] = input;
  } else if (full_buffer) {
    buffer[buf_length - 1] = input;  // overwrite
  } else {
    buffer[buf_length] = input;  // append
  }
}

void InputSequence::addInput(const char* input_str) {
  const uint8_t empty_count = InputSequence::SIZE - 1 - length();
  strncat(buffer, input_str, empty_count);
}

void InputSequence::removeLastInput() {
  const uint8_t buf_length = length();
  if (buf_length > 0) {
    buffer[buf_length - 1] = '\0';
  }
}
