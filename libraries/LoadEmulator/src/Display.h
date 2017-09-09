#ifndef DISPLAY_H
#define DISPLAY_H

#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include "Keypad.h"

// find I2C address using I2C Scanner
#define I2C_ADDR 0x27

class Display : LiquidCrystal_I2C {
public:
    // Duration in milliseconds.
    enum class Speed
        {STATIC=0, FASTER=200, FAST=500, NORMAL=1200, SLOW=1800};

    Display(HardwareSerial &serIntrpt, HardwareSerial &xbIntrpt,
            Keypad &keyIntrpt, uint8_t lcd_Addr=0x27, 
            uint8_t En=2, uint8_t Rw=1, uint8_t Rs=0, 
            uint8_t d4=4, uint8_t d5=5, uint8_t d6=6, uint8_t d7=7,
            uint8_t backlighPin=3, t_backlighPol pol=POSITIVE); 

    void begin( uint8_t cols, uint8_t rows );
    void showMessage(String message, Speed speed=Speed::NORMAL);

private:
    void show_caution_message(void);

    HardwareSerial &_serIntrpt;
    HardwareSerial &_xbIntrpt;
    Keypad &_keyIntrpt;
};

#endif // !DISPLAY_H
