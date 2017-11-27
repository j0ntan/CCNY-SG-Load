#include <Encoder.h>

/*
This example shows how the Encoder object takes an set of integer
values and encodes them into a pair of bytes, whose bit values
correspond to the ON/OFF states of the shift registers.

Note that the Encoder object always contains the idle state of the AC and
DC values. In future updates, this will be decoupled. */

// function prototypes
void printEncodedValues();
void randomizeValues(int *);
void printUserValues(int *);

Encoder encoder;

void setup() {
    Serial.begin(19200);

    const int SIZE = 4;
    int values[SIZE] = {0};     // initialize all to 0


    // We expect an initial OFF state for all relays.
    Serial.println(F("Initial values of encoder:"));
    printEncodedValues();

    // simulate random user input
    randomizeValues(values);
    encoder.encode(values);
    Serial.println(F("Encoding of randomized values: "));
    printUserValues(values);
    printEncodedValues();

    // simulate user input of a balanced load
    values[0] = values[1] = values[2] = random(0,17);
    encoder.encode(values);
    Serial.println(F("Encoding of balanced load:"));
    printUserValues(values);
    printEncodedValues();

    // simulate partial state change
    partialValueChange(values);
    encoder.encode(values);
    Serial.println(F("Encoding a partial change:"));
    printUserValues(values);
    printEncodedValues();

    Serial.println("End of example.");
}

void loop() {}

void printEncodedValues() {
    const byte *encodedBytes = encoder.getACEncodedValues();

    // print AC values first
    Serial.println(F("        A                 B                 C        "));

    // print each individual bit for each byte
    for (short i=0; i<6; ++i) {
        for (short j=0; j<8; ++j) {
            byte selectorBit = B10000000 >> j;
            byte singleBit = (encodedBytes[i] & selectorBit) >> (8-j-1);
            if(singleBit) Serial.print(1);
            else Serial.print(0);
        }
        Serial.print(' ');
    }
    Serial.println();

    // lastly, print DC value
    Serial.print("DC value: ");
    Serial.println(encoder.getDCValue());
    Serial.println();
}
void randomizeValues(int * vals) {
    randomSeed(analogRead(0));

    for(short i=0; i<3; ++i)
        vals[i] = random(0,17);
    vals[3] = random(0,3);
}
void printUserValues(int *vals) {
    for(short i=0; i<4; ++i)
        Serial.print(vals[i]), Serial.print(' ');
    Serial.println();
}
void partialValueChange(int *vals) {
    bool someValueUnchanged = false;
    while(!someValueUnchanged) {
        // randomly decide which values to change
        for(short i=0; i<4; ++i)
            if(random(0,2) % 2) {
                vals[i] = -1;
                someValueUnchanged = true;
            }
            else {
                if(i<3) vals[i] = random(0,17);
                else vals[i] = random(0,3);
            }
    }
}
