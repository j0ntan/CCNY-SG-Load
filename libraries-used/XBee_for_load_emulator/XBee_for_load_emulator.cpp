#include "XBee_for_load_emulator.h"
//#include <XBee.h>

#include "Arduino.h"

//XBee xbee = XBee();

LoadEmuXBee::
LoadEmuXBee(HardwareSerial& serial) :
  _HardSerial(serial) {
  // initialize empty strings
  LCD_msg = "";
  input_sequence = "";
  _HardSerial.begin(9600);
}

bool LoadEmuXBee::
received_data() {
  if (_HardSerial.available() > 0) {
    Serial.println(F("XBee: Incoming data."));
    return true;
  }
}

void LoadEmuXBee::
grab_data() {
  input_sequence = "";
  while(_HardSerial.available() > 1) {
    input_sequence += char(_HardSerial.read());
    delay(1); // needed to allow time for serial read to execute
  }
  _HardSerial.read(); // read & discard the \CRLF byte from buffer

  Serial.print(F("XBee: The received data is \""));
  Serial.print(input_sequence);
  Serial.println(F("\"")); Serial.println();

  LCD_msg  = "                    ";
  LCD_msg += "                    ";
  LCD_msg += "                    ";
  LCD_msg += "                    ";
}
