#ifndef COLLECTOR_H
#define COLLECTOR_H

#include "Arduino.h"
#include "Display.h"

class Collector { // base class
public:
    bool isCollecting(void) const;

    const String getInput(void) const;
    const String getColMsg(void) const;
    const Display::Speed getSpeed(void) const;


protected:
    // TODO: consider using a single static String for all msgs with
    // space reserved for 20x4 characters
    String _input;
    String _collectorMsg; // reserved for error messages
    Display::Speed _speed = Display::Speed::STATIC;

    bool _collectingStatus = true;
};

#endif // !COLLECTOR_H
