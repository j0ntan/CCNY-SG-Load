#ifndef Input_handler_load_emulator_h
#define Input_handler_load_emulator_h

#include "Arduino.h"

#define SIZE 12 // max characters that will be stored/handled

class Input_handler 
{
public:
  Input_handler();
  void accept_keypress(char keypress);
  void accept_serial_port_input(void);
  String send_input_to_parser(void);
  
  String LCD_status, input_sequence;
  bool ENTER_or_CANCEL_pressed;
  
private:
  bool check_for_valid_input(void);
  bool check_for_correct_format(void);
  void update_input_sequence(void);
  void clear_input_sequence(void);
  
  
  const int _input_size = SIZE;
  int _input_index;                   // index to character in input seq.
  int _valid_input_count;
  char _current_keypress, _prev_keypress;
  char _input_sequence[SIZE], _acceptable_chars[18];
  char _multi_keypress_flag, _empty_char_flag;
  String _status, _input_source;
  bool _A_pressed, _B_pressed;
  bool _C_pressed, _D_pressed;
  int _A_count, _B_count;
  int _C_count, _D_count;
  int _A_pos, _B_pos;
  int _C_pos, _D_pos;
  int _number_count;
};

#endif
