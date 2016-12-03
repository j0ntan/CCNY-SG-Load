#include "Parser.h"
/*
More knowledge about parsing techniques can help to improve this class,
especially the check_for_errors() and assign_values() functions, which
repeat some of the same code and are just too long. */

Parser::Parser(void) {
    for(int i = 0; i < 4; i++)
        _switching_values[i] = -1; // init
}

bool Parser::parse(const Scanner::tokens &inputTokens) {
    /*
    Parses a set of valid tokens against several rules for a valid input
    sequence. For a valid sequence, get the values of relays to turn ON
    and store in _switching_values. Return true if sequence has no
    errors. */

    bool error_found = check_for_errors(inputTokens);

    if(!error_found)
        assign_values(inputTokens);

    return !error_found;
}

const int *Parser::getSwitchingValues(void) const{
    return _switching_values;
}
const String& Parser::getErrMsg(void) const {
    return _errMsg;
}


bool Parser::check_for_errors(const Scanner::tokens &inputTokens) {
    /*
    This function checks that certain rules are followed for an input
    sequence. All rules must be followed for an input sequence to be
    parsed into integer values. */

    // counters and position markers to help error checking
    short pos_AC_A = -1;
    short pos_AC_B = -1;
    short pos_AC_C = -1;
    short pos_DC   = -1;
    short count_AC_A = 0;
    short count_AC_B = 0;
    short count_AC_C = 0;
    short count_DC   = 0;

    
    // Array holds intger values of numerical chars in sequence
    // for example,
    // input sequence: 'A' '1' '2' 'B' 'C' '8'
    // gives
    // numerical_vals: -1   1   2  -1  -1   8
    short *numerical_vals = new short[inputTokens.size];

    // initialization
    for (short index=0; index < inputTokens.size; ++index)
        *(numerical_vals + index) = -1;

    // locate relative position of voltages & int values
    for (short counter=0; counter < inputTokens.size; ++counter)
        switch(*(inputTokens.vals + counter)) {
            case Scanner::Alphabet::NUM0:
                *(numerical_vals + counter) = 0; break;
            case Scanner::Alphabet::NUM1:
                *(numerical_vals + counter) = 1; break;
            case Scanner::Alphabet::NUM2:
                *(numerical_vals + counter) = 2; break;
            case Scanner::Alphabet::NUM3:
                *(numerical_vals + counter) = 3; break;
            case Scanner::Alphabet::NUM4:
                *(numerical_vals + counter) = 4; break;
            case Scanner::Alphabet::NUM5:
                *(numerical_vals + counter) = 5; break;
            case Scanner::Alphabet::NUM6:
                *(numerical_vals + counter) = 6; break;
            case Scanner::Alphabet::NUM7:
                *(numerical_vals + counter) = 7; break;
            case Scanner::Alphabet::NUM8:
                *(numerical_vals + counter) = 8; break;
            case Scanner::Alphabet::NUM9:
                *(numerical_vals + counter) = 9; break;
            case Scanner::Alphabet::AC_phA:
                pos_AC_A = counter; count_AC_A++; break;
            case Scanner::Alphabet::AC_phB:
                pos_AC_B = counter; count_AC_B++; break;
            case Scanner::Alphabet::AC_phC:
                pos_AC_C = counter; count_AC_C++; break;
            case Scanner::Alphabet::DC:
                pos_DC   = counter; count_DC++;   break;
        } // end switch


    // Now we check for specific errors:

    // Input must include at least one number
    bool num_found = false;
    for (short i=0; i < inputTokens.size && !num_found; ++i)
        if(*(numerical_vals + i) > -1)
            num_found = true;
    if(num_found==false) {
        _errMsg.concat( F("                    ") );
        _errMsg.concat( F("    Must include    ") );
        _errMsg.concat( F("     a number.      ") );
        _errMsg.concat( F("                    ") );
        delete []numerical_vals;
        return true;
    }

    // last input must be a number
    if(*(numerical_vals + inputTokens.size - 1) == -1) {
        _errMsg.concat( F("                    ") );
        _errMsg.concat( F("      Must end      ") );
        _errMsg.concat( F("   with a number.   ") );
        _errMsg.concat( F("                    ") );
        delete []numerical_vals;
        return true;
    }

    /*
    Check range of numbers. Must be within 0-16 for AC.
    Two consecutive digits must be greater than 9, i.e. '1' vs '01'. */
    if(!numbers_within_range(numerical_vals, inputTokens.size)) {
        // error messages set in numbers_within_range()
        delete []numerical_vals;
        return true;
    }

    // First input must be a voltage
    if(pos_AC_A != 0 && pos_AC_B != 0 && pos_AC_C != 0 && pos_DC != 0) {
        _errMsg.concat( F("                    ") );
        _errMsg.concat( F("   First element    ") );
        _errMsg.concat( F(" must be A,B,C or D ") );
        _errMsg.concat( F("                    ") );
        delete []numerical_vals;
        return true;
    }

    // Must have single occurrence of AC_A,AC_B,AC_C, or DC
    if(count_AC_A > 1 || count_AC_B > 1 ||
       count_AC_C > 1 || count_DC > 1) {
        _errMsg.concat( F("                    ") );
        _errMsg.concat( F("  Include A, B, C,  ") );
        _errMsg.concat( F("  or D only once.   ") );
        _errMsg.concat( F("                    ") );
        delete []numerical_vals;
        return true;
    }

    // AC_A, AC_B, AC_C, DC must go in order
    if( count_AC_A == 1 &&
            (count_AC_B == 1 && pos_AC_A > pos_AC_B)||
            (count_AC_C == 1 && pos_AC_A > pos_AC_C)||
            (count_DC   == 1 && pos_AC_A > pos_DC)
     || count_AC_B == 1 &&
            (count_AC_C == 1 && pos_AC_B > pos_AC_C)||
            (count_DC   == 1 && pos_AC_B > pos_DC)
     || count_AC_C == 1 &&
            (count_DC   == 1 && pos_AC_C > pos_DC)  ) {
        _errMsg.concat( F("                    ") );
        _errMsg.concat( F("    ABCD must be    ") );
        _errMsg.concat( F("     in order.      ") );
        _errMsg.concat( F("                    ") );
        delete []numerical_vals;
        return true;
    }

    // Max for DC is 2
    if(count_DC == 1 && *(numerical_vals + pos_DC + 1) > 2) {
        _errMsg.concat( F("                    ") );
        _errMsg.concat( F("   Out of range.    ") );
        _errMsg.concat( F("   DC value must    ") );
        _errMsg.concat( F("    be 2 or less.   ") );
        delete []numerical_vals;
        return true;
    }

    // done checking for errors
    // couldn't find any at this point

    delete []numerical_vals;
    return false;
}
void Parser::assign_values(const Scanner::tokens &inputTokens) {
    /*
    After having found no errors, now we can check the sequence for the
    integer values of the relays. */

    // counters and position markers to help identify values for relays
    short pos_AC_A = -1;
    short pos_AC_B = -1;
    short pos_AC_C = -1;
    short pos_DC   = -1;
    bool found_AC_A = false;
    bool found_AC_B = false;
    bool found_AC_C = false;
    bool found_DC   = false;

    short *numerical_vals = new short[inputTokens.size];

    // initialization
    for (short index = 0; index < inputTokens.size; ++index)
        *(numerical_vals + index++) = -1;

    // locate relative position of voltages & int values
    for (short counter = 0; counter < inputTokens.size; ++counter)
        switch(*(inputTokens.vals + counter)) {
            case Scanner::Alphabet::NUM0:
                *(numerical_vals + counter) = 0; break;
            case Scanner::Alphabet::NUM1:
                *(numerical_vals + counter) = 1; break;
            case Scanner::Alphabet::NUM2:
                *(numerical_vals + counter) = 2; break;
            case Scanner::Alphabet::NUM3:
                *(numerical_vals + counter) = 3; break;
            case Scanner::Alphabet::NUM4:
                *(numerical_vals + counter) = 4; break;
            case Scanner::Alphabet::NUM5:
                *(numerical_vals + counter) = 5; break;
            case Scanner::Alphabet::NUM6:
                *(numerical_vals + counter) = 6; break;
            case Scanner::Alphabet::NUM7:
                *(numerical_vals + counter) = 7; break;
            case Scanner::Alphabet::NUM8:
                *(numerical_vals + counter) = 8; break;
            case Scanner::Alphabet::NUM9:
                *(numerical_vals + counter) = 9; break;
            case Scanner::Alphabet::AC_phA:
                pos_AC_A = counter; found_AC_A = true; break;
            case Scanner::Alphabet::AC_phB:
                pos_AC_B = counter; found_AC_B = true; break;
            case Scanner::Alphabet::AC_phC:
                pos_AC_C = counter; found_AC_C = true; break;
            case Scanner::Alphabet::DC:
                pos_DC   = counter; found_DC = true; break;
        } // end switch

    // convert consecutive nums into signle num, e.g. '1''4' = '14''-1'
    for (short counter=0; counter < inputTokens.size - 1; ++counter)
        if(numerical_vals[counter]==1 && numerical_vals[counter+1] > -1) {
            numerical_vals[counter] = 10 + numerical_vals[counter+1];
            numerical_vals[counter+1] = -1;
        }


    // assign switching values
    for ( short counter = 0; counter < inputTokens.size; ++counter)
        if(*(numerical_vals + counter) > -1) {
            if(found_AC_A &&
                    pos_AC_A < counter && _switching_values[0] == -1) 
                _switching_values[0] = *(numerical_vals + counter);
            if(found_AC_B &&
                    pos_AC_B < counter && _switching_values[1] == -1) 
                _switching_values[1] = *(numerical_vals + counter);
            if(found_AC_C &&
                    pos_AC_C < counter && _switching_values[2] == -1) 
                _switching_values[2] = *(numerical_vals + counter);
            if(found_DC &&
                    pos_DC < counter   && _switching_values[3] == -1) 
                _switching_values[3] = *(numerical_vals + counter);
        }

    delete []numerical_vals;
}

