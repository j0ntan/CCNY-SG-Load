#include "SDprofile.h"

bool SDprofile::checkConnected(void) {
    // Check if the SD card is connected on start-up.

    // pin 53 is for SPI CS
    _sdConnected = SD.begin(53);
    return _sdConnected;
}
bool SDprofile::openFile(int num) {
    /*
    Attempt to open the file indicated by 'num'. For example, when num=3
    this function attempts to open a file named "PRFL004.txt". Returns 
    true if file open operation is successful. */

    _filename.remove(0); // clear string
    _filename.concat( F("PRFL") );

    // add leading zeros
    if(num < 10) {
        _filename.concat( F("00") );
    }
    else if(num < 100) {
        _filename.concat( F("0") );
    }

    // add file number and file extension
    _filename.concat(num);
    _filename.concat( F(".txt") );

    _openedFile = SD.open(_filename);
    return _openedFile;
}
void SDprofile::closeFile(void) {
    _openedFile.close();
}
bool SDprofile::isOpen(void) {
    // check if file is open and ready to read
    return _openedFile && _openedFile.available();
}
void SDprofile::readLine(void) {
    /*
    Read a line from the opened profile. Comments are skipped. After 
    reading the line, get the values for the 3-phase AC relays. */

    String line_text;

    // store line into a String
    while(_openedFile.available() && _openedFile.peek() != '\n') {
        line_text.concat(char(_openedFile.read()));
    }
    _openedFile.read(); // discard line terminator, '\n'

    bool found_comment = line_text.length() >= 2 &&
            line_text[0] == '/' && line_text[1]=='/';
    if(found_comment) {
        line_text.remove(0);
        readLine(); // read next line
    }
    else if(line_text.length()) {
        if(!parse_line(line_text)) {
            // assigments indicate fail reading of file values
            _phaseVals.aVal = -1;
            _phaseVals.bVal = -1;
            _phaseVals.cVal = -1;
            _phaseVals.time = 0;
        }
    }
}
const SDprofile::Values SDprofile::getValues(void) {
    return _phaseVals;
}

SDprofile::operator bool() {
    return _sdConnected;
}


bool SDprofile::parse_line(const String &line) {
    /*
    Iterate through each character in 'line' and look for the integer 
    values for each relay, as well as the time duration. Return true if
    all these values were found. */

    bool found_a = false;
    bool found_b = false;
    bool found_c = false;
    bool found_time = false;

    short idx = 0; // index
    short size = line.length();

    while(idx < size) {
        if(!found_a && isDigit(line[idx])) {
            found_a = true;
            _phaseVals.aVal = get_num(idx, line);
        }
        else if(!found_b && isDigit(line[idx])) {
            found_b = true;
            _phaseVals.bVal = get_num(idx, line);
        }
        else if(!found_c && isDigit(line[idx])) {
            found_c = true;
            _phaseVals.cVal = get_num(idx, line);
        }
        else if(!found_time && isDigit(line[idx])) {
            found_time = true;
            _phaseVals.time = get_num(idx, line);
        }
        ++idx;
    }

    return found_a && found_b && found_c && found_time;
}
int SDprofile::get_num(short &idx, const String &line) {
    /*
    Look for an integer value. Intergers should be separated by some 
    whitespace, which is ignored, and a comma, which denotes the end of 
    search because we expect commas to follow an integer (i.e. 1, 2). */

    short size = line.length();
    int value = 0;

    while(idx < size && line[idx]!=',') {
        if(isDigit(line[idx])) { // ignore whitespace
            value = 10*value + (int(line[idx] - 48));
            // TODO: check if the following line works in place of the 
            // line above: value = 10*value + (line[idx]-'0');
        }
        ++idx;
    }
    ++idx; // move index to next char after ','

    return value;
}
