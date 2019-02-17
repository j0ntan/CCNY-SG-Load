#ifndef RELAYSTATEGENERATOR
#define RELAYSTATEGENERATOR

class InputSequence;
class RelayState;

bool isValidSequence(const InputSequence& input);
void recordNewRelayState(const InputSequence& input, RelayState& relayState);

#endif  // RELAYSTATEGENERATOR
