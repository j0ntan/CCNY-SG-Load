#include "../include/dsManualCol.h"

dsManualCol::dsManualCol(HardwareSerial &xbeeSer) :
        dspaceCol(xbeeSer) {
    // Initialize with Serial port connected to XBee
}

void dsManualCol::collectInt(void) {
    /*
    Reads a byte from the Serial port and updates the input sequence
    and display message accordingly. */

    read_ds_int(); // read buffer

    parse_ds_int(); // update sequence

    if(_xbee.available() == 0) _collectingStatus = false;
}


void dsManualCol::parse_ds_int(void) {
    /*
    Update input sequence and display message based on value sent by
    dSPACE */
    
    if(_ds_int < 29) {
        switch(_ds_int) {
            case 0: case 1: case 2: case 3: case 4:
            case 5: case 6: case 7: case 8: case 9:
                _input.concat(char(48 + _ds_int));
                update_col_msg();
                break;
            case 10: case 11: case 12:
            case 13: case 14: case 15: case 16:
                _input.concat( F("1") );
                _input.concat(char(38 + _ds_int)); // 48 - 10
                update_col_msg();
                break;
            case 17: case 18: case 19: case 20: // A, B, C, or D
                _input.concat(char(48 + _ds_int)); // 65 - 17
                update_col_msg();
                break;
            case 21:
                // CANCEL
                _input.remove(0);
                _collectingStatus = false;
                update_cancel_msg();
                break;
            case 22:
                // BACKSPACE
                if(_input.length() > 0) {
                    _input.remove(_input.length() - 1);
                    update_col_msg();
                }
                break;
            case 23:
                // RESET
                _input.remove(0);
                _input.concat("ABCD0");
                update_col_msg();
                break;
        } // end switch
    } // end if
    else if(_ds_int < 46) {
        switch(_ds_int) {
            case 29: case 30: case 31: case 32: case 33:
            case 34: case 35: case 36: case 37: case 38:
                _input.concat( F("ABC") );
                _input.concat(char(19 + _ds_int)); // 48 - 29
                break;
            case 39: case 40: case 41:
            case 42: case 43: case 44: case 45:
                _input.concat( F("ABC1") );
                _input.concat(char(9 + _ds_int)); // 48 - 39
                break;
        } // end switch 
        _collectingStatus = false;
        update_col_msg();
    } // end else if
}

void dsManualCol::update_col_msg(void) {
    _collectorMsg.remove(0);

    _collectorMsg.concat( F("                    ") );
    _collectorMsg.concat( F("dSPACE input:       ") );
    
    _collectorMsg.concat(_input);
    short counter = 20 - _input.length();
    while(counter-- > 0)
        _collectorMsg.concat( F(" ") );

    _collectorMsg.concat( F("                    ") );

    _speed = Display::Speed::STATIC;
}
void dsManualCol::update_cancel_msg(void) {
    _collectorMsg.remove(0);

    _collectorMsg.concat( F("                    ") );
    _collectorMsg.concat( F("    dSPACE input    ") );
    _collectorMsg.concat( F("    was canceled.   ") );
    _collectorMsg.concat( F("                    ") );

    _speed = Display::Speed::NORMAL;
}
