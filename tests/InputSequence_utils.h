#ifndef INPUTSEQUENCE_UTILS_H
#define INPUTSEQUENCE_UTILS_H

#include <InputSequence.h>

bool operator==(const InputSequence& lhs, const InputSequence& rhs) {
  bool retval = lhs.length() == rhs.length();
  if (retval) {
    const uint8_t len = lhs.length();
    for (uint8_t i = 0; i < len && retval; ++i) retval = lhs[i] == rhs[i];
  }
  return retval;
}

InputSequence initialized_Sequence(const char* str) {
  InputSequence retval;
  retval.addInput(str);
  return retval;
}

bool operator==(const InputSequence& lhs, const char* rhs) {
  return lhs == initialized_Sequence(rhs);
}

#endif  // INPUTSEQUENCE_UTILS_H
