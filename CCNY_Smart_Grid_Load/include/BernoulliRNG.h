#ifndef BERNOULLIRNG_H
#define BERNOULLIRNG_H

class BernoulliRNG {
 public:
  BernoulliRNG(double p) : p(p) {}
  virtual ~BernoulliRNG() = default;

  virtual bool operator()() const = 0;

 protected:
  double p;
};

#endif  // BERNOULLIRNG_H
