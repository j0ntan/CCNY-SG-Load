#include "Arduino.h"
#include "Input_handler_for_load_emulator.h"

Input_handler::
Input_handler() {
  // Initialize keypress variables: 
  clear_keypad_vars();
  _multi_keypress_flag = '!';
  ENTER_or_CANCEL_pressed = false;
  
  // Initialize serial input variables:
  clear_serial_vars();
  _empty_char_flag = '?';
}

//---------Pubic Methods---------//
void Input_handler::
accept_keypress(char keypress) {
  _current_keypress = keypress;
  
  if (check_for_valid_keypress() == true) {
    _prev_keypress = _current_keypress;
    update_input_sequence();
    _accepted_input_count++;
    
    if (_current_keypress == '*' || _current_keypress == '#') {
      if (_current_keypress == '*' && _accepted_input_count == 1) {
        Serial.print(F("Input handler: Pressed RESET. "));
        Serial.println(F("Resetting all relays."));
        LCD_status  = "   Pressed RESET.   ";
        LCD_status += "     Resetting      ";
        LCD_status += "     all relays.    ";
        LCD_status += "                    ";
      }
      else if (_current_keypress == '*' && _accepted_input_count != 1) {
        Serial.print(F("Input handler: Pressed CANCEL. "));
        Serial.println(F("Resetting input sequence."));
        LCD_status  = "  Pressed CANCEL.   ";
        LCD_status += "     Resetting      ";
        LCD_status += "  input sequence.   ";
        LCD_status += "                    ";
      }
      else {
        Serial.print(F("Input handler: Pressed ENTER. "));
        Serial.println(F("Activating relays."));
        LCD_status  = "   Pressed ENTER.   ";
        LCD_status += "     Activating     ";
        LCD_status += "       relays.      ";
        LCD_status += "                    ";
      }
      String temp_input =  _input_sequence + '\0';
      clear_keypad_vars();
      input_sequence = temp_input;
      ENTER_or_CANCEL_pressed = true;
    }
    
    else {
      // Do nothing. Detailed error message will be 
      // displayed upon validation fail.
    }
  }
  else {
    /*
    Undo some of the actions from the switch statement in the function 
    check_for_valid_keypress due to invalid keypress input value. */ 
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
      case 'A': _A_count--; break;
      case 'B': _B_exists = false; _B_count--; break;
      case 'C': _C_exists = false; _C_count++; break;
      case 'D': _D_exists = false; _D_count++; break;
      default:
        break;
    }
    show_error_msg = true;
  }
}

void Input_handler::
accept_serial_port_input(void) {  
  _temp_input = "";
  while (Serial.available() > 0) {
    _temp_input += char(Serial.read());
  }
  _temp_size = _temp_input.length();
  Serial.print(F("Input handler: Received serial input "));
  Serial.println(_temp_input);
  LCD_status  = "                    ";
  LCD_status += "     Received a     ";
  LCD_status += "    serial input.   ";
  LCD_status += "                    ";
  
  if (check_for_valid_serial_input() == true) {
    Serial.println(F("Input handler: Sending serial input."));
    LCD_status  = "                    ";
    LCD_status += "      Sending       ";
    LCD_status += "    serial input.   ";
    LCD_status += "                    ";
    input_sequence = _temp_input;
  }
  else {
    /* 
    Pass a canceled input sequence upon failed validation. 
    Detailed error message will be displayed upon 
    validation fail. */
    show_error_msg = true;
    input_sequence = "A*"; 
  }
  clear_serial_vars();
}

void Input_handler::
accecpt_XBee_input(void) {
  // To be added in the future.
}


