#ifndef Input_capture_load_emulator_h
#define Input_capture_load_emulator_h

#include "Arduino.h"

#define SIZE 12 // max characters that will be stored/handled

class InputCapture
{
public:
  InputCapture();

  // Keypad functions
  void setKeypadActive(void);
  void captureKeypress(bool heldStatus, char keypress);
  // Keypad variables
  bool stillTakingKeypresses;

  void captureSerialMonitor(void);
  void captureXBee(String xb_in);
  void captureDSpace(void);

  // shared variables
  String captureStatus;
  String inputString;


private:
  // Keypad functions
  void reset_keypad_vars();
};
#endif
