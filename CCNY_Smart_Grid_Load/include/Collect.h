#ifndef COLLECT_H
#define COLLECT_H

class InputSequence;

//**************************** KEYPAD collection *****************************//
InputSequence recordKeypadSequence();

//*************************** PC Serial collection ***************************//
InputSequence collectPCSerialData();

void emptyTheBuffer();

//************************* dSPACE manual collection *************************//
InputSequence collectDSPACEManualData();

//************************* dSPACE Load Profile Mode *************************//
unsigned int readProfileNumberFromSerial();

void createFilename(const unsigned int& profile_number, char* buffer);

#endif  // COLLECT_H
