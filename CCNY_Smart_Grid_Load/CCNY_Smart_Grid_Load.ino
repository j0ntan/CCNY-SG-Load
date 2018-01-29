// Author: Jonathan Gamboa
// version: ?.?
// Institution: Smart Grid Lab, City College of New York (ST 643)

#include "include/CCNY_Smart_Grid_Load_files.h"

RelayState relay_state;
Arduino* arduino = new HardwareArduino;
Keypad* keypad = new HardwareKeypad(37, 36, 35, 34, 33, 32, 31, 30);
ShiftRegister shiftregister{2, 3, 4};
XBee* xbee = new HardwareXBee{Serial};

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
}

void loop() {
  if (keypadButtonWasPressed()) {
    String input_from_keypad = recordKeypadSequence<String>();
    processInputString(input_from_keypad);
  } else if (xbee->hasBufferedData()) {
    if (receivedPCSerialData()) {
      String input_from_PC_serial = collectPCSerialData<String>();
      processInputString(input_from_PC_serial);
    } else if (receivedDSPACEManualData()) {
      String manual_input_from_dSPACE = collectDSPACEManualData<String>();
      processInputString(manual_input_from_dSPACE);
    } else if (receivedDSPACELoadProfile()) {
      activateLoadProfile();
    } else
      emptyTheBuffer();
  }
}

void processInputString(const String& input) {
  TokenSet tokens = scan(input);
  if (!tokens.containsInvalid()) {
    ParseAnalysis analysis = analyzeTokens(tokens);
    if (hasNoParseErrors(analysis)) {
      parseNewRelayState(analysis, relay_state);
      ACRelayBits bits = encode(relay_state);
      outputAllRelays(bits, relay_state.DC);
    }  // else, parse error reported
  } else
    ;  // report invalid char error
}

void activateLoadProfile() {
  unsigned int number = readProfileNumberFromSerial();
  String filename = createFilename<String>(number);

  static const uint8_t SD_CS_pin = 53;
  SDCard sd{SD_CS_pin};
  if (sd.connected() && sd.fileExists(filename)) {
    LoadProfile profile = sd.openFile(filename);
    while (profile.lineAvailable()) {
      const String INPUT_STR = profile.readLine();
      if (!lineIsComment(INPUT_STR)) {
        const String PROFILE_INPUT = extractProfileInput<String>(INPUT_STR);
        const unsigned long DURATION =
            extractProfileDuration<String>(INPUT_STR);
        processInputString(PROFILE_INPUT);
        arduino->delay(DURATION);
      }
    }
  }  // else, report SD card or File error
}
