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

  // Serial monitor functions
  void captureSerialMonitor(void);

  // XBee functions
  void linkXBee(HardwareSerial *serial);
  bool XBeeGotData(void);
  void captureXBee(void);

  // DSpace functions
  void captureDSpace(void);

  // shared functions
  bool isValid();
  // shared variables
  String captureStatus;
  String inputString;


private:
  // Keypad functions
  void reset_keypad_vars();

  // XBee functions
  void send_confirmation();
  // XBee variables
  HardwareSerial *_HardSerial;

  // shared functions
  void reset_shared_vars();
  void check_numbers_range(int num);
  // shared variables
  bool _Reset_flag;
  int _counter;
  bool _numbers_outof_range;
};
#endif
