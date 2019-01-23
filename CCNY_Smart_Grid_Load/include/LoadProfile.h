#ifndef LOADPROFILE_H
#define LOADPROFILE_H

#include <stdint.h>
#include <SD.h>

class InputSequence;

class LoadProfile {
 public:
  LoadProfile(File file);
  ~LoadProfile();

  bool lineAvailable();
  void readLine(char* buffer, InputSequence& sequence, unsigned long& duration,
                uint8_t buf_size);

 private:
  File file;
};

#endif  // LOADPROFILE_H
