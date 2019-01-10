// Author: Jonathan Gamboa
// version: ?.?
// Institution: Smart Grid Lab, City College of New York (ST 643)

#include "include/CCNY_Smart_Grid_Load_files.h"

//
// SG-Load Arduino I/O pin mapping:
//
// Shift register outputs
const DigitalOutput* serial_data_output = new HardwareOutput{2};
const DigitalOutput* SR_clock_output = new HardwareOutput{3};
const DigitalOutput* ST_clock_output = new HardwareOutput{4};
// DC relay outputs
const DigitalOutput* relay1_output = new HardwareOutput{22};
const DigitalOutput* relay2_output = new HardwareOutput{23};

RelayState relay_state;
Timer* timer = new HardwareTimer;
Keypad* keypad = new HardwareKeypad(37, 36, 35, 34, 33, 32, 31, 30);
ShiftRegister shiftregister{serial_data_output, SR_clock_output,
                            ST_clock_output};
XBee* xbee = new HardwareXBee{Serial};

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
}

void loop() {
  String user_input;
  if (keypadButtonWasPressed()) {
    user_input = recordKeypadSequence<String>();
    processInputString(user_input);
  } else if (xbee->hasBufferedData()) {
    if (receivedPCSerialData()) {
      user_input = collectPCSerialData<String>();
      processInputString(user_input);
    } else if (receivedDSPACEManualData()) {
      user_input = collectDSPACEManualData<String>();
      processInputString(user_input);
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
      outputAllRelays(bits, relay_state.DC, relay1_output, relay2_output);
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
        timer->delay(DURATION);
      }
    }
  }  // else, report SD card or File error
}