bool Parser::numbers_within_range(short *numerical_vals, short size) {
    /*
    Checks that a numerical value for an AC phase relays is within the
    range 0-16. */

    short counter = 0;
    bool found_num_0;
    bool found_num_1;
    bool found_num_after_num;
    bool found_789_after_1;
    bool found_num_gt_19; // gt is 'greater than'

    while(counter < size) {
        found_num_0 = numerical_vals[counter] ==  0;
        found_num_1 = numerical_vals[counter] ==  1;
        found_num_after_num = (counter + 1 < size) &&
            *(numerical_vals + counter + 1) > -1;
        found_789_after_1 = (counter + 1 < size) &&
            *(numerical_vals + counter + 1) > 6; // upper bound is 9
        found_num_gt_19 = *(numerical_vals + counter) > 1 && // case: >19
            found_num_after_num;

        // Do not allow inputs of 17, 18, 19, or > 19
        if(found_num_1 && found_789_after_1 || found_num_gt_19) {
            _errMsg.concat( F("                    ") );
            _errMsg.concat( F("   Out of range.    ") );
            _errMsg.concat( F("   AC values must   ") );
            _errMsg.concat( F("  be in range 0-16. ") );
            return false;
        }
        // Do not allow leading zeros, i.e. '1' vs '01'
        else if(found_num_0 && found_num_after_num) {
            _errMsg.concat( F("                    ") );
            _errMsg.concat( F(" No leading zeros.  ") );
            _errMsg.concat( F("  Use '6' instead   ") );
            _errMsg.concat( F("      of '06'.      ") );
            return false;
        }
        ++counter;
    } // end while

    // found nothing out of range
    return true;
}