#include "../include/Keypad.h"

Keypad::Keypad(int R1, int R2, int R3, int R4,
               int C1, int C2, int C3, int C4) :
        _row( {R1, R2, R3, R4} ),
        _col( {C1, C2, C3, C4} ) {
    /*
    Initialize a keypad instance and assign the input row pins and 
    output column pins. Outputs are LOW by default, and are toggled
    to determine a keypress event and ID. */

    // Assign row pins; HI/LO state to be read
    pinMode(_row[0], INPUT_PULLUP);
    pinMode(_row[1], INPUT_PULLUP);
    pinMode(_row[2], INPUT_PULLUP);
    pinMode(_row[3], INPUT_PULLUP);

    
    // Assign column pins & set them to a LOW initial/idle state
    pinMode(_col[0], OUTPUT);
    pinMode(_col[1], OUTPUT);
    pinMode(_col[2], OUTPUT);
    pinMode(_col[3], OUTPUT);
    digitalWrite(_col[0], LOW);
    digitalWrite(_col[1], LOW);
    digitalWrite(_col[2], LOW);
    digitalWrite(_col[3], LOW);
}
void Keypad::setHoldThreshold(int thresh=500) {
    /*
    Change the value of the threshold time used to detect a button hold
    event. */
    if(thresh > 0) _hold_threshold = thresh;
}

bool Keypad::isPressed(void) const{
    // Check if > 0 keys are pressed
    return !digitalRead(_row[0]) || !digitalRead(_row[1]) ||
           !digitalRead(_row[2]) || !digitalRead(_row[3]);
}
bool Keypad::isHeld(void) const {
    /*
    Check if a button is held longer than threshold value. */

    int press_duration = 0;

    while ( isPressed() && press_duration < _hold_threshold ) {
        delay(10);
        press_duration += 10; // increment 10 ms
    }
    return press_duration >= _hold_threshold;
}

char Keypad::getKey(void) const {
    /*
    Returns a character that corresponds to the key that was pressed.
    When multiple keys are pressed '?' is returned. */

    bool pressedKeys[4][4] = {};
    // Toggle pins to determine pressed keys and press count
    short press_count = toggle_keys(pressedKeys);

    return press_count > 1 ? '?' : find_single_key(pressedKeys);;
}


short Keypad::toggle_keys(bool pressedKeys[][4]) const {
    // Toggles output pins to determine which keys have been pressed

    short keypress_count = 0;

    // Check row pins.
    for (int r_idx = 0; r_idx < 4; r_idx++)
        if (digitalRead(_row[r_idx]) == 0)
            // Toggle column pins.
            for (int c_idx = 0; c_idx < 4; c_idx++) {
                digitalWrite(_col[c_idx], HIGH);
                if (digitalRead(_row[r_idx]) == true) {
                    pressedKeys[r_idx][c_idx] = true;
                    keypress_count++;
                }
                // Revert to original state.
                digitalWrite(_col[c_idx],  LOW);
            }

    return keypress_count;
}
char Keypad::find_single_key(bool pressedKeys [][4]) const {
    /*
    Looks through 4x4 array 'pressedKeys' and returns the corresponding
    character of the physical keypad. Each key is assigned an ID, 
    starting from the top left and traversing to the bottom right. */

    short key_ID = 0;
    for (int r_idx = 0; r_idx < 4; r_idx++)
        for (int c_idx = 0; c_idx < 4; c_idx++)
            if (pressedKeys[r_idx][c_idx]) {
                c_idx = 4; // Escape inner for
                r_idx = 4; // Escape outer for
            }
            else key_ID++;

    switch (key_ID) {
        case 0: return '1';
        case 1: return '2';
        case 2: return '3';
        case 3: return 'A';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return 'B';
        case 8: return '7';
        case 9: return '8';
        case 10: return '9';
        case 11: return 'C';
        case 12: return '*';
        case 13: return '0';
        case 14: return '#';
        case 15: return 'D';
    }
}
