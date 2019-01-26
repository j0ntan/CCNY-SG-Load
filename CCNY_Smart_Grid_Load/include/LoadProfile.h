#ifndef LOADPROFILE_H
#define LOADPROFILE_H

#include <stdint.h>

class TextFile;
class InputSequence;

class LoadProfile {
 public:
  LoadProfile(TextFile* file);
  ~LoadProfile();

  bool lineAvailable();
  void readLine(InputSequence& sequence, unsigned long& duration);

 private:
  static const uint8_t SIZE = 101;
  char buffer[LoadProfile::SIZE] = {};

  TextFile* file;
};

#endif  // LOADPROFILE_H
