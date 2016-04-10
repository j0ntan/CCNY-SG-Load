#ifndef Keypad_load_emulator_h
#define Keypad_load_emulator_h

#include "Arduino.h"

#define ROW1_PIN 30
#define ROW2_PIN 31
#define ROW3_PIN 32
#define ROW4_PIN 33
#define COL1_PIN 34
#define COL2_PIN 35
#define COL3_PIN 36
#define COL4_PIN 37

#define SENSITIVITY  80    // Milliseconds between reads.
                           // Lower is more sensitive.


class Keypad
{
public:
  Keypad();
  void set_pins(int R1, int R2, int R3, int R4, 
                int C1, int C2, int C3, int C4);
  void initialize(void);
  bool check_any_press(void);
  char find_pressed_key(void);
  
  
private:
  void toggle_keys(bool (&keys_are_pressed) [4][4]);
  void pause_until_stopped_pressing(void);
  char find_single_key();
  
  int _row1, _row2, _row3, _row4,
      _col1, _col2, _col3, _col4;
  int _keypress_count, _row_pressed, _col_pressed;
  bool _key_is_pressed;
  char _printed_keys[16], _pressed_key_char;
};

#endif
