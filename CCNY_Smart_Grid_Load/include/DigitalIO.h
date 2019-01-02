#ifndef DIGITALIO_H
#define DIGITALIO_H

class DigitalInput {
 public:
  virtual ~DigitalInput() = default;

  virtual bool read() const = 0;
};

class DigitalOutput {
 public:
  virtual ~DigitalOutput() = default;

  virtual void set() const = 0;
  virtual void clear() const = 0;
};

#endif  // DIGITALIO_H
