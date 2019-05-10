#include "../../include/Display/Message.h"
#include <avr/pgmspace.h>

namespace Display {
// A fun little message about a bear inside the load :D
extern const Display::Message caution_1 PROGMEM = {
    "                    ", "     Caution!!      ", "    !!Caution!!     ",
    "                    ", Display::Duration::NORMAL};
extern const Display::Message caution_2 PROGMEM = {
    "                    ", "    Grizzly bear    ", "      inside!!      ",
    "                    ", Display::Duration::NORMAL};
extern const Display::Message caution_3 PROGMEM = {
    "                    ", "       DO NOT       ", "       OPEN!!       ",
    "                    ", Display::Duration::NORMAL};

extern const Display::Message initialization_1 PROGMEM = {
    "        CCNY        ", "   Smart Grid Lab   ", "  Prof. A. Mohamed  ",
    "      ST 6/643      ", Display::Duration::LONG};
extern const Display::Message initialization_2 PROGMEM = {
    "                    ", "Author: J. Gamboa   ", "Code version: ?.?   ",
    "                    ", Display::Duration::SHORTER};
extern const Display::Message initialization_3 PROGMEM = {
    "                    ", "                    ", "  Ready for input.  ",
    "                    ", Display::Duration::SHORTER};

extern const Display::Message load_reset PROGMEM = {
    "                    ", "     Load RESET     ", "                    ",
    "                    ", Display::Duration::SHORT};
extern const Display::Message cancel_sequence PROGMEM = {
    "                    ", "       CANCEL       ", "Input is cleared.   ",
    "                    ", Display::Duration::SHORT};

extern const Display::Message keypad_sequence PROGMEM = {
    "                    ", "Keypad sequence:    ", "                    ",
    "                    ", Display::Duration::STATIC};

extern const Display::Message dspace_sequence PROGMEM = {
    "                    ", "dSPACE input:       ", "                    ",
    "                    ", Display::Duration::STATIC};

extern const Display::Message SD_file_open_error PROGMEM = {
    "Could not open the  ", "chosen file. Check  ", "the SD card module  ",
    "or file choice.     ", Display::Duration::LONG};

extern const Display::Message invalid_char_error PROGMEM = {
    "                    ", "  Cannot interpret  ",
    "the character:      ", "   Repeat input.    ", Display::Duration::STATIC};
extern const Display::Message missing_number_error PROGMEM = {
    "                    ", "       Error        ", " Input must contain ",
    "at least one number.", Display::Duration::NORMAL};
extern const Display::Message missing_phase_error PROGMEM = {
    "                    ", "       Error        ", " Input must contain ",
    "at least one letter.", Display::Duration::NORMAL};
extern const Display::Message numerical_ending_error PROGMEM = {
    "                    ", "       Error        ", "   Input must end   ",
    "   with a number.   ", Display::Duration::NORMAL};
extern const Display::Message digits_out_of_range_error PROGMEM = {
    "                    ", "       Error        ", "Number cannot exceed",
    "     2 digits.      ", Display::Duration::NORMAL};
extern const Display::Message DC_out_of_range_error PROGMEM = {
    "                    ", "       Error        ", " DC value is out of ",
    "  range. Max is 2.  ", Display::Duration::NORMAL};
extern const Display::Message AC_out_of_range_error PROGMEM = {
    "                    ", "       Error        ", " AC value is out of ",
    " range. Max is 16.  ", Display::Duration::NORMAL};
extern const Display::Message no_starting_phase_error PROGMEM = {
    "                    ", "       Error        ", "Input sequence must ",
    "begin with a phase. ", Display::Duration::NORMAL};
extern const Display::Message repeating_phase_error PROGMEM = {
    "                    ", "       Error        ", " Phase is repeated. ",
    "                    ", Display::Duration::NORMAL};
extern const Display::Message leading_zero_error PROGMEM = {
    "                    ", "       Error        ", "   Remove leading   ",
    "zeros, i.e. 6 vs 06 ", Display::Duration::NORMAL};
extern const Display::Message unordered_phases_error PROGMEM = {
    "                    ", "       Error        ", "   Phases are not   ",
    "     in order.      ", Display::Duration::NORMAL};

extern const Display::Message load_idle_state PROGMEM = {
    "Idle state:         ", "AC(3-ph) =          ", "A:                  ",
    "DC:                 ", Display::Duration::STATIC};
}  // namespace Display
