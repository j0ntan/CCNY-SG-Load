#ifndef SR_LOAD_EMULATOR_h
#define SR_LOAD_EMULATOR_h

/*
Refer to the documentation for details on cascading of the shift 
registers (74HC595) and for the connection of the shift registers to
Arduino and the relays. The currently installed lab setup uses the
Arduino pins 4-7 for the Shift Register Clear, Shift Register Clock,
Output Clock, and Data input pins of the shift register, 
respectively. */
// Default Arduino outputs for the shift register pins:
#define SERIAL_DATA     7   // or SERIAL_INPUT
#define OUTPUT_CLK      6   // or STORAGE_REG_CLK
#define SHIFT_REG_CLK   5
#define SHIFT_REG_CLR   4   // or MASTER_RESET

// Assign the delay values, in milliseconds, for both clocks
#define SHIFT_REG_CLK_DELAY 10
#define OUTPUT_CLK_DELAY 10

// Set to 0 or 1 to Turn OFF or ON active low outputs, respectively
#define ACTIVE_LOW 1

#include "Arduino.h"



class Shift_Registers
{
public: 
  Shift_Registers();
  Shift_Registers(int IC_count);
  void set_Arduino_pins(int serial_data_pin,   // for non-default values
                        int output_clk_pin,
                        int shift_reg_clk_pin,
                        int shift_reg_clr_pin);  
  void initialize(bool *serial_data_array);
  void initialize(byte *serial_data_array);
  void send_serial_data(void);
  void trigger_output(void);

private:
  int _output_size;                   // default is 48
  bool * _serial_data_array;
  byte * _serial_byte_array;
  bool _serial_data_is_bytes;
  
  //-------------Pins:-------------//
  int _serial_data, _shift_reg_clk;   // Only used when default pins
  int _output_clk, _shift_reg_clr;    // are changed with set_...();
  
};

#endif