//--------Private Methods--------//
bool Input_handler::
check_for_valid_keypress(void) {
  
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
    case 'A': _A_count++; break;
    case 'B': _B_exists = true; _B_count++;  break;
    case 'C': _C_exists = true; _C_count++; break;
    case 'D': _D_exists = true; _D_count++; break;
    default: break;
  }
  
  // First input must be a letter or *.
  if (_accepted_input_count == 0 && 
     (_current_keypress != 'A' && _current_keypress != 'B' && 
      _current_keypress != 'C' && _current_keypress != 'D' &&
      _current_keypress != '*') ) {
    Serial.println(F("Input handler: First press A,B,C,D or *"));
    LCD_status  = "                    ";
    LCD_status += "    First press     ";
    LCD_status += "    A,B,C,D or *.   ";
    LCD_status += "                    ";
    return false;
  }
  
  // Must follow: A before B, before C, before D
  else if (
    (_current_keypress == 'A'&&(_B_exists || _C_exists || _D_exists)) ||
    (_current_keypress == 'B' && (_C_exists || _D_exists)) ||
    (_current_keypress == 'C' && (_D_exists)) ) {
    Serial.println(F("Input handler: ABCD must be in order."));
    LCD_status  = "                    ";
    LCD_status += "    ABCD must be    ";
    LCD_status += "     in order.      ";
    LCD_status += "                    ";
    return false;
  }
  
  // Must have a single keypress of A,B,C, or D
  else if ( _A_count > 1 || _B_count > 1 || 
            _C_count > 1 || _D_count > 1 ) {
    Serial.println(F("Input handler: Press A, B, C, or D only once."));
    LCD_status  = "                    ";
    LCD_status += "   Press A, B, C,   ";
    LCD_status += "   or D only once.  ";
    LCD_status += "                    ";
    return false;
  }
  
  // Do not allow inputs of 17, 18, 19 (0-16 is allowed).
  else if (_prev_keypress == '1' && 
          (_current_keypress == '7' || _current_keypress == '8' ||
           _current_keypress == '9')) {
    Serial.println(F("Input handler: Inputs must be in range 0-16."));
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
    Serial.println(F("Inputs must be in range 0-16."));
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
    Serial.print(F("Input handler: Incorrect format. ")); 
    Serial.println(F("Use '6' instead of '06'."));
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
    Serial.print(F("Input handler: Missing a number before ")); 
    Serial.println(F("pressing ENTER (#)."));
    LCD_status  = "  Missing a number  ";
    LCD_status += "  before pressing   ";
    LCD_status += "     ENTER (#).     ";
    LCD_status += "                    ";
    return false;
  }
  
  // Check for mutli-keypress
  else if (_current_keypress == _multi_keypress_flag) {
    Serial.print(F( "Input handler: Multiple keys were pressed. "));
    Serial.println(F("Please press one key at a time."));
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
  _input_sequence[_accepted_input_count] = _current_keypress;
  
  Serial.print("Input sequence: ");
  for (int i = 0; i <= _accepted_input_count; i++) {
    Serial.print(_input_sequence[i]);
    Serial.print(' ');
  }
  Serial.println();

  input_sequence  = "                    ";
  input_sequence += "Current input:      ";
  input_sequence += _input_sequence + '\0';
  input_sequence += "                    ";
}

void Input_handler::
clear_keypad_vars(void) {
  _accepted_input_count = 0;
  _current_keypress = '.';
  _prev_keypress = '.';
  show_error_msg = false;
  
  
  _B_exists = false;
  _C_exists = false;
  _D_exists = false;
  
  _A_count = 0;
  _B_count = 0;
  _C_count = 0;
  _D_count = 0;
  
  
  _number_count = 0;
  
  for (int i = 0; i < _input_size; i++) {
    _input_sequence[i] = '\0';
  }

  input_sequence  = "                    ";
  input_sequence += "  Nothing in input  ";
  input_sequence += "   sequence yet.    ";
  input_sequence += "                    ";
}

