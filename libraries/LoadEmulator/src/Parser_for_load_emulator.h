#ifndef Parser_load_emulator_h
#define Parser_load_emulator_h

#include "Arduino.h"

/*
Max size is determined by longest possible input sequence, e.g.
A11B12C13D5* */
#define INPUT_STR_MAX_SIZE 12

class Parser
{
public:
  Parser();
  void parse(String);
  
  bool shift_reg_data  [48];  // Bool values for 3-phase AC
  byte shift_reg_bytes [6];   // Byte values for 3-phase AC
  int DC_value;
  String load_idle_status;

private:
  void source_correction(void);
  void parse_data(void);
  void create_shift_reg_data(void);
  void create_shift_reg_bytes(void);
  void update_load_idle_status(void);
 
  const int _max_size = INPUT_STR_MAX_SIZE;
  String _input_str;
  int _input_str_len;
  
  int _numerical_values[INPUT_STR_MAX_SIZE];
  bool _A_state, _B_state;
  bool _C_state, _D_state;
  int _A_pos, _B_pos;
  int _C_pos, _D_pos;
  int _A_value, _B_value;
  int _C_value, _D_value;
};

#endif Parser_load_emulator_h
