#ifndef HARDWARERNG_H
#define HARDWARERNG_H

#include "BernoulliRNG.h"

/*
Note: Due to the limitations of an 8-bit micro, we are using a very simple
implementation of a bernoulli distribution. Essentially, the ratio of true
values in the 'vals' array determines the actual 'p' value. For the SG-Load, we
hard-coded a 'p' value of 0.2, thus there are 2 true and 8 false values in the
array. By randomly choosing a value in the array, we get our distribution. */

class HardwareRNG : public BernoulliRNG {
 public:
  HardwareRNG(double p);
  ~HardwareRNG() final = default;

  bool operator()() const final;

 private:
  static const unsigned char SIZE = 10;
  const bool vals[SIZE] = {true,  true,  false, false, false,
                           false, false, false, false, false};
};

#endif  // HARDWARERNG_H
