#ifndef PARSER_H
#define PARSER_H

#include "Scanner.h"

class Parser {
public:
    Parser(void);

    bool parse(const Scanner::tokens &inputTokens);

    const int *getSwitchingValues(void) const;
    const String& getErrMsg(void) const;


private:
    bool check_for_errors(const Scanner::tokens &inputTokens);
    void assign_values(const Scanner::tokens &inputTokens);
    
    // helper function
    bool numbers_within_range(short *numerical_vals, short size);

    int _switching_values[4] = {0};
    
    String _errMsg;
};

#endif // !PARSER_H
