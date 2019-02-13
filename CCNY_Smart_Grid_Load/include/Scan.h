#ifndef SCAN_H
#define SCAN_H

#include <stdint.h>
#include "InputSequence.h"
#include "../src/ValidTokens.h"

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

TokenSet scan(const InputSequence& inputString) {
  TokenSet tokens(inputString.length());

  for (uint8_t n = 0; n < tokens.size(); n++) {
    tokens[n] = charToToken(inputString[n]);
    if (tokens[n] == Token::INVALID) break;
  }

  return tokens;
}

#endif  // SCAN_H
