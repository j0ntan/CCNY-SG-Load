#ifndef ENCODER_H
#define ENCODER_H

#include "Arduino.h"

class Encoder {
public:
    void encode(const int *switchingValues);

    const byte *getACEncodedValues(void) const;
    const int &getDCValue(void) const;
    
    const String getIdleState(void) const;


private:
    void setPhase(byte &MSB, byte &LSB, const int &phVal) const;

    byte _encodedValues[6] = {0};
    int _AC_A = 0;
    int _AC_B = 0;
    int _AC_C = 0;
    int _DC = 0;
};

#endif // !ENCODER_H
