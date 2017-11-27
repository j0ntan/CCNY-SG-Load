#ifndef DSMANUALCOL_H
#define DSMANUALCOL_H

#include "dspaceCol.h"

class dsManualCol : public dspaceCol{
public:
    dsManualCol(HardwareSerial &xbeeSer);

    void collectInt(void);


private:
    void parse_ds_int(void);

    void update_col_msg(void);
    void update_cancel_msg(void);
};

#endif // !DSMANUALCOL_H
