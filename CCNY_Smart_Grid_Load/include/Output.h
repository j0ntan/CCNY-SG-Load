#ifndef OUTPUT_H
#define OUTPUT_H

#include "DigitalIO.h"
#include "Encode.h"

void outputAllRelays(const ACRelayBits& ACvalues, const uint8_t& DCvalue,
                     const DigitalOutput* DC_relay1_output,
                     const DigitalOutput* DC_relay2_output) {}

class RelayState;

void outputNewRelayState(const RelayState& newRelayState);

#endif  // OUTPUT_H
