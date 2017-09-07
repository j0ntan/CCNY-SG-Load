/*
Author: Jonathan Gamboa
Date: 3/16/2016
Institution: Smart Grid Lab, City College of New York

New control system for the load bank uses Shift Registers instead 
of latches + MuxShield in order to reduce complexity and to avoid 
issues and malfunctions with the latch system.

This version of the code will be optimized and will differ very much 
from previous versions. Modularity is the main focus in building this
code. This control system can be thought of as containing modules for:
the keypad, the LCD, the shift registers, an input handler, and a parser
to interface user input with the hardware. */

#include "Keypad_for_load_emulator.h"
#include "Input_handler_for_load_emulator.h"
#include "Parser_for_load_emulator.h"
#include "Shift_Reg_for_load_emulator.h"
#include "LCD_for_load_emulator.h"


Keypad keypad;
Input_handler input_handler;
Parser parser;
Shift_Registers shift_reg;
Liquid_Crystal_Display LCD;


void setup() {
  Serial.begin(9600);
  
  //keypad.set_pins(4,5,6,7,10,11,12,13); // For Arduino Uno
  keypad.initialize();
  //shift_reg.set_Arduino_pins(9, 8, 3, 2);
  shift_reg.initialize(parser.shift_reg_data);
  LCD.initialize();
}

void loop() {
  if (keypad.check_any_press()) {
    while( input_handler.ENTER_or_CANCEL_pressed == false )
        if (keypad.check_any_press()) {
          input_handler.accept_keypress( keypad.find_pressed_key() );
          //Serial.print("LCD status: "); Serial.println(input_handler.LCD_status); Serial.println("LCD status: Finished."); Serial.println();
          //Serial.println("LCD status: "); my_lcd.show_message( input_handler.LCD_status );
          //Serial.println("LCD status: "); Serial.println(input_handler.LCD_status); Serial.println();
        }
    // Reset this value for the next input sequence.
    input_handler.ENTER_or_CANCEL_pressed = false;
    
    //lcd.show_message( input_handler.LCD_status );
    parser.parse(input_handler.input_sequence);
    shift_reg.send_serial_data();
    shift_reg.trigger_output();
    //String mystr = input_handler.input_sequence; Serial.print("mystr is: "); Serial.println(mystr); parser.receive_input(mystr);
  }
  if (Serial.available() > 0) {
      input_handler.accept_serial_port_input();
      Serial.print("Serial: Input sequence is "); Serial.println(input_handler.input_sequence);
      parser.parse(input_handler.input_sequence);
      shift_reg.send_serial_data();
      shift_reg.trigger_output();
  }
}
