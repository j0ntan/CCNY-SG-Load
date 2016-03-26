#ifndef Input_handler_load_emulator_h
#define Input_handler_load_emulator_h

#include "Arduino.h"

#define SIZE 12 // max characters that will be stored/handled

class Input_handler 
{
public:
  // ORIGINAL FUNCTIONS
  Input_handler();
  void accept_keypress(char keypress);
  void accept_serial_port_input(void);
  void accecpt_XBee_input(void);
    
  
  String LCD_status; 
  String input_sequence;
  bool ENTER_or_CANCEL_pressed;
  bool show_error_msg;
  
  
private:
  bool check_for_valid_keypress(void);
  void update_input_sequence(void);
  void clear_keypad_vars(void);
  bool check_for_valid_serial_input(void);
  void clear_serial_vars(void);
  void check_numbers_range(int);
  
  
  const int _input_size = SIZE;
  
  // Keypad variables
  int _accepted_input_count;
  char _current_keypress, _prev_keypress;
  char _input_sequence[SIZE];
  char _multi_keypress_flag, _empty_char_flag;
  int _number_count;
  
  // Serial input variables
  String _temp_input;
  int _temp_size;
  bool _illegal_char_exists;
  char _illegal_char;
  int _A_pos, _B_pos, _C_pos, _D_pos;
  int _counter;
  bool _Reset_exists;
  bool _numbers_outof_range;
  
  // Keypad & Serial variables
  bool _B_exists, _C_exists, _D_exists;
  int _A_count, _B_count, _C_count, _D_count;
};

#endif
