#include "include/XBee.h"
#include "include/HardwareXBee.h"

XBee *xbee = new HardwareXBee{Serial};

void setup() {
  Serial.begin(9600);
  Serial.println(F("Begin tests.\n"));

  detectDataReceived();
  countBytesReceived();
  readAndCompareText();
  peekAndCompareText();

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

void printTestResult(bool result) {
  if (result)
    Serial.println(F("PASSED"));
  else
    Serial.println(F("FAILED"));

  waitForEnter();
}

void emptyBuffer() {
  while (Serial.available()) {
    Serial.read();
    delay(10);  // wait for any remaining incoming bytes
  }
}

bool receivedSomethingWithinTenSeconds() {
  const unsigned long TEN_SECONDS = 10000;
  const unsigned long TIME_INTERVAL = 100;
  unsigned long time_elapsed = 0;
  bool received_serial_data = false;

  while (!received_serial_data && time_elapsed <= TEN_SECONDS) {
    delay(TIME_INTERVAL);
    time_elapsed += TIME_INTERVAL;
    received_serial_data = xbee->hasBufferedData();
  }

  return received_serial_data;
}

bool receivedTwoBytes() {
  /*
  NOTE: The Serial Monitor is capable of attaching different line endings to the
  user input. It can have no line ending, a carriage return & newline ending
  ('\r\n' Windows-style), or just a newline ending ('\n' Linux-style). These
  should not add to the byte count. */

  if (receivedSomethingWithinTenSeconds()) {
    delay(50);  // allow time to fill buffer
    int interface_byte_count = xbee->bytesAvailable();

    while (Serial.available()) {
      char serial_char = Serial.read();
      if (serial_char == '\r' || serial_char == '\n') interface_byte_count--;
    }

    return interface_byte_count == 2;
  } else
    return false;
}

bool ableToReadArduinoWord() {
  if (receivedSomethingWithinTenSeconds()) {
    delay(100);  // wait for buffer fill
    const String EXPECTED{"arduino"};
    String received;

    while (Serial.available()) {
      char letter = static_cast<char>(xbee->readByte());
      if (letter != '\r' && letter != '\n') received += letter;
    }

    return EXPECTED == received;
  } else
    return false;
}

bool ableToPeekCodeWord() {
  if (receivedSomethingWithinTenSeconds()) {
    delay(100);
    const String EXPECTED{"code"};
    String received;

    while (Serial.available())
      if (xbee->peekByte() != '\r' && xbee->peekByte() != '\n')
        received += Serial.read();
      else
        Serial.read();  // discard the byte

    return EXPECTED == received;
  } else
    return false;
}

void detectDataReceived() {
  Serial.println(F("Test: detectDataReceived"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Testing the \"hasBufferedData()\" function."));
  Serial.println(F("- The Serial buffer will expect to receive a bit"));
  Serial.println(F("  of text through the Serial Monitor within a 10"));
  Serial.println(F("  second time period."));
  waitForEnter();

  Serial.println(F("- Enter some text now."));
  printTestResult(receivedSomethingWithinTenSeconds());
  emptyBuffer();
}

void countBytesReceived() {
  Serial.println(F("Test: countBytesReceived"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Testing the \"bytesAvailable()\" function."));
  Serial.println(F("- The Serial buffer will expect to receive 2 bytes"));
  Serial.println(F("  or any 2 characters, through the Serial Monitor"));
  Serial.println(F("  within a 10 second interval. You can send the"));
  Serial.println(F("  word \"OK\" or any two letters."));
  waitForEnter();

  Serial.println(F("- Enter 2 characters now."));
  printTestResult(receivedTwoBytes());
  emptyBuffer();
}

void readAndCompareText() {
  Serial.println(F("Test: readAndCompareText"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Testing the \"readByte()\" function."));
  Serial.println(F("- The Serial buffer will expect to receive the"));
  Serial.println(F("  word \"arduino\" within a 10 second time period."));
  waitForEnter();

  Serial.println(F("- Enter the  word \"arduino\" now."));
  printTestResult(ableToReadArduinoWord());
  emptyBuffer();
}

void peekAndCompareText() {
  Serial.println(F("Test: peekAndCompareText"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Testing the \"peekByte()\" function."));
  Serial.println(F("- The Serial buffer will expect to receive the"));
  Serial.println(F("  word \"code\" within a 10 second time period."));
  waitForEnter();

  Serial.println(F("- Enter the  word \"code\" now."));
  printTestResult(ableToPeekCodeWord());
  emptyBuffer();
}
