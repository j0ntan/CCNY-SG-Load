#ifndef SDFLASH_H
#define SDFLASH_H

class TextFile;

class SDFlash {
 public:
  virtual ~SDFlash() = default;

  virtual bool connected() const = 0;
  virtual bool fileExists(const char* filename) const = 0;
  virtual TextFile* openFile(const char* filename) const;
};

#endif  // SDFLASH_H
