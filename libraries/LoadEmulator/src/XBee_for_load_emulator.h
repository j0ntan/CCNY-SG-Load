#ifndef XBee_for_load_emulator_h
#define XBee_for_load_emulator_h

#include "Arduino.h"

class LoadEmuXBee {
  public:
    LoadEmuXBee(HardwareSerial& serial);
    bool received_data(void);
    void grab_data();

    String LCD_msg;
    String input_sequence;

  private:
    void send_confirmation();

    HardwareSerial& _HardSerial;
};

#endif
