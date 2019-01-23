#ifndef HARDWARETEXTFILE_H
#define HARDWARETEXTFILE_H

#include "TextFile.h"
#include <SD.h>

class HardwareTextFile : public TextFile {
 public:
  HardwareTextFile(SDLib::File file);
  ~HardwareTextFile() final;

  size_t readBytesUntil(char terminator, char *buffer, size_t length) final;
  int available() final;

 private:
  SDLib::File file;
};

#endif  // HARDWARETEXTFILE_H
