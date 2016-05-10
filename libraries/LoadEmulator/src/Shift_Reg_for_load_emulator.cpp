#include "Arduino.h"
#include "Shift_Reg_for_load_emulator.h"

Shift_Registers::
Shift_Registers() {
  /*
  Assign default values to private class variables for the shift
  register pins. */
  _serial_data = SERIAL_DATA;
  _output_clk = OUTPUT_CLK;
  _shift_reg_clk = SHIFT_REG_CLK;
  _shift_reg_clr = SHIFT_REG_CLR;
  
  _output_size = 48;
}

Shift_Registers::
Shift_Registers(int IC_count) {
  /*
  Function is not used. Will probably be removed. */
  _serial_data = SERIAL_DATA;
  _output_clk = OUTPUT_CLK;
  _shift_reg_clk = SHIFT_REG_CLK;
  _shift_reg_clr = SHIFT_REG_CLR;
  
  _output_size = 8 * IC_count;
}

void Shift_Registers::
set_Arduino_pins(int s_d, int o_clk, int SR_clk, int SR_clr) {
  /*
  This function is used to set the pins on the Arduino that control the
  shift registers. This is for non-default values. */
  _serial_data = s_d;
  _output_clk = o_clk;
  _shift_reg_clk = SR_clk;
  _shift_reg_clr = SR_clr;
}

void Shift_Registers::
initialize(bool *serial_data_array) {
  /*
  This function initializes the shift registers by setting the shift
  register clear pin to HIGH state and sets other shift register pins
  to default LOW state. Also initializes data_stream_states to False 
  value, becuase relays should be OFF by default at initial power up.*/
  
  // Set Arduino pins for shift register as outputs.
  pinMode(_shift_reg_clk, OUTPUT);
  pinMode(_output_clk,    OUTPUT);
  pinMode(_serial_data,   OUTPUT);
  pinMode(_shift_reg_clr, OUTPUT);
  
  // Set initial states of shift register pins.
  digitalWrite(_shift_reg_clk, LOW);
  digitalWrite(_output_clk,    LOW);
  digitalWrite(_serial_data,   LOW || ACTIVE_LOW);
  digitalWrite(_shift_reg_clr, HIGH);
  
  // Set relays to be in OFF state at startup.
  for (int i = 0; i < 48; i++) {
    digitalWrite(_shift_reg_clk, HIGH);
    delay(SHIFT_REG_CLK_DELAY);
    digitalWrite(_shift_reg_clk,  LOW);
    delay(SHIFT_REG_CLK_DELAY);
  }
  digitalWrite(_output_clk, HIGH);
  delay(OUTPUT_CLK_DELAY);
  digitalWrite(_output_clk,  LOW);
  
  _serial_data_array = serial_data_array;
  _serial_data_is_bytes = false;
  
  Serial.println(F("Shift Reg: Shift Registers Initialized."));
}

void Shift_Registers::
initialize(byte *serial_data_array) {
  /*
  This function initializes the shift registers by setting the shift
  register clear pin to HIGH state and sets other shift register pins
  to default LOW state. Also initializes data_stream_states to False 
  value, becuase relays should be OFF by default at initial power up.*/
  
  // Set Arduino pins for shift register as outputs.
  pinMode(_shift_reg_clk, OUTPUT);
  pinMode(_output_clk,    OUTPUT);
  pinMode(_serial_data,   OUTPUT);
  pinMode(_shift_reg_clr, OUTPUT);
  
  // Set initial states of ouptut pins.
  digitalWrite(_shift_reg_clk, LOW);
  digitalWrite(_output_clk,    LOW);
  digitalWrite(_serial_data,   LOW || ACTIVE_LOW);
  digitalWrite(_shift_reg_clr, HIGH);
  
  for (int i = 0; i < 48; i++) {
    digitalWrite(_shift_reg_clk, HIGH);
    delay(SHIFT_REG_CLK_DELAY);
    digitalWrite(_shift_reg_clk,  LOW);
    delay(SHIFT_REG_CLK_DELAY);
  }
  digitalWrite(_output_clk, HIGH);
  delay(OUTPUT_CLK_DELAY);
  digitalWrite(_output_clk,  LOW);
  
  _serial_byte_array = serial_data_array;
  _serial_data_is_bytes = true;
  
  Serial.println(F("Shift Reg: Shift Registers Initialized.")); 
}

void Shift_Registers::
send_serial_data() {
  /*
  This function sends the data stream, which corresponds to the states
  of the relays (True or False for ON or OFF, respectively), to the 
  shift registers. Then it signals the shift registers to activate this
  data stream to its output pins so that the relays can recieve their 
  respective states (ON or OFF).*/
  
  if (_serial_data_is_bytes == true) {
    _serial_byte_array += ((_output_size/8) - 1);
    byte relay_state_bit;
    for (int i = 6 - 1; i > -1; i--) {
      for (int j = 0; j < 8; j++) {
        relay_state_bit = (*_serial_byte_array & (B00000001 << j)) >> j;
        digitalWrite(_serial_data, relay_state_bit ^ ACTIVE_LOW);
      
        digitalWrite(_shift_reg_clk, HIGH);
        delay(SHIFT_REG_CLK_DELAY);
        digitalWrite(_shift_reg_clk,  LOW);
        delay(SHIFT_REG_CLK_DELAY);
      }
      _serial_byte_array--;
    }
      
  }
  else {
    for (int i = _output_size - 1; i > -1; i--) {      
      digitalWrite(_serial_data, *(_serial_data_array + i) ^ ACTIVE_LOW);
      
      digitalWrite(_shift_reg_clk, HIGH);
      delay(SHIFT_REG_CLK_DELAY);
      digitalWrite(_shift_reg_clk,  LOW);
      delay(SHIFT_REG_CLK_DELAY);
    }
  }
  
  // keep pin low during idle state
  digitalWrite(_serial_data,    LOW);
  
  Serial.println(F("Shift Reg: Finished sending."));
}

void Shift_Registers::
trigger_output() {
  // send data to shift registers and activate output
  digitalWrite(_output_clk, HIGH);
  delay(OUTPUT_CLK_DELAY);
  digitalWrite(_output_clk,  LOW);
  Serial.println(F("Shift Reg: Outputs are activated."));
}
