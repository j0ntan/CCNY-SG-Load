#ifndef INPUTSEQUENCE_H
#define INPUTSEQUENCE_H

#include <stdint.h>

class InputSequence {
 public:
  InputSequence() = default;
  ~InputSequence() = default;

  InputSequence(const InputSequence& seq);
  InputSequence& operator=(const InputSequence& rhs);

  uint8_t length() const;

  char& operator[](uint8_t index);
  char operator[](uint8_t index) const;

  void applyBalancedSequence(uint8_t amount);
  void applyResetSequence();
  void cancelSequence();

  void addInput(char input);
  void addInput(const char* input_str);
  void removeLastInput();

 private:
  static const uint8_t SIZE = 13;  // 12 chars + null terminator
  char buffer[SIZE] = {};
};

#endif  // INPUTSEQUENCE_H
