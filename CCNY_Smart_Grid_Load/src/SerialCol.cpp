#include "../include/SerialCol.h"

SerialCol::SerialCol(const String &serInput) {
    /*
    Assigns the received serial input data to the collector input
    sequence. A serial input longer than 11 chars is truncated. Then
    updates the message to be displayed. */
    
    _input.remove(0);

    if(serInput.length() > 11) {
        _input.concat(serInput.substring(0, 11));
        _collectorMsg.concat( F("Input truncated.    ") );
    }
    else {
        _input.concat(serInput);
        _collectorMsg.concat( F("                    ") );        
    }

    _collectorMsg.concat( F("Received input:     ") );

    _collectorMsg.concat(_input);
    short counter = 20 - _input.length();
    while(counter-- > 0) _collectorMsg.concat( F(" ") );

    _collectorMsg.concat( F("                    ") );

    _speed = Display::Speed::NORMAL;
}
