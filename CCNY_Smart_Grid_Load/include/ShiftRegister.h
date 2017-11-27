#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#include "Arduino.h"

class ShiftRegister {
public:
    ShiftRegister(const uint8_t serData=2, const uint8_t shClk=3, 
        const uint8_t stoClk=4, const uint8_t shClr=5);

    void begin(void);

    void feedData(const byte *data, short numBytes);
    void outputData( void );
    void outputData(const byte *data, short numBytes);
    void clrShiftRegs(void);

    void setShPeriod(const unsigned long period);
    void setStPeriod(const unsigned long period);


private:
    void _setPinModes(void);
    void _ticShClk(void); 
    void _ticStClk(void);

    // shift register pins
    const uint8_t _serData;
    const uint8_t _shClk;
    const uint8_t _stoClk;
    const uint8_t _shClr;

    // clk periods in milliseconds
    unsigned long _shDelay = 1;
    unsigned long _stDelay = 1;
};

#endif // !SHIFT_REGISTER_H
