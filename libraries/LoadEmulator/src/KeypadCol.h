#ifndef KEYPADCOL_H
#define KEYPADCOL_H

#include "Collector.h"
#include "Keypad.h"

class KeypadCol : public Collector{
public:
    KeypadCol(Keypad &keypad);

    void collectKeypress(void);
    void waitKeyRelease(void);


private:
    void record_single_press(void);
    void use_hold_function(void);
    void prepend_balanced(void);

    void update_col_msg(void);
    void set_cancel_msg(void);

    Keypad &_keypad;
    char _pressed_key;
};

#endif // !KEYPADCOL_H
