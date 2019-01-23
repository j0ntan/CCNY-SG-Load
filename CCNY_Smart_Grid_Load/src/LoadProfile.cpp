#include "../include/LoadProfile.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "../include/InputSequence.h"

namespace {
void fillBuffer(char* buffer, uint8_t buf_size, File& file) {
  // clear buffer
  memset(buffer, 0, buf_size);

  // read into buffer
  file.readBytesUntil('\n', buffer, buf_size - 1);

  // remove any carriage-return ('\r')
  const unsigned int LAST_CHAR_INDEX = strlen(buffer) - 1;
  if (buffer[LAST_CHAR_INDEX] == '\r') buffer[LAST_CHAR_INDEX] = '\0';
}

bool lineIsComment(char* buffer) {
  return strlen(buffer) >= 2 && buffer[0] == '/' && buffer[1] == '/';
}

uint8_t countNumSpacePairs(char* buffer, uint8_t buf_length) {
  uint8_t pairs_counted = 0;
  for (uint8_t i = 1; i < buf_length; ++i) {
    if (isblank(buffer[i]) && isdigit(buffer[i - 1])) {
      ++pairs_counted;
    }
  }
  return pairs_counted;
}

bool onlyNumsAndSpaces(char* buffer, uint8_t buf_length) {
  bool valid_char = true;
  for (uint8_t i = 0; i < buf_length && valid_char; ++i)
    valid_char = isdigit(buffer[i]) || isblank(buffer[i]);
  return valid_char;
}

bool validFormat(char* buffer) {
  // minimun requirements is 4 numbers & 3 spaces in-between

  const uint8_t MINIMUM_LENGTH = 7;
  const uint8_t LENGTH = strlen(buffer);
  const bool ENDS_WITH_NUM = isdigit(buffer[LENGTH - 1]);
  return LENGTH >= MINIMUM_LENGTH && onlyNumsAndSpaces(buffer, LENGTH) &&
         (countNumSpacePairs(buffer, LENGTH) == 3 && ENDS_WITH_NUM);
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

void LoadProfile::readLine(InputSequence& sequence, unsigned long& duration) {
  do {
    fillBuffer(buffer, LoadProfile::SIZE, file);
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

  char* const DURATION_PTR = strrchr(buffer, ' ') + 1;
  duration = strtoul(DURATION_PTR, nullptr, 0);
}
