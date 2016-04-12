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

#include <Input_handler_for_load_emulator.h>
#include <Keypad_for_load_emulator.h>
#include <LCD_for_load_emulator.h>
#include <Parser_for_load_emulator.h>
#include <Shift_Reg_for_load_emulator.h>
#include <XBee_for_load_emulator.h>


Keypad keypad;
Input_handler input_handler;
Parser parser;
Shift_Registers shift_reg;
Liquid_Crystal_Display lcd;
LoadEmuXBee xb = LoadEmuXBee(Serial1);


void setup() {
  Serial.begin(19200);

  //keypad.setPins(4,5,6,7,10,11,12,13); // for Arduino Uno connection
  keypad.setPins(37, 36, 35, 34, 33, 32, 31, 30);  // for Arduino Mega connection
  keypad.begin();

  shift_reg.set_Arduino_pins(7, 6, 5, 4);
  shift_reg.initialize(parser.shift_reg_data);
  //shift_reg.initialize(parser.shift_reg_bytes);
  
  lcd.initialize();
  lcd.show_message(parser.load_idle_status, "faster");

  // pins for DC relays:
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  
}

void loop() {
  if (keypad.wasPressed()) {
    while ( input_handler.ENTER_or_CANCEL_pressed == false ) {
      if (keypad.wasPressed()) {
        input_handler.accept_keypress( keypad.getKey() );
        if (input_handler.show_error_msg == true) {
          lcd.show_message( input_handler.LCD_status, "fast" );
          input_handler.show_error_msg = false; // reset after showing msg once
          //lcd.show_message( input_handler.input_sequence, "faster" );
        }
        else if ( input_handler.ENTER_or_CANCEL_pressed == false ) {
          lcd.show_message( input_handler.input_sequence, "faster" );
        }
      }
    }
    //Serial.print("My sequence is: "); // for debugging
    Serial.println(input_handler.input_sequence);
    // Reset this value for the next input sequence.
    input_handler.ENTER_or_CANCEL_pressed = false;

    //input_handler.input_sequence = input_handler.input_sequence
    parser.parse(input_handler.input_sequence);
    shift_reg.send_serial_data();
    shift_reg.trigger_output();
    Activate_DC(parser.DC_value);

    lcd.show_message( input_handler.LCD_status, "faster" );
    lcd.show_message( parser.load_idle_status, "faster" );
  }
  else if (Serial.available() > 0) {
    input_handler.accept_serial_port_input();
    
    parser.parse(input_handler.input_sequence);
    shift_reg.send_serial_data();
    shift_reg.trigger_output();

    lcd.show_message( input_handler.LCD_status );
    lcd.show_message( parser.load_idle_status );
  }
  else if (xb.received_data()) {
    xb.grab_data();
    input_handler.accept_XBee_input(xb.input_sequence);

    parser.parse(input_handler.input_sequence);
    shift_reg.send_serial_data();
    shift_reg.trigger_output();

    lcd.show_message( input_handler.LCD_status );
    lcd.show_message( parser.load_idle_status );
  }
}

void Activate_DC(int DC_value) {
  switch (DC_value) {
    case 0:
      digitalWrite(22, LOW);
      digitalWrite(23, LOW);
      break;
    case 1:
      digitalWrite(22, HIGH);
      digitalWrite(23, LOW);
      break;
    case 2:
      digitalWrite(22, HIGH);
      digitalWrite(23, HIGH);
      break;
    default:
      digitalWrite(22, LOW);
      digitalWrite(23, LOW);
      break;
  }
}
