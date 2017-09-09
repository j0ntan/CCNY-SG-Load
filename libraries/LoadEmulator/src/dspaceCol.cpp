#include "dspaceCol.h"

dspaceCol::dspaceCol(HardwareSerial &xbeeSer) :
        _xbee(xbeeSer) {
    // Initialize with Serial port connected to XBee
}

bool dspaceCol::profileModeRequested() const {
    // profile mode commands begin at 46
    return _xbee.peek() > 45;
}


void dspaceCol::read_ds_int(void) {
    /*
    Reads the XBee Serial port for an integer sent by dSPACE. This
    function is needed because dSPACE spams the serial buffer, instead
    of sending a single byte, each time that it sends a value. Thus, we
    need to look for unique bytes and ignore copies that are sent. */

    int temp_int = _ds_int;

    // read and store unique value from serial port
    while(temp_int == _ds_int)
        _ds_int = _xbee.read();

    // remove copies of _ds_int from buffer
    while(_xbee.peek() == _ds_int) {
        _xbee.read();
        delay(5); // guard time, allow incoming data to fill buffer 
    }
}
