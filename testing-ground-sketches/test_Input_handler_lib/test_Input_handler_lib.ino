#include "Input_handler_for_load_emulator.h"
#include "Keypad_for_load_emulator.h"



Input_handler input_handler;
Keypad keypad;


void setup() {
  Serial.begin(9600);

  //keypad.set_pins(4, 5, 6, 7, 10, 11, 12, 13);  // for Arduino Uno connection
  keypad.set_pins(37, 36, 35, 34, 33, 32, 31, 30);  // for Arduino Mega connection
  keypad.initialize();
}

void loop() {
  if (keypad.check_any_press()) {
    while ( input_handler.ENTER_or_CANCEL_pressed == false )
      if (keypad.check_any_press())
        input_handler.accept_keypress2( keypad.find_pressed_key() );
    input_handler.ENTER_or_CANCEL_pressed = false;
    //lcd.show_message( input_handler.LCD_status );
    //parser.receive_input( input_handler.send_input_to_parser() );
  }
  if (Serial.available() > 0) {
    input_handler.accept_serial_port_input();
  }
}
