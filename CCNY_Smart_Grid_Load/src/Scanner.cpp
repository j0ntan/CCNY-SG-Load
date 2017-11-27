#include "../include/Scanner.h"

Scanner::~Scanner(void) {
    if(!_tokens.vals) delete []_tokens.vals;
}

bool Scanner::scan(const String &input) {
    /*
    Scan the input and assign an Alphabet token to each character.
    Returns true if no invalid characters are found. */

    short size = input.length();
    _tokens.vals = new Alphabet[size];
    _tokens.size = size;

    // Assign tokens
    short counter = 0;
    while(counter < size && !_illegalChar)
        *(_tokens.vals + counter) = get_token(input[counter++]);

    if(_illegalChar)
        update_err_msg(input[--counter]);

    return !_illegalChar;
}

const Scanner::tokens Scanner::getTokens(void) const {
    return _tokens;
}
const String Scanner::getErrMsg(void) const {
    return _errMsg;
}


Scanner::Alphabet Scanner::get_token(const char &inputChar) {
    switch(inputChar) {
        case 'A': case 'a':     return Alphabet::AC_phA;
        case 'B': case 'b':     return Alphabet::AC_phB;
        case 'C': case 'c':     return Alphabet::AC_phC;
        case 'D': case 'd':     return Alphabet::DC;
        case '0': return Alphabet::NUM0;  case '1': return Alphabet::NUM1;
        case '2': return Alphabet::NUM2;  case '3': return Alphabet::NUM3;
        case '4': return Alphabet::NUM4;  case '5': return Alphabet::NUM5;
        case '6': return Alphabet::NUM6;  case '7': return Alphabet::NUM7;
        case '8': return Alphabet::NUM8;  case '9': return Alphabet::NUM9;
        default:
            _illegalChar = true; return Alphabet::INVALID;
    }
}

void Scanner::update_err_msg(char illegalChar) {
    _errMsg.concat( F("                    ") );
    _errMsg.concat( F("  Cannot interpret  ") );
    
    _errMsg.concat( F("  the character: ") );
    _errMsg.concat(illegalChar);
    _errMsg.concat( F(" ") );
    _errMsg.concat( F("   Repeat input.    ") );
}
