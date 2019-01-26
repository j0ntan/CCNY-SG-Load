#ifndef HARDWARESDFLASH_H
#define HARDWARESDFLASH_H

#include <stdint.h>
#include "SDFlash.h"

class HardwareSDFlash : public SDFlash {
 public:
  HardwareSDFlash(uint8_t chipSelectPin);
  ~HardwareSDFlash() final;

  bool connected() const final;
  bool fileExists(const char* filename) const final;
  TextFile* openFile(const char* filename) const final;

 private:
  uint8_t chip_select;
};

#endif  // HARDWARESDFLASH_H
