#include "Parser_for_load_emulator.h"

#include "Arduino.h"

Parser::
Parser() {
  _A_value = 0;
  _B_value = 0;
  _C_value = 0;
  _D_value = 0;
  
  for (int i = 0; i < 48; i++) {
    shift_reg_data[i] = 0;
    shift_reg_bytes[i / 8] = 0x0; 
  }
  DC_value = 0;
  
  load_idle_status  = "Load status         ";
  load_idle_status += "A:0  B:0  C:0       ";
  load_idle_status += "DC:0                ";
  load_idle_status += "                    ";
  
}

void Parser::
parse(String input_str) {
  // Reset states of helper variables for each parse process.
  for (int i = 0; i < _max_size; i++)
    _numerical_values[i] = -1;
  _A_state = false; _B_state = false;
  _C_state = false; _D_state = false;
  _A_pos = -1; _B_pos = -1;
  _C_pos = -1; _D_pos = -1;

  
  _input_str = input_str;
  
  source_correction();
  parse_data();
  create_shift_reg_data();
  create_shift_reg_bytes();
  update_load_idle_status();
}

void Parser::
source_correction() {
  _input_str_len = _input_str.length();
  
  if (_input_str[_input_str_len - 1] == '#') {
    
    _input_str.remove(_input_str_len - 1);
    _input_str_len--;
    Serial.println(F("Parser: Removed ENTER ('#') char."));
    Serial.print(F("Parser: Parsing the given input, "));
    Serial.println(_input_str);
    Serial.println();
  }
  else if (_input_str[_input_str_len - 1] == '*' &&
           _input_str_len > 1) {
    Serial.println(F("Parser: Canceling input, nothing to parse."));
  }
  else if (_input_str[_input_str_len - 1] == '*' &&
           _input_str_len == 1) {
    Serial.println(F("Parser: Resetting relays."));
  }
}

