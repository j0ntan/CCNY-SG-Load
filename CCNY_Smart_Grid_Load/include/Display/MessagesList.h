#ifndef DISPLAY_MESSAGESLIST_H
#define DISPLAY_MESSAGESLIST_H

#include "Message.h"

namespace Display {
extern const Display::Message caution_1;
extern const Display::Message caution_2;
extern const Display::Message caution_3;

extern const Display::Message initialization_1;
extern const Display::Message initialization_2;
extern const Display::Message initialization_3;

extern const Display::Message load_reset;
extern const Display::Message cancel_sequence;

extern const Display::Message keypad_sequence;

extern const Display::Message dspace_sequence;

extern const Display::Message SD_file_open_error;

// parsing error messages
extern const Display::Message invalid_char_error;
extern const Display::Message missing_number_error;
extern const Display::Message missing_phase_error;
extern const Display::Message numerical_ending_error;
extern const Display::Message digits_out_of_range_error;
extern const Display::Message DC_out_of_range_error;
extern const Display::Message AC_out_of_range_error;
extern const Display::Message no_starting_phase_error;
extern const Display::Message repeating_phase_error;
extern const Display::Message leading_zero_error;
extern const Display::Message unordered_phases_error;

extern const Display::Message load_idle_state;
}  // namespace Display

#endif  // DISPLAY_MESSAGESLIST_H
