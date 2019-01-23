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
  void readLine(InputSequence& sequence, unsigned long& duration);

 private:
  static const uint8_t SIZE = 101;
  char buffer[LoadProfile::SIZE] = {};

  File file;
};

#endif  // LOADPROFILE_H