void Parser::
parse_data() {
  
  // Case for cancelling input sequence.
  if (_input_str[_input_str_len - 1] == '*' &&
      _input_str_len > 1) {
    _input_str.remove(0);
    _input_str_len = 0;
  }
  
  for (int i = 0; i < _input_str_len; i++) {
    switch (_input_str[i])
    {
      case '0': _numerical_values[i] = 0; break;
      case '1': _numerical_values[i] = 1; break;
      case '2': _numerical_values[i] = 2; break;
      case '3': _numerical_values[i] = 3; break;
      case '4': _numerical_values[i] = 4; break;
      case '5': _numerical_values[i] = 5; break;
      case '6': _numerical_values[i] = 6; break;
      case '7': _numerical_values[i] = 7; break;
      case '8': _numerical_values[i] = 8; break;
      case '9': _numerical_values[i] = 9; break;
      case 'A': _A_state = true; _A_pos = i; break;
      case 'B': _B_state = true; _B_pos = i; break;
      case 'C': _C_state = true; _C_pos = i; break;
      case 'D': _D_state = true; _D_pos = i; break;
      default: break;
    }
  }
  
  /*
  NOTE ON NUMERICAL VALUES ARRAY:
  When assigning values to the individual phases or DC value, we use
  the numercal values array to convert the numerical characters in the
  input sequence into their corresponding integer values. This is fine
  when the characters are 0 or 2-9, but when it is 1 it requires better
  handling. This is becuase it can refer to the numerical value of the
  number one or the tens digit in the numbers 10-16. For that reason,
  we look at three cases when assigning number values:
  when the character
  1. is 0 or 2-9 (or, equivalently, not equal to 1)
  2. is equal to 1 followed by a letter
  3. is equal to 1 followed by another number
 */

  // A, B, and C are all present in input
  if (_A_state && _B_state && _C_state) {
    // case for ABC#
    if (_B_pos == (_A_pos + 1) && _C_pos == (_B_pos + 1))  {
      if (_numerical_values[_C_pos + 1] != 1) {
        _A_value = _numerical_values[_C_pos + 1];
        _B_value = _A_value;
        _C_value = _A_value;
      }
      else if (_numerical_values[_C_pos + 1] == 1 &&
             ((_C_pos + 1) == (_input_str_len - 1)) ||
               _D_pos == (_C_pos + 2)) {
        _A_value = 1;
        _B_value = 1;
        _C_value = 1;
      }
      else {
        _A_value = 10 + _numerical_values[_C_pos + 2];
        _B_value = _A_value;
        _C_value = _A_value;
      }
    }
    // case for AB#C#
    else if (_B_pos == (_A_pos + 1) && _C_pos != (_B_pos + 1))  {
      // assign A & B values
      if (_numerical_values[_B_pos + 1] != 1) {
        _A_value = _numerical_values[_B_pos + 1];
        _B_value = _numerical_values[_B_pos + 1];
      }
      else if (_numerical_values[_B_pos + 1] == 1 &&
              _C_pos == (_B_pos + 2)) {
        _A_value = 1;
        _B_value = 1;
      }
      else {
        _A_value = 10 + _numerical_values[_B_pos + 2];
        _B_value = 10 + _numerical_values[_B_pos + 2];
      }
      // assign C value
      if (_numerical_values[_C_pos + 1] != 1) {
        _C_value = _numerical_values[_C_pos + 1];
      }
      else if (_numerical_values[_C_pos + 1] == 1 &&
             ((_C_pos + 1) == (_input_str_len - 1) ||
              _D_pos == (_C_pos + 2)) ) {
        _C_value = 1;
      }
      else {
        _C_value = 10 + _numerical_values[_C_pos + 2];
      }
    }
    // case for A#BC#
    else if (_B_pos != (_A_pos + 1) && _C_pos == (_B_pos + 1))  {
      // assign B & C values
      if (_numerical_values[_C_pos + 1] != 1) {
        _B_value = _numerical_values[_C_pos + 1];
        _C_value = _numerical_values[_C_pos + 1];
      }
      else if (_numerical_values[_C_pos + 1] == 1 &&
             ((_C_pos + 1) == (_input_str_len - 1) ||
              _D_pos == (_C_pos + 2))) {
        _B_value = 1;
        _C_value = 1;
      }
      else {
        _B_value = 10 + _numerical_values[_C_pos + 2];
        _C_value = 10 + _numerical_values[_C_pos + 2];
      }
      // assign A value
      if (_numerical_values[_A_pos + 1] != 1) {
        _A_value = _numerical_values[_A_pos + 1];
      }
      else if (_numerical_values[_A_pos + 1] == 1 &&
               _B_pos  == (_A_pos + 2)) {
        _A_value = 1;
      }
      else {
        _A_value = 10 + _numerical_values[_A_pos + 2];
      }
    }
    // case for A#B#C#
    else {
      if (_numerical_values[_A_pos + 1] != 1)
        _A_value = _numerical_values[_A_pos + 1];
      else if (_numerical_values[_A_pos + 1] == 1 &&
               _numerical_values[_A_pos + 2] == -1)
        _A_value = 1;
      else
        _A_value = 10 + _numerical_values[_A_pos + 2];
      
      if (_numerical_values[_B_pos + 1] != 1)
        _B_value = _numerical_values[_B_pos + 1];
      else if (_numerical_values[_B_pos + 1] == 1 &&
               _numerical_values[_B_pos + 2] == -1)
        _B_value = 1;
      else
        _B_value = 10 + _numerical_values[_B_pos + 2];
      
      if (_numerical_values[_C_pos + 1] != 1)
        _C_value = _numerical_values[_C_pos + 1];
      else if (_numerical_values[_C_pos + 1] == 1 &&
               _numerical_values[_C_pos + 2] == -1)
        _C_value = 1;
      else
        _C_value = 10 + _numerical_values[_C_pos + 2];
    }
  }
  
  // One or two AC phases are present in input 
  else {
    if (_A_state) {
      if (_numerical_values[_A_pos + 1] != 1 && 
          _numerical_values[_A_pos + 1] != -1)        // e.g. A9
        _A_value = _numerical_values[_A_pos + 1];
      else if (_numerical_values[_A_pos + 1] == -1)   // e.g. AB3
        _A_value = _numerical_values[_A_pos + 2];
      else if (_numerical_values[_A_pos + 1] == 1 &&  
             ((_A_pos + 1) == (_input_str_len - 1)) ||
               _B_pos == (_A_pos + 2) || 
               _C_pos == (_A_pos + 2) ||
               _D_pos == (_A_pos + 2) )               // e.g. A1 or A1B2
        _A_value = 1;
      else
        _A_value = 10 + _numerical_values[_A_pos + 2];
    }
    if (_B_state) {
      if (_numerical_values[_B_pos + 1] != 1 &&
          _numerical_values[_B_pos + 1] != -1)
        _B_value = _numerical_values[_B_pos + 1];
      else if (_C_state || _D_state &&
               _numerical_values[_A_pos + 1] == -1) {
        _B_value = _numerical_values[_B_pos + 2];
      }
      else if (_numerical_values[_B_pos + 1] == 1 &&
             ((_B_pos + 1) == (_input_str_len - 1)) ||
               _C_pos == (_B_pos + 2) ||
               _D_pos == (_B_pos + 2))
        _B_value = 1;
      else
        _B_value = 10 + _numerical_values[_B_pos + 2];
    }
    if (_C_state) {
      if (_numerical_values[_C_pos + 1] != 1 && 
          _numerical_values[_C_pos + 1] !=-1)
        _C_value = _numerical_values[_C_pos + 1];
      else if (_D_state &&
               _numerical_values[_C_pos + 1] == -1) {
        _C_value = _numerical_values[_C_pos + 2];
      }
      else if (_numerical_values[_C_pos + 1] == 1 &&
             ((_C_pos + 1) == (_input_str_len - 1)) ||
               _D_pos == (_C_pos + 2))
        _C_value = 1;
      else
        _C_value = 10 + _numerical_values[_C_pos + 2];
    }
  }
  
  if (_D_state) {
    if (_numerical_values[_D_pos + 1] != 1)
      _D_value = _numerical_values[_D_pos + 1];
    else if (_numerical_values[_D_pos + 1] == 1 &&
            (_D_pos + 1) == (_input_str_len - 1))
      _D_value = 1;
    else
      _D_value = 10 + _numerical_values[_D_pos + 2];
    
    DC_value = _D_value;
  }    
  
  if (_input_str == "*") {
    _A_value = 0;
    _B_value = 0;
    _C_value = 0;
    _D_value = 0;
  }
  
  // Print current values for each phase.
  Serial.println("Parser: Values for each phase are:");
  Serial.print("A:"); Serial.print(_A_value); Serial.print(' ');
  Serial.print("B:"); Serial.print(_B_value); Serial.print(' ');
  Serial.print("C:"); Serial.print(_C_value); Serial.print(' ');
  Serial.print("D:"); Serial.print(_D_value); Serial.print(' ');
  Serial.println(); Serial.println(); 
}

