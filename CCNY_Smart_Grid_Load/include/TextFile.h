#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <stddef.h>

class TextFile {
 public:
  virtual ~TextFile() = default;

  virtual size_t readBytesUntil(char terminator, char *buffer,
                                size_t length) = 0;
  virtual int available() = 0;
};

#endif  // TEXTFILE_H
