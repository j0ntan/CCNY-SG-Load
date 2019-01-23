#include "../include/LoadProfile.h"
#include <string.h>
#include "../include/InputSequence.h"

namespace {
bool lineIsComment(char* buffer) {
  return strlen(buffer) >= 2 && buffer[0] == '/' && buffer[1] == '/';
}

void addNumbersToSequence(char*& num_begins, char*& num_ends,
                          InputSequence& input) {
  num_ends = strchr(num_begins, ' ');
  if (num_ends) {
    while (num_begins != num_ends) input.addInput(*(num_begins++));
    num_begins = ++num_ends;
  } else {
    input.addInput('?');
    num_ends = num_begins;
  }
}
}  // namespace

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

void LoadProfile::readLine(char* buffer, InputSequence& sequence,
                           unsigned long& duration, uint8_t buf_size) {
  do {
    fillBuffer(buffer, buf_size);
  } while (!lineIsComment(buffer));

  char* num_begins = buffer;
  char* num_ends = buffer;

  // phase A
  sequence.addInput('A');
  addNumbersToSequence(num_begins, num_ends, sequence);

  // phase B
  sequence.addInput('B');
  addNumbersToSequence(num_begins, num_ends, sequence);

  // phase C
  sequence.addInput('C');
  addNumbersToSequence(num_begins, num_ends, sequence);

  buffer = strrchr(buffer, ' ');
  duration = strtoul(++buffer, nullptr, 0);
}