void Parser::
create_shift_reg_data() {
  Serial.println("Parser: Shift register data values are");
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 16; j++) {
      if (i == 0)
        if (j < _A_value)
          shift_reg_data[16 * i + j] = true;
        else
          shift_reg_data[16 * i + j] = false;
      if (i == 1)
        if (j < _B_value)
          shift_reg_data[16 * i + j] = true;
        else
          shift_reg_data[16 * i + j] = false;
      if (i == 2)
        if (j < _C_value)
          shift_reg_data[16 * i + j] = true;
        else
          shift_reg_data[16 * i + j] = false;
      Serial.print(shift_reg_data[16 * i + j]);
      Serial.print(' ');
    }
    Serial.println();
  }
  Serial.println();
}

void Parser::
create_shift_reg_bytes() {
  Serial.println(F("Parser: Shift register byte values are"));
  byte temp_byte;
  for (int i = 0; i < 6;  i++) {
    temp_byte = B00000000;
    for (int j = 0; j < 8; j++) {
      temp_byte = shift_reg_data[i*8+j] | temp_byte;
      if (j < 7)
        temp_byte = temp_byte << 1;
    }
    Serial.println(temp_byte, BIN);
  }
}

void Parser::
update_load_idle_status(void) {
  load_idle_status  = "Load status         ";
  
  
  load_idle_status += "A:";
  if (_A_value < 10) {
    load_idle_status += char(_A_value + 48);
    load_idle_status += "  ";
  }
  else {
    load_idle_status += "1";
    load_idle_status += char((_A_value - 10) + 48);
    load_idle_status += " ";
  }
  
  load_idle_status += "B:";
  if (_B_value < 10) {
    load_idle_status += char(_B_value + 48);
    load_idle_status += "  ";
  }
  else {
    load_idle_status += "1";
    load_idle_status += char((_B_value - 10) + 48);
    load_idle_status += " ";
  }
  
  load_idle_status += "C:";
  if (_C_value < 10) {
    load_idle_status += char(_C_value + 48);
    load_idle_status += "  ";
  }
  else {
    load_idle_status += "1";
    load_idle_status += char((_C_value - 10) + 48);
    load_idle_status += " ";
  }
  int len = load_idle_status.length();
  for (int i = 0; i < 40 - len; i++) {
    load_idle_status += " ";
  }
    
  
  load_idle_status += "DC:";
  load_idle_status += char(_D_value + 48);
  len = load_idle_status.length();
  for (int i = 0; i < 60 - len; i++) {
    load_idle_status += " ";
  }
  
  
  load_idle_status += "                    ";
}
