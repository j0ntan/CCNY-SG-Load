#include <ShiftRegister.h>

/*
This example shows how to configure the shift registers by sending
bytes through the Arduino. This can be done in two steps or a single
step, as shown below. */

/*
Refer to source file for explanation on how byte values correspond
to the ON/OFF state of the shift registers. */
const int MAX_REGISTERS = 6;
byte sampleBytes[MAX_REGISTERS] = {B10000000, B00000000,
        B11000000, B00000000,
        B11100000, B00000000};

// use default pins 2-5
ShiftRegister sr;

void setup() {
    // must initialize at start
    sr.begin();

    // store bytes in memory
    feedData(sampleBytes, MAX_REGISTERS);
    // send memory values to output pins
    outputData();

    // do the same in one step
    outputData(sampleBytes, MAX_REGISTERS);

    // not yet implemented
    //clrShiftRegs();
}

void loop() {}
