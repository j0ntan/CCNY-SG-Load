// Author: Jonathan Gamboa
// version: ?.?
// Institution: Smart Grid Lab, City College of New York (ST 643)

#include "include/HardwareTimer.h"
#include "include/HardwareIO.h"
#include "include/HardwareKeypad.h"
#include "include/HardwareXBee.h"
#include "include/HardwareSDFlash.h"
#include "include/HardwareRNG.h"
#include "include/Display/HardwareLCD.h"
#include "include/InputSequence.h"
#include "include/LoadProfile.h"
#include "include/Monitor.h"
#include "include/Collect.h"
#include "include/RelayState.h"
#include "include/RelayStateGenerator.h"
#include "include/ShiftRegister.h"
#include "include/Output.h"
#include "include/Display/MessagesList.h"
#include "include/Display/StatusMsgSequences.h"

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
SDFlash* sd = new HardwareSDFlash{53};
BernoulliRNG* rng = new HardwareRNG(0.2);
Display::LCD* lcd = new Display::HardwareLCD(0x27);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  Display::initializationSequence(*rng);
}

void loop() {
  InputSequence user_input;
  if (keypadButtonWasPressed()) {
    user_input = collectKeypadSequence();
    processInputSequence(user_input);
  } else if (xbee->hasBufferedData()) {
    if (receivedPCSerialData()) {
      user_input = collectPCSequence();
      processInputSequence(user_input);
    } else if (receivedDSPACEManualData()) {
      user_input = collectDSPACESequence();
      processInputSequence(user_input);
    } else if (receivedDSPACELoadProfile()) {
      activateLoadProfile();
    } else
      xbee->clearBuffer();
  }
}

void processInputSequence(const InputSequence& input) {
  if (isValidSequence(input)) {
    recordNewRelayState(input, relay_state);
    outputNewRelayState(relay_state);
    lcd->printMsg(Display::load_idle_state);
    Display::updateIdleState(relay_state);
  }
  // else, display error message
}

void activateLoadProfile() {
  unsigned int number = readRequestedProfileNumber();
  char filename[12] = {"PRFL000.txt"};
  createFilename(number, filename);

  if (sd->connected() && sd->fileExists(filename)) {
    LoadProfile profile{sd->openFile(filename)};
    while (profile.lineAvailable()) {
      InputSequence profile_input;
      unsigned long duration = 0;
      profile.readLine(profile_input, duration);
      processInputSequence(profile_input);
      timer->delay(duration);
    }
  } else
    lcd->printMsg(Display::SD_file_open_error);
}
