#ifndef VALIDTOKENS_H
#define VALIDTOKENS_H

#include <stdint.h>

enum class Token : uint8_t {
  NUM0,
  NUM1,
  NUM2,
  NUM3,
  NUM4,
  NUM5,
  NUM6,
  NUM7,
  NUM8,
  NUM9,
  AC_phaseA,
  AC_phaseB,
  AC_phaseC,
  DC,
  INVALID
};

Token charToToken(const char& ch);

#endif  // VALIDTOKENS_H
