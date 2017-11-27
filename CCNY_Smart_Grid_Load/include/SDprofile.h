#ifndef SDPROFILE_H
#define SDPROFILE_H

#include "Arduino.h"
#include "SPI.h"
#include "SD.h"

class SDprofile {
public:
    struct Values {
        int aVal;
        int bVal;
        int cVal;
        unsigned long time;
    };

    bool checkConnected(void);
    bool openFile(int num);
    void closeFile(void);
    bool isOpen(void);
    void readLine(void);

    const Values getValues(void);

    operator bool();

private:
    bool parse_line(const String &line);
    int get_num(short &index, const String &line);

    bool _sdConnected = false;
    File _openedFile;
    
    String _filename;

    Values _phaseVals;
};

#endif // !SDPROFILE_H
