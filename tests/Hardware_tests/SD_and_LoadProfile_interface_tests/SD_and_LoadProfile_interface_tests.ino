// This sketch tests the SD card & load profile interface with an SD card module
// connected by the SPI interface.
// Prior to running these tests, the SD card should contain 3 text files that
// can be found in the 'utils' folder.

#include "include/LoadProfile.h"
#include "include/SDCard.h"

static const uint8_t SD_CHIP_SELECT = 53;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Begin tests.\n"));

  detectSDCardNotConnected();
  detectSDCardConnected();
  indicateFileNotFound();
  checkEmptyFileContents();
  readAndPrintThreeLines();
  readLFLineEndings();

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

void detectSDCardNotConnected() {
  Serial.println(F("Test: detectSDCardNotConnected"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Make sure that the SD card or module IS NOT"));
  Serial.println(F("  CONNECTED."));
  waitForEnter();

  SDCard sd{SD_CHIP_SELECT};
  printTestResult(!sd.connected());
}

void detectSDCardConnected() {
  Serial.println(F("Test: detectSDCardConnected"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Verify that the SD card IS CONNECTED."));
  waitForEnter();

  SDCard sd{SD_CHIP_SELECT};
  printTestResult(sd.connected());
}

void indicateFileNotFound() {
  Serial.println(F("Test: indicateFileNotFound"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Verify that the SD card IS CONNECTED."));
  Serial.println(F("- This test will look for the file \"Missing.txt\","));
  Serial.println(F("  which should not be on the card."));
  waitForEnter();

  SDCard sd{SD_CHIP_SELECT};
  const String filename{F("Missing.txt")};
  printTestResult(sd.connected() && !sd.fileExists(filename));
}

void checkEmptyFileContents() {
  Serial.println(F("Test: checkEmptyFileContents"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Verify that the SD card IS CONNECTED."));
  Serial.println(F("- This test will open the file \"empty.txt\" and"));
  Serial.println(F("  check if it is empty."));
  waitForEnter();

  SDCard sd{SD_CHIP_SELECT};
  const String filename{F("empty.txt")};
  if (sd.connected() && sd.fileExists(filename)) {
    LoadProfile profile = sd.openFile(filename);
    printTestResult(!profile.lineAvailable());
  } else {
    Serial.println(F("- SD card or file not found."));
    printTestResult(false);
  }
}

void readAndPrintThreeLines() {
  Serial.println(F("Test: readAndPrintThreeLines"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Verify that the SD card IS CONNECTED."));
  Serial.println(F("- This test will open the file \"3lines.txt\","));
  Serial.println(F("  then read & print its contents."));
  Serial.println(F("- The number of lines read is recorded & compared"));
  Serial.println(F("  to our expectation, 3 in this case."));
  waitForEnter();

  SDCard sd{SD_CHIP_SELECT};
  const String filename{F("3lines.txt")};
  if (sd.connected() && sd.fileExists(filename)) {
    LoadProfile profile = sd.openFile(filename);

    static const uint8_t EXPECTED_COUNT = 3;
    uint8_t count = 0;
    while (profile.lineAvailable()) {
      Serial.println(profile.readLine());
      ++count;
    }

    printTestResult(count == EXPECTED_COUNT);
  } else {
    Serial.println(F("- SD card or file not found."));
    printTestResult(false);
  }
}

void readLFLineEndings() {
  Serial.println(F("Test: readLFLineEndings"));
  Serial.println(F("=================================================="));
  Serial.println(F("- Verify that the SD card IS CONNECTED."));
  Serial.println(F("- This test will open the file \"endLF.txt\","));
  Serial.println(F("  then read & print its contents."));
  Serial.println(F("- This test checks that profiles written with \\LF"));
  Serial.println(F("  line endings can be read correctly."));
  waitForEnter();

  SDCard sd{SD_CHIP_SELECT};
  const String filename{F("endLF.txt")};
  if (sd.connected() && sd.fileExists(filename)) {
    LoadProfile profile = sd.openFile(filename);

    while (profile.lineAvailable()) Serial.println(profile.readLine());

    Serial.println(F("- Verify that the file was read & printed"));
    Serial.println(F("  correctly."));
  } else {
    Serial.println(F("- SD card or file not found."));
    printTestResult(false);
  }
}
