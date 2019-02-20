#include "ValidTokens.h"

Token charToToken(const char& ch) {
  switch (ch) {
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