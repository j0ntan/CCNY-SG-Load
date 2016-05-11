#ifndef Keypad_load_emulator_h
#define Keypad_load_emulator_h

#include "Arduino.h"

/*
Refer to report for names of keypad pins + connection to Arduino input
pins. */
#define ROW1_PIN 37
#define ROW2_PIN 36
#define ROW3_PIN 35
#define ROW4_PIN 34
#define COL1_PIN 33
#define COL2_PIN 32
#define COL3_PIN 31
#define COL4_PIN 30

/*
Milliseconds between reads. Lower is more sensitive. */
#define SENSITIVITY  80

/*
Threshold in milliseconds. Defines HOLD(button) vs PRESS(button). */
#define HOLD_THRESHOLD 800

class Keypad
{
public:
  Keypad();
  void begin(void);
  void begin(int R1, int R2, int R3, int R4,
             int C1, int C2, int C3, int C4);
  bool isPressed(void);
  char getKey(void);
  
  bool heldStatus;


private:
  void toggle_keys(bool (&keys_are_pressed) [4][4]);
  void pause_until_threshold(void);
  char find_single_key();
  
  int _row1, _row2, _row3, _row4,
      _col1, _col2, _col3, _col4;
  int _keypress_count, _row_pressed, _col_pressed;
  bool _key_is_pressed;
  char _printed_keys[16], _pressed_key_char;
  int _ms_held;
};

#endif