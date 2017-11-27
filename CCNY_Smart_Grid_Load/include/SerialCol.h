#ifndef SERIALCOL_H
#define SERIALCOL_H

#include "Arduino.h"
#include "Collector.h"

class SerialCol : public Collector {
public:
    SerialCol(const String &input);
};

#endif // !SERIALCOL_H
