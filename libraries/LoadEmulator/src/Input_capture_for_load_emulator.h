#ifndef Input_capture_load_emulator_h
#define Input_capture_load_emulator_h

#include "Arduino.h"

#define SIZE 12 // max characters that will be stored/handled

enum Phase {A, B, C, DC};

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
  bool XBeeGotData(void);
  void captureXBee(void);

  // dSPACE functions
  bool dSPACEavailable(void);
  void captureRXdSPACE(void);
  void captureManBal();
  void setProfile(int step);
  //dSPACE variables
  bool dSPACErxActive;

  // shared functions
  void linkXBee(HardwareSerial *serial);
  bool isValid();
  // shared variables
  String captureStatus;
  String inputString;


private:
  // Keypad functions
  void reset_keypad_vars();

  // XBee functions
  void send_confirmation();

  //dSPACE functions
  void reset_dSPACE_vars(void);
  int read_dSPACE_int(void);
  void cancel_dSPACE(void);
  void erase_last_dSPACE(void);
  void reset_dSPACE(void);
  void enter_dSPACE(void);
  void request_dSPACE(Phase);
  //dSPACE variables
  int _previous_int, _current_int;
  const String _profile[18];

  // shared functions
  void reset_shared_vars();
  void check_numbers_range(int num);
  // shared variables
  bool _Reset_flag;
  int _counter;
  bool _numbers_outof_range;
  HardwareSerial *_HardSerial;
};
#endif
