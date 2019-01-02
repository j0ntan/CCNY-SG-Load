#include "../include/LoadProfile.h"
#include "WString.h"

LoadProfile::LoadProfile(File file) : File(file) {}

LoadProfile::~LoadProfile() { File::close(); }

bool LoadProfile::lineAvailable() { return File::available() > 0; }

String LoadProfile::readLine() {
  String line = File::readStringUntil('\n');
  const unsigned int LAST_CHAR_INDEX = line.length() - 1;
  if (line[LAST_CHAR_INDEX] == '\r') line.remove(LAST_CHAR_INDEX);
  return line;
};
