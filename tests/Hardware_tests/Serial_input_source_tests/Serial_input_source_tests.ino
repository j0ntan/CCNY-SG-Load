/*
This sketch tests the ability to detect an incoming serial transmission from an
external serial data source using an installed XBee device. This test sketch is
needed in order to verify that the timing in the functions (using the XBee
interface) will correctly read a serial transmission. This timing is difficult
to determine without the Arduino, hence the need for a test running on an
Arduino device.

NOTES:
  1. Since the XBee acts as a tranparent serial link, it is OK to run this test
  sketch using the built-in Serial Monitor or some other serial connection in
  place of the XBee.
  2. Make sure to only send alphanumeric characters through the serial link,
  i.e. avoid sending newline or carriage-return (\n, \r\n) line endings. Doing
  so results in undefined behavior. */

#include "include/ArduinoInterface.h"
#include "include/HardwareArduino.h"
#include "include/Keypad.h"
#include "include/XBee.h"
#include "include/HardwareXBee.h"
#include "include/Monitor.h"
#include "include/Collect.h"

Arduino *arduino = new HardwareArduino;
XBee *xbee = new HardwareXBee{Serial};

void setup() {
  Serial.begin(9600);
  Serial.println(F("Serial port is ready.\n"));
}

void loop() {
  if (xbee->hasBufferedData()) {
    if (receivedPCSerialData()) {
      String serialInput = collectPCSerialData<String>();

      Serial.println(F("Detected reception of PC serial data."));
      Serial.print(F("Recieved input is \""));
      Serial.print(serialInput);
      Serial.println(F("\""));
      Serial.println(F("Verify that serial transmission is correct."));
      Serial.println(F("Debug if otherwise.\n"));
    } else if (receivedDSPACEManualData()) {
      Serial.println(F("Detected reception of manual mode dSPACE input.\n"));
      emptyBuffer();
    } else {
      Serial.println(F("Clearing the buffer..."));
      emptyBuffer();
      Serial.println(F("DONE.\n"));
    }
  }
}

void emptyBuffer() {
  while (Serial.available()) {
    Serial.read();
    delay(10);  // wait for any remaining incoming bytes
  }
}
