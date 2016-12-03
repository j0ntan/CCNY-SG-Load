#ifndef SCANNER_H
#define SCANNER_H

#include "Arduino.h"

class Scanner {
public:
    enum class Alphabet
        {NUM0, NUM1, NUM2, NUM3, NUM4,
         NUM5, NUM6, NUM7, NUM8, NUM9,
         AC_phA, AC_phB, AC_phC, DC, INVALID};

    struct tokens{
        Alphabet *vals = nullptr;
        short size = 0;
    };


    ~Scanner(void);

    bool scan(const String &input); // true if no input error

    const tokens getTokens(void) const;
    const String getErrMsg(void) const;


private:
    Alphabet get_token(const char &inputChar);

    void update_err_msg(char illegalChar);

    tokens _tokens;
    bool _illegalChar = false;
    String _errMsg;
};

#endif // !SCANNER_H
