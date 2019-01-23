#include "../include/HardwareTextFile.h"

HardwareTextFile::HardwareTextFile(SDLib::File file) : file(file) {}

HardwareTextFile::~HardwareTextFile() { file.close(); }

size_t HardwareTextFile::readBytesUntil(char terminator, char *buffer,
                                        size_t length) {
  return file.readBytesUntil(terminator, buffer, length);
}

int HardwareTextFile::available() { return file.available(); }
