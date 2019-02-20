#ifndef INPUTSEQUENCE_UTILS_H
#define INPUTSEQUENCE_UTILS_H

class InputSequence;

bool operator==(const InputSequence& lhs, const InputSequence& rhs);

InputSequence initialized_Sequence(const char* str);

bool operator==(const InputSequence& lhs, const char* rhs);

#endif  // INPUTSEQUENCE_UTILS_H
