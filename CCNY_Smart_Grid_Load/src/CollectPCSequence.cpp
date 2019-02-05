#include "../include/InputSequence.h"
#include "../include/Timer.h"
#include "../include/XBee.h"

// Globals defined in main application
extern XBee* xbee;
extern Timer* timer;

InputSequence collectPCSerialData() {
  InputSequence PC_input;
  while (xbee->hasBufferedData())
    PC_input.addInput(
        static_cast<char>(xbee->readByte()));  // read data into input
  return PC_input;
}

void emptyTheBuffer() {
  while (xbee->hasBufferedData()) {
    xbee->readByte();
    timer->delay(5);  // guard time, allow incoming data to fill buffer
  }
}
