#ifndef DSPACECOL_H
#define DSPACECOL_H

#include "Collector.h"

class dspaceCol : public Collector {
public:
    dspaceCol(HardwareSerial &xbeeSer);

    bool profileModeRequested() const;


protected:
    void read_ds_int(void);

    HardwareSerial &_xbee;
    int _ds_int = -1;
};

#endif // !DSPACECOL_H
