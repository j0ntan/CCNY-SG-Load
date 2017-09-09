#ifndef DSPROFILECOL_H
#define DSPROFILECOL_H

#include "dspaceCol.h"
#include "SDprofile.h"

class dsProfileCol : public dspaceCol{
public:
    dsProfileCol(HardwareSerial &xbeeSer, SDprofile &sd);

    bool openFile(void);
    bool readLine(void);

    const unsigned long getDelay(void);


private:
    void update_file_opened_msg(void);
    void update_cant_open_msg(void);
    void update_not_conn_msg(void);

    SDprofile &_sd;
    SDprofile::Values _val;


    unsigned long _delay;
};

#endif // !DSPROFILECOL_H
