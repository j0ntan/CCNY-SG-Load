#ifndef SCAN_H
#define SCAN_H

#include <stdint.h>
#include "InputSequence.h"

enum class Token : uint8_t {
  NUM0 = 0,
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

class TokenSet {
 public:
  TokenSet() = default;
  explicit TokenSet(uint8_t size) : _size(size), _tokens(new Token[size]) {
    memset(_tokens, static_cast<int>(Token::INVALID), _size);
  }
  TokenSet(const TokenSet& ts) : _size(ts._size), _tokens(new Token[_size]) {
    memcpy(ts._tokens, _tokens, _size);
  }
  ~TokenSet() {
    delete[] _tokens;
    _tokens = nullptr;
    _size = 0;
  }

  TokenSet& operator=(const TokenSet& rhs) {
    if (&rhs != this) {
      delete[] _tokens;
      _size = rhs._size;
      _tokens = new Token[_size];
      memcpy(_tokens, rhs._tokens, _size);
    }
    return *this;
  }
  Token& operator[](uint8_t i) { return _tokens[i]; }
  const Token& operator[](uint8_t i) const { return _tokens[i]; }

  uint8_t size() const { return _size; }

  bool containsInvalid() const {
    for (uint8_t i = 0; i < _size; i++)
      if (_tokens[i] == Token::INVALID) return true;
    return false;
  }

 private:
  uint8_t _size = 0;
  Token* _tokens = nullptr;
};

namespace helper {
Token convertCharToToken(const char& inputChar) {
  switch (inputChar) {
    case 'A':
    case 'a':
      return Token::AC_phaseA;
    case 'B':
    case 'b':
      return Token::AC_phaseB;
    case 'C':
    case 'c':
      return Token::AC_phaseC;
    case 'D':
    case 'd':
      return Token::DC;
    case '0':
      return Token::NUM0;
    case '1':
      return Token::NUM1;
    case '2':
      return Token::NUM2;
    case '3':
      return Token::NUM3;
    case '4':
      return Token::NUM4;
    case '5':
      return Token::NUM5;
    case '6':
      return Token::NUM6;
    case '7':
      return Token::NUM7;
    case '8':
      return Token::NUM8;
    case '9':
      return Token::NUM9;
    default:
      return Token::INVALID;
  }
}
}  // namespace helper

TokenSet scan(const InputSequence& inputString) {
  TokenSet tokens(inputString.length());

  for (uint8_t n = 0; n < tokens.size(); n++) {
    tokens[n] = helper::convertCharToToken(inputString[n]);
    if (tokens[n] == Token::INVALID) break;
  }

  return tokens;
}

#endif  // SCAN_H
