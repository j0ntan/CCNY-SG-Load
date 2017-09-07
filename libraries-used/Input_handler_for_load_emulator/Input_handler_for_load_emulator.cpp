#include "Arduino.h"
#include "Input_handler_for_load_emulator.h"

Input_handler::
Input_handler() {
  _multi_keypress_flag = '!';
  _empty_char_flag = '?';
  _valid_input_count = 0;
  
  for (int i = 0; i < 10; i++) {
    // offset due to data type conversion
    _acceptable_chars[i] = char(i + 48);                  
  }
  
  _current_keypress = '.';
  _prev_keypress = '.';
  
  
  _A_pressed = false;
  _B_pressed = false;
  _C_pressed = false;
  _D_pressed = false;
  
  _A_count = 0;
  _B_count = 0;
  _C_count = 0;
  _D_count = 0;
  
  _A_pos = -1;
  _B_pos = -1;
  _C_pos = -1;
  _D_pos = -1;
  
  _number_count = 0;
  ENTER_or_CANCEL_pressed = false;
}

//---------Pubic Methods---------//
void Input_handler::
accept_keypress(char keypress) {
  _current_keypress = keypress;
  _input_source = "Keypad";
  
  if (check_for_valid_input() == true) {
    _prev_keypress = _current_keypress;
    update_input_sequence();
    _valid_input_count++;
    
    if (_current_keypress == '*' || _current_keypress == '#') {
      if (_current_keypress == '*' && _valid_input_count == 1) {
        _status = "Pressed RESET. Resetting all relays.";
        Serial.println(_status);
        LCD_status  = "   Pressed RESET.   ";
        LCD_status += "     Resetting      ";
        LCD_status += "     all relays.    ";
        LCD_status += "                    ";
      }
      else if (_current_keypress == '*' && _valid_input_count != 1) {
        _status = "Pressed CANCEL. Resetting input sequence.";
        Serial.println(_status);
        LCD_status  = "  Pressed CANCEL.   ";
        LCD_status += "     Resetting      ";
        LCD_status += "  input sequence.   ";
        LCD_status += "                    ";
      }
      else {
        _status = "Pressed ENTER. Activating relays.";
        Serial.println(_status);
        LCD_status  = "   Pressed ENTER.   ";
        LCD_status += "     Activating     ";
        LCD_status += "       relays.      ";
        LCD_status += "                    ";
      }
      //Serial.println("Serial: Updating public input sequence.");
      input_sequence = _input_sequence + '\0';
      clear_input_sequence();
      ENTER_or_CANCEL_pressed = true;
    }
  }
  else {
    /*
    Undo some of the actions from the switch statement in the function 
    check_for_valid_input due to an invalid keypress input value. */ 
    switch (_current_keypress) {
      case '0': _number_count--; break;
      case '1': _number_count--; break;
      case '2': _number_count--; break;
      case '3': _number_count--; break;
      case '4': _number_count--; break;
      case '5': _number_count--; break;
      case '6': _number_count--; break;
      case '7': _number_count--; break;
      case '8': _number_count--; break;
      case '9': _number_count--; break;
      case 'A':
        _A_pressed = false; _A_count--;
        _A_pos = -1; break;
      case 'B':
        _B_pressed = false; _B_count--;
        _B_pos = -1; break;
      case 'C': 
        _C_pressed = false; _C_count++; 
        _C_pos = -1; break;
      case 'D':
        _D_pressed = false; _D_count++; 
        _D_pos = -1; break;
      default:
        break;
    }
    // Print the error message.
    Serial.println(_status);
  }
}

void Input_handler::
accept_serial_port_input(void) {
  _input_source = "Serial Monitor";
  
  _input_index = 0;
  while (Serial.available() > 0) {
    _input_sequence[_input_index] = char(Serial.read());
    if (_input_index != (SIZE-1))
      _input_index++;
  }
  _input_sequence[_input_index] = '#';
  
  input_sequence = _input_sequence + '\0';
  int _size = input_sequence.length();
  
  
  Serial.print("Accepted serial input: ");
  for ( _input_index = 0; _input_index < _size; _input_index++) {
    if (_input_sequence[_input_index] != _empty_char_flag) {
      Serial.print(_input_sequence[_input_index]);
    }
  }
  Serial.println(); 
}

String Input_handler::
send_input_to_parser() {
  ENTER_or_CANCEL_pressed = false; // Reset this to get next input.
  return _input_sequence + '\0';
  // To be continued... 
}


//--------Private Methods--------//
bool Input_handler::
check_for_valid_input() {
  if (check_for_correct_format() && 1) {
    // To be continued...
    return true;      // All checks have been passed :)
  }
  else
    return false; 
}

