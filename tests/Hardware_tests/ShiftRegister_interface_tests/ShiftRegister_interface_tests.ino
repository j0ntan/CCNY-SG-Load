// This sketch tests the ShiftRegister interface with a 74HC595 device connected
// to an Arduino.

#include "include/ShiftRegister.h"

ShiftRegister shiftregister{2, 3, 4};

void setup() {
  Serial.begin(9600);
  Serial.println(F("Begin tests.\n"));

  verifyInitialization();
  shiftByteIntoRegisters();
  moveRegisterBitsToOutput();
  moveInputBitsToOutput();

  Serial.println(F("End of tests."));
}

void loop() {}

void waitForEnter() {
  Serial.println(F("Press ENTER to continue...\n"));
  bool enterPressed = false;
  do {
    while (Serial.available())
      if (Serial.read() == '\n') enterPressed = true;
  } while (!enterPressed);
}

void verifyInitialization() {
  Serial.println(F("Test: verifyInitialization"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Check the state of the shift register outputs"));
  Serial.println(F("  (with an LED or voltmeter). Verify that the"));
  Serial.println(F("  outputs are all at 5 V."));
  waitForEnter();
}

void shiftByteIntoRegisters() {
  const byte PIN_OUTPUT_STATE =
      0x07;  // corresponds to only bits A, B, & C turned ON

  Serial.println(F("Test: shiftByteIntoRegisters"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Testing the side effects of the \"ShiftByte()\""));
  Serial.println(F("  function."));
  Serial.print(F("- The byte with a value of "));
  Serial.print(PIN_OUTPUT_STATE, HEX);
  Serial.println(F(" will be shifted"));
  Serial.println(F("  into the register."));
  waitForEnter();

  shiftregister.shiftByte(PIN_OUTPUT_STATE);
  Serial.println(F("- Byte shifted. Verify that the outputs have not"));
  Serial.println(F("  changed, i.e. they should all still be at 5 V."));
  waitForEnter();
}

void moveRegisterBitsToOutput() {
  Serial.println(F("Test: moveRegisterBitsToOutput"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Testing the \"updateOutput()\" function."));
  Serial.println(F("- The bits that were shifted into the register"));
  Serial.println(F("  from the previous test will be moved to the"));
  Serial.println(F("  output pins of the IC."));
  waitForEnter();

  shiftregister.updateOutput();
  Serial.println(F("- Verify that the shift register output pins"));
  Serial.println(F("  changed according to the stored bits from the"));
  Serial.println(F("  previous test."));
  waitForEnter();
}

void moveInputBitsToOutput() {
  const byte PIN_OUTPUT_STATE = 0x09;  // corresponds to bits A & D turned ON

  Serial.println(F("Test: moveInputBitsToOutput"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Testing the \"outputByte()\" function."));
  Serial.print(F("- The byte with a value of "));
  Serial.print(PIN_OUTPUT_STATE, HEX);
  Serial.println(F(" will be stored"));
  Serial.println(F("  into the register and output pins."));
  waitForEnter();

  shiftregister.outputByte(PIN_OUTPUT_STATE);
  Serial.println(F("- Verify that the pins corresponding to the output"));
  Serial.println(F("  byte were toggled to 5 V."));
  waitForEnter();
}
