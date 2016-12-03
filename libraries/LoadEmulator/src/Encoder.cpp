#include "Encoder.h"

void Encoder::encode(const int *switchingValues) {
    /*
    Takes an array of integer values that correspond to the relay states
    and encode them as bytes. */

    // switchingValues = -1 indicate keep previous value 
    if(switchingValues[0] > -1) _AC_A = switchingValues[0];
    if(switchingValues[1] > -1) _AC_B = switchingValues[1];
    if(switchingValues[2] > -1) _AC_C = switchingValues[2];
    if(switchingValues[3] > -1) _DC   = switchingValues[3];

    setPhase(_encodedValues[0], _encodedValues[1], _AC_A); // set _AC_A
    setPhase(_encodedValues[2], _encodedValues[3], _AC_B); // set _AC_B
    setPhase(_encodedValues[4], _encodedValues[5], _AC_C); // set _AC_C
}

const byte *Encoder::getACEncodedValues(void) const {
    return _encodedValues;
}
const int &Encoder::getDCValue(void) const {
    return _DC;
}

const String Encoder::getIdleState(void) const {
    String idle_str;
    idle_str.remove(0);

    idle_str.concat( F("Idle state:         ") );
    idle_str.concat( F("AC(3-ph) =          ") );

    idle_str.concat( F("A:") );
    idle_str.concat(_AC_A);

    // double digit, single space; single digit, double space
    if(_AC_A > 9) idle_str.concat( F(" B:") );
    else idle_str.concat( F("  B:") );
    idle_str.concat(_AC_B);
    if(_AC_B > 9) idle_str.concat( F(" C:") );
    else idle_str.concat( F("  C:") );
    idle_str.concat(_AC_C);
    idle_str.concat( F("      ") );
    if(_AC_C < 10) idle_str.concat( F(" ") );

    idle_str.concat( F("DC:") );
    idle_str.concat(_DC);
    idle_str.concat( F("                ") );

    return idle_str; 
}


void Encoder::setPhase(byte &MSB, byte &LSB, const int &phVal) const {
    // Sets the bit values for the two bytes for a single phase.

    MSB = LSB = B00000000;
    for(short i = 0; i < phVal && i < 8; ++i)   MSB |= B10000000 >> i;
    for(short i = 0; i < phVal-8 && i < 8; ++i) LSB |= B10000000 >> i;
}
