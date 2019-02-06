#ifndef COLLECT_H
#define COLLECT_H

class InputSequence;

//**************************** KEYPAD collection *****************************//
InputSequence collectKeypadSequence();

//*************************** PC Serial collection ***************************//
InputSequence collectPCSequence();

//************************* dSPACE manual collection *************************//
InputSequence collectDSPACESequence();

//************************* dSPACE Load Profile Mode *************************//
unsigned int readRequestedProfileNumber();

void createFilename(const unsigned int& profile_number, char* buffer);

#endif  // COLLECT_H
