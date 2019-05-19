#include <Keypad.h>
#include <Timer.h>
#include <Display/LCD.h>
#include <Display/MessagesList.h>

Keypad* keypad = nullptr;
Timer* timer = nullptr;
Display::LCD* lcd = nullptr;

namespace Display {
const Display::Message caution_1 = {
    "                    ", "     Caution!!      ", "    !!Caution!!     ",
    "                    ", Display::Duration::NORMAL};
const Display::Message caution_2 = {
    "                    ", "    Grizzly bear    ", "      inside!!      ",
    "                    ", Display::Duration::NORMAL};
const Display::Message caution_3 = {
    "                    ", "       DO NOT       ", "       OPEN!!       ",
    "                    ", Display::Duration::NORMAL};
const Display::Message initialization_1 = {
    "        CCNY        ", "   Smart Grid Lab   ", "  Prof. A. Mohamed  ",
    "      ST 6/643      ", Display::Duration::LONG};
const Display::Message initialization_2 = {
    "                    ", "Author: J. Gamboa   ", "Code version: ?.?   ",
    "                    ", Display::Duration::SHORTER};
const Display::Message initialization_3 = {
    "                    ", "                    ", "  Ready for input.  ",
    "                    ", Display::Duration::SHORTER};
const Display::Message load_reset = {
    "                    ", "     Load RESET     ", "                    ",
    "                    ", Display::Duration::SHORT};
const Display::Message cancel_sequence = {
    "                    ", "       CANCEL       ", "Input is cleared.   ",
    "                    ", Display::Duration::SHORT};
const Display::Message keypad_sequence = {
    "                    ", "Keypad sequence:    ", "                    ",
    "                    ", Display::Duration::STATIC};
const Display::Message dspace_sequence = {
    "                    ", "dSPACE input:       ", "                    ",
    "                    ", Display::Duration::STATIC};
const Display::Message SD_file_open_error = {
    "Could not open the  ", "chosen file. Check  ", "the SD card module  ",
    "or file choice.     ", Display::Duration::LONG};
const Display::Message invalid_char_error = {
    "                    ", "  Cannot interpret  ",
    "the character:      ", "   Repeat input.    ", Display::Duration::STATIC};
const Display::Message missing_number_error = {
    "                    ", "       Error        ", " Input must contain ",
    "at least one number.", Display::Duration::NORMAL};
const Display::Message missing_phase_error = {
    "                    ", "       Error        ", " Input must contain ",
    "at least one letter.", Display::Duration::NORMAL};
const Display::Message numerical_ending_error = {
    "                    ", "       Error        ", "   Input must end   ",
    "   with a number.   ", Display::Duration::NORMAL};
const Display::Message digits_out_of_range_error = {
    "                    ", "       Error        ", "Number cannot exceed",
    "     2 digits.      ", Display::Duration::NORMAL};
const Display::Message DC_out_of_range_error = {
    "                    ", "       Error        ", " DC value is out of ",
    "  range. Max is 2.  ", Display::Duration::NORMAL};
const Display::Message AC_out_of_range_error = {
    "                    ", "       Error        ", " AC value is out of ",
    " range. Max is 16.  ", Display::Duration::NORMAL};
const Display::Message no_starting_phase_error = {
    "                    ", "       Error        ", "Input sequence must ",
    "begin with a phase. ", Display::Duration::NORMAL};
const Display::Message repeating_phase_error = {
    "                    ", "       Error        ", " Phase is repeated. ",
    "                    ", Display::Duration::NORMAL};
const Display::Message leading_zero_error = {
    "                    ", "       Error        ", "   Remove leading   ",
    "zeros, i.e. 6 vs 06 ", Display::Duration::NORMAL};
const Display::Message unordered_phases_error = {
    "                    ", "       Error        ", "   Phases are not   ",
    "     in order.      ", Display::Duration::NORMAL};
const Display::Message load_idle_state = {
    "Idle state:         ", "AC(3-ph) =          ", "A:                  ",
    "DC:                 ", Display::Duration::STATIC};
}  // namespace Display
