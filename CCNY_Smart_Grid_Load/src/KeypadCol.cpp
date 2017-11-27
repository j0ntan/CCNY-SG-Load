#include "../include/KeypadCol.h"
KeypadCol::KeypadCol(Keypad &keypad) :
        _keypad(keypad) {
    /*
    pass the Keypad instance that is used to collect keypress events
    and input characters */
}

void KeypadCol::collectKeypress(void) {
    /*
    After a keypad button press, either get an input character (and
    update input sequence) or perform function from button press/hold */

    // identify button pressed
    _pressed_key = _keypad.getKey();
    
    // update sequence
    record_single_press();
    
    if (_collectingStatus && _keypad.isHeld())
        // _collectingStatus condition needed for RESET vs CANCEL
        // issue; see comment in record_single_press()
        use_hold_function();
}
void KeypadCol::waitKeyRelease(void) {
    // stay idle until button is released

    while(_keypad.isPressed())
        ; // you do nothing, Jon Snow!!

    // guard time between consecutive presses
    delay(300);
}


void KeypadCol::record_single_press(void) {
    /*
    Update input sequence or perform function based on the key that was
    pressed. */

    switch (_pressed_key) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case 'A': case 'B': case 'C': case 'D':
            _input.concat(_pressed_key);
            update_col_msg();
            break;
        case '*':
            if(_keypad.isHeld())
                /*
                Early call to use_hold_function is needed to avoid
                issue with the following inout sequence: press(any),
                then press and hold('*'). Without if, initial press
                of '*' will cause BACKSPACE and then the caller will
                call use_hold_function and apply RESET instead of
                the intended CANCEL. */
                use_hold_function();
            else
                // BACKSPACE
                if(_input.length() > 0) {
                    _input.remove(_input.length() - 1);
                    update_col_msg();
                }
            break; // backspace after key release
        case '#': // ENTER
            _collectingStatus = false; break;
    } // end switch
}
void KeypadCol::use_hold_function(void) {
    /*
    Update input sequence or perform function based on the key that was
    held. */

    switch (_pressed_key) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            // e.g. hold '1' + '2' = "ABC12" or '6' = "ABC6"
            prepend_balanced();
            _collectingStatus = false;
            update_col_msg();
            break;
        case 'A': case 'B': case 'C': case 'D': // do nothing
            // undo the recorded keypress
            _input.remove(_input.length() - 1);
            break;
        case '*':
            if(_input.length() > 0) {
                // CANCEL
                _input.remove(0);
                set_cancel_msg();
            }
            else {
                // RESET
                _input.remove(0);
                _input.concat( F("ABCD0") );
                update_col_msg();
            }
            _collectingStatus = false;
            break;
        case '#': // do nothing
            break;
    } // end switch
}
void KeypadCol::prepend_balanced(void) {
    /*
    Take the characters already stored in the sequence and add "ABC" to 
    the front. */
    
    static String tempStr;
    tempStr.reserve(5);
    tempStr.remove(0);

    tempStr.concat(F("ABC"));
    tempStr.concat(_input);
    
    _input.remove(0);
    _input.concat(tempStr);
}

void KeypadCol::update_col_msg(void) {
    /*
    Update the message displayed on LCD due to some change in the input
    sequence. */

    _collectorMsg.remove(0);
    _collectorMsg.concat( F("                    ") );
    _collectorMsg.concat( F("Input sequence:     ") );
    
    _collectorMsg.concat(_input);
    // add spaces after input to complete 20
    int spaces_counter = 20 - _input.length();
    while(spaces_counter-- > 0) _collectorMsg.concat( F(" ") );

    _collectorMsg.concat( F("                    ") );

    _speed = Display::Speed::STATIC;
}
void KeypadCol::set_cancel_msg(void) {
    // Update the message displayed on LCD due to a CANCEL function.

    _collectorMsg.remove(0);
    _collectorMsg.concat( F("                    ") );
    _collectorMsg.concat( F("       CANCEL       ") );
    _collectorMsg.concat( F("Input is cleared.   ") );
    _collectorMsg.concat( F("                    ") );

    _speed = Display::Speed::NORMAL;
}
