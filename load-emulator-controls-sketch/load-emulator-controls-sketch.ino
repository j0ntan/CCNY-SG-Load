/*
  Author: Jonathan Gamboa
  Date: 5/3/2016
  Institution: Smart Grid Lab, City College of New York

  New control system for the load bank uses Shift Registers instead 
  of latches + MuxShield in order to reduce complexity and to avoid 
  issues and malfunctions with the latch system.

  This version of the code will be optimized and will differ very much
  from previous versions. Modularity is the main focus in building this
  code. This control system can be thought of as containing modules for:
  the keypad, the LCD, the shift registers, an input handler, and a parser
  to interface user input with the hardware. */

#include <Input_capture_for_load_emulator.h>
#include <Keypad_for_load_emulator.h>
#include <LCD_for_load_emulator.h>
#include <Parser_for_load_emulator.h>
#include <shift_reg_for_load_emulator.h>


Keypad keypad;
InputCapture myInput;
Parser parser;
Shift_Registers shiftReg;
Liquid_Crystal_Display lcd;

void setup() {
  Serial.begin(19200);

  //keypad.begin(4,5,6,7,10,11,12,13); // for Arduino Uno connection
  keypad.begin();

  myInput.linkXBee(&Serial3);

  shiftReg.set_Arduino_pins(7, 6, 5, 4);
  shiftReg.initialize(parser.shift_reg_data);
  //shiftReg.initialize(parser.shiftReg_bytes);
  
  lcd.begin();
  lcd.showMessage(parser.load_idle_status, "static");

  // pins for DC relays:
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  
}

void loop() {
  if (keypad.isPressed()) {
    myInput.setKeypadActive();
    while ( myInput.stillTakingKeypresses ) {
      if (keypad.isPressed()) {
        myInput.captureKeypress(keypad.heldStatus, keypad.getKey());
        lcd.showMessage(myInput.captureStatus, "static");
        while (keypad.isPressed()) {
          // do nothing until key is depressed
        }
      }
    }
    lcd.showMessage(myInput.captureStatus, "faster");

    if (myInput.inputString.length() == 0) {  // no change to the load
      lcd.showMessage(parser.load_idle_status, "static");
    }
    else if (myInput.isValid()) { // the input is accepted and the relays are updated

      parser.parse(myInput.inputString);
      lcd.showMessage(parser.load_idle_status, "static");

      shiftReg.send_serial_data();
      shiftReg.trigger_output();
      Activate_DC(parser.DC_value);
    }
    else {  // some error occurred with the input, display error message
      lcd.showMessage(myInput.captureStatus, "slow");
      lcd.showMessage(parser.load_idle_status, "static");
    }
  } // end: if (keypad.isPressed())

  else if (Serial.available() > 0) {
    myInput.captureSerialMonitor();

    if (myInput.isValid()) {
      lcd.showMessage(myInput.captureStatus, "faster");

      parser.parse(myInput.inputString);
      lcd.showMessage(parser.load_idle_status, "static");

      shiftReg.send_serial_data();
      shiftReg.trigger_output();
      Activate_DC(parser.DC_value);
    }
    else {
      lcd.showMessage(myInput.captureStatus, "slow");
      lcd.showMessage(parser.load_idle_status, "static");
    }
  }

  else if (myInput.XBeeGotData()&&0) {
    myInput.captureXBee();

    if (myInput.isValid()) {
      lcd.showMessage(myInput.captureStatus, "fast");

      parser.parse(myInput.inputString);
      lcd.showMessage(parser.load_idle_status, "static");

      shiftReg.send_serial_data();
      shiftReg.trigger_output();
      Activate_DC(parser.DC_value);
    }
    else {
      lcd.showMessage(myInput.captureStatus, "slow");
      lcd.showMessage(parser.load_idle_status, "static");
    }
  }

  else if (myInput.dSPACEavailable() && Serial3.peek() < 29 ) { // manual mode
    myInput.dSPACErxActive = true;

    myInput.captureRXdSPACE();

    if (myInput.isValid()) {
      parser.parse(myInput.inputString);
      lcd.showMessage(parser.load_idle_status, "static");

      shiftReg.send_serial_data();
      shiftReg.trigger_output();
      Activate_DC(parser.DC_value);
    }
    else {
      lcd.showMessage(myInput.captureStatus, "slow");
      lcd.showMessage(parser.load_idle_status, "static");
    }
  }

  else if (myInput.dSPACEavailable() && Serial3.peek() < 45 ) { // manual bal mode
    myInput.captureManBal();

    parser.parse(myInput.inputString);
    lcd.showMessage(parser.load_idle_status, "static");

    shiftReg.send_serial_data();
    shiftReg.trigger_output();
    Activate_DC(parser.DC_value);
  }

  else if (myInput.dSPACEavailable()) { // load profile mode
    int i = 0;
    while(myInput.dSPACEavailable()) {
      Serial3.read();
    }
    while( i < 18 ) {
      myInput.setProfile( i++ );
      parser.parse(myInput.inputString);
      lcd.showMessage(parser.load_idle_status, "static");

      shiftReg.send_serial_data();
      shiftReg.trigger_output();
      Activate_DC(parser.DC_value);
      
      delay(1500);
    }
  }
} // end of loop()

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
