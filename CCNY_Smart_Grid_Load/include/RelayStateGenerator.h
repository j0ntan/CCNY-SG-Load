#ifndef RELAYSTATEGENERATOR
#define RELAYSTATEGENERATOR

class InputSequence;
class RelayState;

bool isValidSequence(const InputSequence& input);
RelayState generateRelayState(const InputSequence& input);

#endif  // RELAYSTATEGENERATOR
