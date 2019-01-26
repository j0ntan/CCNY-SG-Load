#include "../include/HardwareSDFlash.h"
#include "../include/HardwareTextFile.h"
#include <SD.h>  // includes an "extern SDClass SD"

HardwareSDFlash::HardwareSDFlash(uint8_t chipSelectPin)
    : chip_select(chipSelectPin) {}

HardwareSDFlash::~HardwareSDFlash() { SD.end(); }

bool HardwareSDFlash::connected() const { return SD.begin(chip_select); }

bool HardwareSDFlash::fileExists(const char* filename) const {
  return SD.exists(filename);
}

TextFile* HardwareSDFlash::openFile(const char* filename) const {
  return new HardwareTextFile{SD.open(filename)};
}
