#ifndef LOADPROFILE_H
#define LOADPROFILE_H

#include "WString.h"
#include <SPI.h>
#include <SD.h>

class LoadProfile : public File {
 public:
  LoadProfile(File file);
  ~LoadProfile();

  bool lineAvailable();
  String readLine();
};

#endif  // LOADPROFILE_H
