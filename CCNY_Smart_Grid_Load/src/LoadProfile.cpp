#include "../include/LoadProfile.h"
#include <string.h>

LoadProfile::LoadProfile(File file) : file(file) {}

LoadProfile::~LoadProfile() { file.close(); }

bool LoadProfile::lineAvailable() { return file.available() > 0; }

void LoadProfile::fillBuffer(char* buffer, uint8_t buf_size) {
  // clear buffer
  memset(buffer, 0, buf_size);

  // read into buffer
  file.readBytesUntil('\n', buffer, buf_size - 1);

  // remove any carriage-return ('\r')
  const unsigned int LAST_CHAR_INDEX = strlen(buffer) - 1;
  if (buffer[LAST_CHAR_INDEX] == '\r') buffer[LAST_CHAR_INDEX] = '\0';
}
