#include "Display.h"

Display::Display(HardwareSerial &serIntrpt, HardwareSerial &xbIntrpt,
        Keypad &keyIntrpt, uint8_t lcd_Addr, 
        uint8_t En, uint8_t Rw, uint8_t Rs,  
        uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
        uint8_t backlighPin, t_backlighPol pol) :
        
        _serIntrpt(serIntrpt), _xbIntrpt(xbIntrpt), _keyIntrpt(keyIntrpt),
        LiquidCrystal_I2C( lcd_Addr, En, Rw, Rs,
                d4, d5, d6, d7, backlighPin, pol ) {
    // blank ctor
}

void Display::begin(uint8_t cols, uint8_t rows) {
    /*
    This function initializes the LCD and prints a start-up message. */

    LiquidCrystal_I2C::begin( cols, rows, LCD_5x8DOTS );
    // Switch on the backlight
    LiquidCrystal_I2C::setBacklight(HIGH);

    String init_str;
    // Begin writing startup message.
    init_str  = F("        CCNY        ");
    init_str += F("   Smart Grid Lab   ");
    init_str += F("  Prof. A. Mohamed  ");
    init_str += F("      ST 6/643      ");
    showMessage(init_str, Speed::SLOW);

    // Show 'caution' message at random initializations :)
    randomSeed(analogRead(0));
    if (random(17) == 2 || random(17) == 11)
        show_caution_message();

    // Show author & code version.
    init_str  = F("                    ");
    init_str += F("Author: J. Gamboa   ");
    init_str += F("Code version: 4.3   ");
    init_str += F("                    ");
    showMessage(init_str, Speed::FASTER);

    // End of startup messages.
    init_str  = F("                    ");
    init_str += F("  Ready for input.  ");
    init_str += F("                    ");
    init_str += F("                    ");
    showMessage(init_str, Speed::FASTER);
}
void Display::showMessage(String message, Speed speed) {
    /*
    Display on the LCD a string that contains four lines of text.
    A short pause follows to allow reading. This pause is determined by
    the Speed parameter. This pause can be interrupted and ended early 
    by any input from the hardware sources. */

    // print to LCD.
    setCursor (0, 0);
    print(message.substring( 0,20));
    setCursor (0, 1);
    print(message.substring(20,40));
    setCursor (0, 2);
    print(message.substring(40,60));
    setCursor (0, 3);
    print(message.substring(60));

    // Pause until display time is finished or interrupt occurs.
    int time_interval = 0;
    while(time_interval < static_cast<int>(speed)) {
        if (_serIntrpt.available() || _xbIntrpt.available() || 
                _keyIntrpt.isPressed())
            break; // exit early
        else {
            delay(100);
            time_interval += 100;
        }
    } 
}


void Display::show_caution_message(void) {
    /*
    Shows a fun little message on the LCD about a bear inside
    the load :D */

    String caution;  
    caution  = F("                    ");
    caution += F("     Caution!!      ");
    caution += F("    !!Caution!!     ");
    caution += F("                    ");
    showMessage(caution);

    caution  = F("                    ");
    caution += F("    Grizzly bear    ");
    caution += F("      inside!!      ");
    caution += F("                    ");
    showMessage(caution);

    caution  = F("                    ");
    caution += F("       DO NOT       ");
    caution += F("       OPEN!!       ");
    caution += F("                    ");
    showMessage(caution);
}
