#include "ShiftRegister.h"


ShiftRegister::ShiftRegister( const uint8_t serData, const uint8_t shClk,
        const uint8_t stoClk, const uint8_t shClr) :
        _serData(serData), _shClk(shClk), _stoClk(stoClk),
        _shClr(shClr) {
    _setPinModes();
}

void ShiftRegister::begin(void) {
    // Reset the shift register pins on initialization

    byte init[6] = {0};
    outputData(init, 6);
}

void ShiftRegister::feedData(const byte *data, short numBytes) {
    /*
    Send bytes to the shift registers to hold in memory. Requires call
    to outputData() to activate the pins from values in memory. Bytes
    are sent in reverse order. */

    // traverse bytes in reverse
    for(int i = numBytes; i > 0; --i) {
        byte tempByte = *(data + i - 1);
        byte output_Bit;
        
        // traverse bits in reverse
        for ( int i = 0; i < 8; i++ ) {
            // select bit 'i' with logical AND, then shift to rightmost
            output_Bit = ( tempByte & ( B00000001 << i ) ) >> i;
            
            // invert output_Bit because relays are active LOW
            digitalWrite( _serData, !output_Bit );
            _ticShClk();
        }
    }
}
void ShiftRegister::outputData( void ) {
    // Pulse the StClk to bring memory values onto the output pins
    _ticStClk();
}
void ShiftRegister::outputData(const byte *data, short numBytes) {
    // Single call for convenience
    feedData(data, numBytes);
    outputData();
}
void ShiftRegister::clrShiftRegs( void ) {
    /*
    Not used at the moment, but might provide better performance. For a
    reset, the current setup simply passes 6 zero value bytes, which
    gets stored into memory, bit-by-bit. It's probably faster, although
    only slightly, to use the _shClr pin to reset the output. */
}

void ShiftRegister::setShPeriod( const unsigned long period ) {
    // Set _shClk period in ms, rounded up to next even number
    _shDelay = ( period % 2 == 0) ? period / 2 : ( period + 1 ) / 2;
}
void ShiftRegister::setStPeriod( const unsigned long period ) {
    // Set _stClk period in ms, rounded up to next even number
    _stDelay = ( period % 2 == 0) ? period / 2 : ( period + 1 ) / 2;
}


void ShiftRegister::_setPinModes(void) {
    pinMode( _serData, OUTPUT );
    pinMode( _shClk,   OUTPUT );
    pinMode( _stoClk,  OUTPUT );
    pinMode( _shClr,   OUTPUT );

    // set pins to idle state
    digitalWrite(_serData, LOW);
    digitalWrite(_shClk,   LOW);
    digitalWrite(_stoClk,  LOW);
    digitalWrite(_shClr,   HIGH);
}
void ShiftRegister::_ticShClk( void ) {
    digitalWrite( _shClk, HIGH );
    delay( _shDelay );
    digitalWrite( _shClk, LOW );
    delay( _shDelay );
}
void ShiftRegister::_ticStClk( void ) {
    digitalWrite( _stoClk, HIGH );
    delay( _stDelay );
    digitalWrite( _stoClk, LOW );
    delay( _stDelay );
}
