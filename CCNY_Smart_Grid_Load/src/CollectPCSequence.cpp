#include "../include/InputSequence.h"
#include "../include/Timer.h"
#include "../include/XBee.h"

// Globals defined in main application
extern XBee* xbee;
extern Timer* timer;

InputSequence collectPCSequence() {
  InputSequence PC_sequence;
  while (xbee->hasBufferedData())
    PC_sequence.addInput(
        static_cast<char>(xbee->readByte()));  // read data into sequence
  return PC_sequence;
}

void emptyTheBuffer() {
  while (xbee->hasBufferedData()) {
    xbee->readByte();
    timer->delay(5);  // guard time, allow incoming data to fill buffer
  }
}
