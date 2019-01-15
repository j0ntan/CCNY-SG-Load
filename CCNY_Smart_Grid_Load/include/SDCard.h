#ifndef SDCARD_H
#define SDCARD_H

#include "LoadProfile.h"
#include <WString.h>
#include <SD.h>

class SDCard {
 public:
  SDCard(const uint8_t& ChipSelectPin);
  ~SDCard();

  SDCard(const SDCard&) = delete;
  SDCard& operator=(const SDCard& rhs) = delete;

  bool connected() const;
  bool fileExists(const String& filename) const;
  LoadProfile openFile(const String& filename);

 private:
  bool _is_connected = false;
};

#endif  // SDCARD_H
