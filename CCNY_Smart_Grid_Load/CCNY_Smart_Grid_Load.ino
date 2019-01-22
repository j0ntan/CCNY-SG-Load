// Author: Jonathan Gamboa
// version: ?.?
// Institution: Smart Grid Lab, City College of New York (ST 643)

#include "include/HardwareTimer.h"
#include "include/HardwareIO.h"
#include "include/HardwareKeypad.h"
#include "include/HardwareXBee.h"
#include "include/InputSequence.h"
#include "include/SDCard.h"
#include "include/Monitor.h"
#include "include/Collect.h"
#include "include/Scan.h"
#include "include/Parse.h"
#include "include/Encode.h"
#include "include/Output.h"

//
// SG-Load Arduino I/O pin mapping:
//
// Keypad outputs
const DigitalInput* row1 = new HardwareInput{37};
const DigitalInput* row2 = new HardwareInput{36};
const DigitalInput* row3 = new HardwareInput{35};
const DigitalInput* row4 = new HardwareInput{34};
const DigitalOutput* col1 = new HardwareOutput{33};
const DigitalOutput* col2 = new HardwareOutput{32};
const DigitalOutput* col3 = new HardwareOutput{31};
const DigitalOutput* col4 = new HardwareOutput{30};
const DigitalInput* input_pins[] = {row1, row2, row3, row4};
const DigitalOutput* output_pins[] = {col1, col2, col3, col4};
// Shift register outputs
const DigitalOutput* serial_data_output = new HardwareOutput{2};
const DigitalOutput* SR_clock_output = new HardwareOutput{3};
const DigitalOutput* ST_clock_output = new HardwareOutput{4};
// DC relay outputs
const DigitalOutput* relay1_output = new HardwareOutput{22};
const DigitalOutput* relay2_output = new HardwareOutput{23};

RelayState relay_state;
Timer* timer = new HardwareTimer;
Keypad* keypad = new HardwareKeypad{input_pins, output_pins};
ShiftRegister shiftregister{serial_data_output, SR_clock_output,
                            ST_clock_output};
XBee* xbee = new HardwareXBee{Serial};

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
}

void loop() {
  InputSequence user_input;
  if (keypadButtonWasPressed()) {
    user_input = recordKeypadSequence();
    processInputString(user_input);
  } else if (xbee->hasBufferedData()) {
    if (receivedPCSerialData()) {
      user_input = collectPCSerialData();
      processInputString(user_input);
    } else if (receivedDSPACEManualData()) {
      user_input = collectDSPACEManualData();
      processInputString(user_input);
    } else if (receivedDSPACELoadProfile()) {
      activateLoadProfile();
    } else
      emptyTheBuffer();
  }
}

void processInputString(const InputSequence& input) {
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
  SDCard* sd = new SDCard{SD_CS_pin};
  if (sd->connected() && sd->fileExists(filename)) {
    LoadProfile profile = sd->openFile(filename);
    while (profile.lineAvailable()) {
      const String INPUT_STR = profile.readLine();
      if (!lineIsComment(INPUT_STR)) {
        const InputSequence PROFILE_INPUT =
            extractProfileInput<String>(INPUT_STR);
        const unsigned long DURATION =
            extractProfileDuration<String>(INPUT_STR);
        processInputString(PROFILE_INPUT);
        timer->delay(DURATION);
      }
    }
  }  // else, report SD card or File error
  delete sd;
}