bool Input_handler::
check_for_valid_serial_input (void) {
  while (_counter < _temp_size) {
    switch (_temp_input[_counter]) {
      case '0':  check_numbers_range(0); break;
      case '1':  check_numbers_range(1); break;
      case '2':  check_numbers_range(2); break;
      case '3':  check_numbers_range(3); break;
      case '4':  check_numbers_range(4); break;
      case '5':  check_numbers_range(5); break;
      case '6':  check_numbers_range(6); break;
      case '7':  check_numbers_range(7); break;
      case '8':  check_numbers_range(8); break;
      case '9':  check_numbers_range(9); break;
      case 'A': _A_count++; break;
      case 'B': _B_exists = true; _B_count++; break;
      case 'C': _C_exists = true; _C_count++; break;
      case 'D': _D_exists = true; _D_count++; break;
      case '*': _Reset_exists = true; break;
      default: 
        _counter = _temp_size; _illegal_char_exists = true;
        _illegal_char = _temp_input[_counter] ;break;
    }
    _counter++;
  }
  
  
  if (_illegal_char_exists == true) {
    Serial.print(F("Input handler: Found the illegal character '"));
    Serial.print(_illegal_char);
    Serial.println(F("' in the input."));
    LCD_status  = "      Error in      ";
    LCD_status += "    serial input.   "; 
    LCD_status += "  Canceling input.  ";
    LCD_status += "                    ";
    return false;
  }
  
  else if (_Reset_exists && _temp_size == 1) {
    Serial.print(F("Input handler: Pressed RESET. "));
    Serial.println(F("Resetting all relays."));
    LCD_status  = "   Pressed RESET.   ";
    LCD_status += "     Resetting      ";
    LCD_status += "     all relays.    ";
    LCD_status += "                    ";
    return true;
  }
  
  else if (_temp_size > 11) {
    Serial.println(F("Input handler: Too many inputs."));
    LCD_status  = "                    ";
    LCD_status += "  Too many inputs.  "; 
    LCD_status += "                    ";
    LCD_status += "                    ";
    return false;
  }
  
  // Do not allow inputs of 17, 18, 19 (0-16 is allowed).
  // Do not allow inputs greater than 19.
  // Two digit inputs must have value greater than 9, i.e. '1' vs '01'.
  else if (_numbers_outof_range == true) {
    // Error message already set and printed to monitor.
    return false;
  }
  
  // First input must be a letter or *.
  else if (_temp_input[0] != 'A' && _temp_input[0] != 'B' && 
           _temp_input[0] != 'C' && _temp_input[0] != 'D' &&
           _temp_input[0] != '*') {
    Serial.println(F("Input handler: First type A,B,C,D or *"));
    LCD_status  = "                    ";
    LCD_status += "     First type     ";
    LCD_status += "    A,B,C,D or *.   ";
    LCD_status += "                    "; 
    return false;
  }
  
  // Must follow: A before B, before C, before D
  else if ( ((_A_pos > _B_pos) && _B_exists) || 
            ((_A_pos > _C_pos) && _C_exists) ||
            ((_A_pos > _D_pos) && _D_exists) ||
            ((_B_pos > _C_pos) && _C_exists) || 
            ((_B_pos > _D_pos) && _D_exists) || 
            ((_C_pos > _D_pos) && _D_exists) ) {
    Serial.println(F("Input handler: ABCD must be in order."));
    LCD_status  = "                    ";
    LCD_status += "    ABCD must be    ";
    LCD_status += "     in order.      ";
    LCD_status += "                    ";
    return false;
  }
  
  // Must have a single keypress of A,B,C, or D
  else if ( _A_count > 1 || _B_count > 1 || 
            _C_count > 1 || _D_count > 1 ) {
    Serial.println(F("Input handler: Type A, B, C, or D only once."));
    LCD_status  = "                    ";
    LCD_status += "    Type A, B, C,   ";
    LCD_status += "   or D only once.  ";
    LCD_status += "                    ";
    return false;
  }
  
  else
    return true;  
}

void Input_handler::
clear_serial_vars(void) {
  _temp_input = "";
  _temp_size = 0;
  _illegal_char_exists = false;
  _illegal_char = '\0';
  _Reset_exists = false;
  _numbers_outof_range = false;
  _counter = 0;
  show_error_msg = false;
    
  _A_pos = -1;
  _B_pos = -1;
  _C_pos = -1;
  _D_pos = -1;
  
  _B_exists = false;
  _C_exists = false;
  _D_exists = false;
  
  _A_count = 0;
  _B_count = 0;
  _C_count = 0;
  _D_count = 0;
}
 
void Input_handler::
check_numbers_range(int num) {
  // Do not allow inputs of 17, 18, 19 (0-16 is allowed).
  if (num == 1 && (_counter != (_temp_size - 1)) &&
     (_temp_input[_counter + 1] == '7' || 
      _temp_input[_counter + 1] == '8' ||
      _temp_input[_counter + 1] == '9')) {
    Serial.println(F("Input handler: Inputs must be in range 0-16."));
    LCD_status  = "                    ";
    LCD_status += "   Inputs must be   ";
    LCD_status += "   in range 0-16.   ";
    LCD_status += "                    ";
    _numbers_outof_range = true;
  }
  
  // Do not allow inputs greater than 19.
  else if ((_counter != (_temp_size - 1)) && num != 0 && 
           (_temp_input[_counter + 1] != 'B' || 
            _temp_input[_counter + 1] != 'C' ||
            _temp_input[_counter + 1] != 'D')) {
    Serial.println(F("Input handler: Inputs must be in range 0-16."));
    LCD_status  = "                    ";
    LCD_status += "   Inputs must be   ";
    LCD_status += "   in range 0-16.   ";
    LCD_status += "                    ";
    _numbers_outof_range = true;
  }
  
  // Two digit inputs must have value greater than 9, i.e. '1' vs '01'.
  else if ((_counter != (_temp_size - 1)) && num == 0  && 
           (_temp_input[_counter + 1] != 'B' || 
            _temp_input[_counter + 1] != 'C' ||
            _temp_input[_counter + 1] != 'D')) {
    Serial.print(F("Input handler: Incorrect format. ")); 
    Serial.println(F("Use '6' instead of '06'."));
    LCD_status  = " Incorrect format.  ";
    LCD_status += "  Use '6' instead   ";
    LCD_status += "      of '06'.      ";
    LCD_status += "                    ";
    _numbers_outof_range = true;
  }
  
  else
    _numbers_outof_range = false;
}
