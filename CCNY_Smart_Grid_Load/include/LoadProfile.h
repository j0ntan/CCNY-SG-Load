#ifndef LOADPROFILE_H
#define LOADPROFILE_H

#include <stdint.h>
#include <SD.h>

class LoadProfile : public File {
 public:
  LoadProfile(File file);
  ~LoadProfile();

  bool lineAvailable();
  void fillBuffer(char* buffer, uint8_t buffer_size);
};

#endif  // LOADPROFILE_H
