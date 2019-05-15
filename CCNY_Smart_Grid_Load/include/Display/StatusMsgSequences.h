#ifndef DISPLAY_STATUSMSGSEQUENCES_H
#define DISPLAY_STATUSMSGSEQUENCES_H

class BernoulliRNG;
class RelayState;

namespace Display {
void initializationSequence(const BernoulliRNG& rng);
void updateIdleState(const RelayState& state);
}  // namespace Display

#endif  // DISPLAY_STATUSMSGSEQUENCES_H
