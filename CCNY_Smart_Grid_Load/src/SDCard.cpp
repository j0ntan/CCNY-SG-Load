#include "../include/SDCard.h"
#include "../include/HardwareTextFile.h"

SDCard::SDCard(const uint8_t& ChipSelectPin) {
  _is_connected = SD.begin(ChipSelectPin);
}

SDCard::~SDCard() { SD.end(); }

bool SDCard::connected() const { return _is_connected; }

bool SDCard::fileExists(const String& filename) const {
  return SD.exists(filename);
};

TextFile* SDCard::openFile(const String& filename) {
  return new HardwareTextFile{SD.open(filename)};
};
