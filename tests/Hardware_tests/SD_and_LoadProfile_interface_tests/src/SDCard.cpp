#include "../include/SDCard.h"

SDCard::SDCard(const uint8_t& ChipSelectPin) {
  _is_connected = SD.begin(ChipSelectPin);
}

SDCard::~SDCard() { SD.end(); }

bool SDCard::connected() const { return _is_connected; }

bool SDCard::fileExists(const String& filename) const {
  return SD.exists(filename);
};

LoadProfile SDCard::openFile(const String& filename) {
  return SD.open(filename);
};