bool Input_handler::
check_for_correct_format() {
  
  switch (_current_keypress) {
    case '0': _number_count++; break;
    case '1': _number_count++; break;
    case '2': _number_count++; break;
    case '3': _number_count++; break;
    case '4': _number_count++; break;
    case '5': _number_count++; break;
    case '6': _number_count++; break;
    case '7': _number_count++; break;
    case '8': _number_count++; break;
    case '9': _number_count++; break;
    case 'A':
      _A_pressed = true; _A_count++;
      _A_pos = _valid_input_count; break;
    case 'B':
      _B_pressed = true; _B_count++; 
      _B_pos = _valid_input_count; break;
    case 'C': 
      _C_pressed = true; _C_count++; 
      _C_pos = _valid_input_count; break;
    case 'D':
      _D_pressed = true; _D_count++; 
      _D_pos = _valid_input_count; break;
    default:
      break;
  }
  
  // First input must be a letter or *.
  if (_valid_input_count == 0 && 
     (_current_keypress != 'A' && _current_keypress != 'B' && 
      _current_keypress != 'C' && _current_keypress != 'D' &&
      _current_keypress != '*')  ) {
    _status = "First press A,B,C,D or *";
    LCD_status  = "                    ";
    LCD_status += "    First press     ";
    LCD_status += "    A,B,C,D or *.   ";
    LCD_status += "                    ";
    return false;
  }
  
  // Must follow: A before B, before C, before D
  else if (
    (_current_keypress == 'A'&&(_B_pressed || _C_pressed || _D_pressed)) ||
    (_current_keypress == 'B' && (_C_pressed || _D_pressed)) ||
    (_current_keypress == 'C' && (_D_pressed)) ) {
    _status = "ABCD must be in order.";
    LCD_status  = "                    ";
    LCD_status += "    ABCD must be    ";
    LCD_status += "     in order.      ";
    LCD_status += "                    ";
    return false;  
  }
  
  // Must have a single keypress of A,B,C, or D
  else if ( _A_count > 1 || _B_count > 1 || 
            _C_count > 1 || _D_count > 1 ) {
    _status = "Press A, B, C, or D only once.";
    LCD_status  = "                    ";
    LCD_status += "   Press A, B, C,   ";
    LCD_status += "   or D only once.  ";
    LCD_status += "                    ";
    return false;
  }
  
  // Do not allow inputs of 17, 18, 19 (0-16 is allowed).
  else if ((_prev_keypress == '1' && _current_keypress == '7') ||
           (_prev_keypress == '1' && _current_keypress == '8') ||
           (_prev_keypress == '1' && _current_keypress == '9')) {
    _status = "Inputs must be in range 0-16.";
    LCD_status  = "                    ";
    LCD_status += "   Inputs must be   ";
    LCD_status += "   in range 0-16.   ";
    LCD_status += "                    ";
    return false;
  }
  
  // Do not allow inputs greater than 19.
  else if((_prev_keypress == '2' || _prev_keypress == '3' ||
           _prev_keypress == '4' || _prev_keypress == '5' ||
           _prev_keypress == '6' || _prev_keypress == '7' ||
           _prev_keypress == '8' || _prev_keypress == '9') && 
          (_current_keypress == '0' || _current_keypress == '1' ||
           _current_keypress == '2' || _current_keypress == '3' ||
           _current_keypress == '4' || _current_keypress == '5' ||
           _current_keypress == '6' || _current_keypress == '7' ||
           _current_keypress == '8' || _current_keypress == '9' )) {
    _status = "Inputs must be in range 0-16.";
    LCD_status  = "                    ";
    LCD_status += "   Inputs must be   ";
    LCD_status += "   in range 0-16.   ";
    LCD_status += "                    ";
    return false;
  }
  
  // Two digit inputs must have value greater than 9, i.e. '1' vs '01'.
  else if (_prev_keypress == '0' && 
          (_current_keypress == '0' || _current_keypress == '1' ||
           _current_keypress == '2' || _current_keypress == '3' ||
           _current_keypress == '4' || _current_keypress == '5' ||
           _current_keypress == '6' || _current_keypress == '7' ||
           _current_keypress == '8' || _current_keypress == '9' )) {
    _status = "Incorrect format. Use '6' instead of '06'.";
    LCD_status  = " Incorrect format.  ";
    LCD_status += "  Use '6' instead   ";
    LCD_status += "      of '06'.      ";
    LCD_status += "                    ";
    return false;
  }
  
  // There must always be a number pressed before ENTER(#).
  else if (_current_keypress == '#' &&
          (_prev_keypress != '0' && _prev_keypress != '1' &&
           _prev_keypress != '2' && _prev_keypress != '3' &&
           _prev_keypress != '4' && _prev_keypress != '5' &&
           _prev_keypress != '6' && _prev_keypress != '7' &&
           _prev_keypress != '8' && _prev_keypress != '9')) {
    _status = "Missing a number before pressing ENTER (#).";
    LCD_status  = "  Missing a number  ";
    LCD_status += "  before pressing   ";
    LCD_status += "     ENTER (#).     ";
    LCD_status += "                    ";
    return false;
  }
  
  // Check for mutli-keypress
  else if (_current_keypress == _multi_keypress_flag) {
    _status =  "Multiple keys were pressed. ";     // Trying to avoid
    _status += "Please press one key at a time.";  // long single lines
    LCD_status  = " Multiple keys were ";
    LCD_status += "  pressed. Please   ";
    LCD_status += "   press one key    ";
    LCD_status += "     at a time.     ";
    return false;
  }
  
  else
    return true;
}

void Input_handler::
update_input_sequence(void) {
  _input_sequence[_valid_input_count] = _current_keypress;
  
  Serial.print("Input sequence: ");
  for (int i = 0; i <= _valid_input_count; i++) {
    Serial.print(_input_sequence[i]);
    Serial.print(' ');
  }
  Serial.println();
}

void Input_handler::
clear_input_sequence() {
  _valid_input_count = 0;
  _current_keypress = '.';
  _prev_keypress = '.';
  
  
  _A_pressed = false;
  _B_pressed = false;
  _C_pressed = false;
  _D_pressed = false;
  
  _A_count = 0;
  _B_count = 0;
  _C_count = 0;
  _D_count = 0;
  
  _A_pos = -1;
  _B_pos = -1;
  _C_pos = -1;
  _D_pos = -1;
  
  _number_count = 0;
  
  for (int i = 0; i < _input_size; i++) {
    _input_sequence[i] = '\0';
  }
}
