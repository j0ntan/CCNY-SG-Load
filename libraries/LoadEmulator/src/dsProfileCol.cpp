#include "dsProfileCol.h"

dsProfileCol::dsProfileCol(HardwareSerial &xbeeSer, SDprofile &sd) :
        dspaceCol(xbeeSer), _sd(sd) {
    // Initialize with Serial port connected to XBee & connected SD card
}

bool dsProfileCol::openFile(void) {
    /*
    Attempts to open a file on the SD card after receiving a value from
    dSPACE. Return true if file could be open. Returns false if SD is
    not connected, or specific file is not found, or for some reason
    could not be opened. */

    if(_sd) { // check card connected
        read_ds_int();
        if(_sd.openFile(_ds_int - 45)) { // check file can be opened
            update_file_opened_msg();
            return true;
        }
        else {
            update_cant_open_msg();
            return false;
        }
    }
    else {
        read_ds_int(); // remove value from serial buffer
        update_not_conn_msg();
        return false;
    }
}
bool dsProfileCol::readLine(void) {
    /*
    Read the load profile values line by line, and update the input
    sequence and display message. */

    if(_sd.isOpen()) {
        _sd.readLine();
        _val = _sd.getValues();
        _delay = _val.time;

        // after reading, check if more data available
        if(!_sd.isOpen()) {
             _sd.closeFile();
             _collectingStatus = false;
        }

        if(_val.aVal!=-1 && _val.bVal!=-1 && _val.cVal!=-1 &&
                _val.time!=0) { // -1 indicates value not read or missing
            _input.remove(0);
            _input.concat( F("A") );
            _input.concat(_val.aVal);
            _input.concat( F("B") );
            _input.concat(_val.bVal);
            _input.concat( F("C") );
            _input.concat(_val.cVal);
        }
        return true;
    }
    else {
        update_cant_open_msg();
        return false;
    }
}

const unsigned long dsProfileCol::getDelay(void) {
    return _delay;
}


void dsProfileCol::update_file_opened_msg(void) {
    _collectorMsg.concat( F("                    ") );
    _collectorMsg.concat( F("    File opened     ") );
    _collectorMsg.concat( F("    successfully.   ") );
    _collectorMsg.concat( F("                    ") );

    _speed = Display::Speed::NORMAL;
}
void dsProfileCol::update_cant_open_msg(void) {
    _collectorMsg.concat( F("                    ") );
    _collectorMsg.concat( F("   Failed to open   ") );
    _collectorMsg.concat( F("  the chosen file.  ") );
    _collectorMsg.concat( F("                    ") );

    _speed = Display::Speed::SLOW;
}
void dsProfileCol::update_not_conn_msg(void) {
    _collectorMsg.concat( F(" SD card connection ") );
    _collectorMsg.concat( F("   not detected.    ") );
    _collectorMsg.concat( F(" Check connections  ") );
    _collectorMsg.concat( F("    and reboot.     ") );

    _speed = Display::Speed::SLOW;
}
