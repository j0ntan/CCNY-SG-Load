#include "../include/LoadProfile.h"
#include <string.h>

LoadProfile::LoadProfile(File file) : File(file) {}

LoadProfile::~LoadProfile() { File::close(); }

bool LoadProfile::lineAvailable() { return File::available() > 0; }

void LoadProfile::fillBuffer(char* buffer, uint8_t buf_size) {
  // clear buffer
  memset(buffer, 0, buf_size);

  // read into buffer
  File::readBytesUntil('\n', buffer, buf_size - 1);

  // remove any carriage-return ('\r')
  const unsigned int LAST_CHAR_INDEX = strlen(buffer) - 1;
  if (buffer[LAST_CHAR_INDEX] == '\r') buffer[LAST_CHAR_INDEX] = '\0';
}
