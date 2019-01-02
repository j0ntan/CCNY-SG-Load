#ifndef LOADPROFILE_H
#define LOADPROFILE_H

#include <SD.h>

class LoadProfile : public File {
 public:
  LoadProfile(File file);
  ~LoadProfile();

  bool lineAvailable();
  String readLine();
};

#endif  // LOADPROFILE_H
