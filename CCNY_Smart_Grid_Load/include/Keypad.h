#ifndef KEYPAD_H
#define KEYPAD_H

#include "Arduino.h"

class Keypad {
public:
  Keypad(int R1=37, int R2=36, int R3=35, int R4=34,
         int C1=33, int C2=32, int C3=31, int C4=30);
  void setHoldThreshold(int thresh);

  bool isPressed(void) const;
  bool isHeld(void) const;
  
  char getKey(void) const;


private:
  short toggle_keys(bool pressedKeys [][4]) const;
  char find_single_key(bool pressedKeys [][4]) const;
  
  
  const int _row[ 4 ];
  const int _col[ 4 ];

  int _hold_threshold = 500; // default, in ms
};

#endif // !KEYPAD_H
