#include <gmock/gmock.h>
#include <RelayState.h>
#include <Encode.h>
#include <utility>
#include <vector>

using namespace ::testing;

bool operator==(const ACRelayBits& lhs, const ACRelayBits& rhs) {
  return lhs.phaseA_LSB == rhs.phaseA_LSB && lhs.phaseA_MSB == rhs.phaseA_MSB &&
         lhs.phaseB_LSB == rhs.phaseB_LSB && lhs.phaseB_MSB == rhs.phaseB_MSB &&
         lhs.phaseC_LSB == rhs.phaseC_LSB && lhs.phaseC_MSB == rhs.phaseC_MSB;
}

TEST(EncodeValues, setBitsForValuesZeroToSixteen) {
  typedef std::pair<uint8_t, uint8_t> phase_bits_t;

  const std::vector<phase_bits_t> phase_bits_sequence = {
      {0b11111111, 0b11111111}, {0b11111111, 0b11111110},
      {0b11111111, 0b11111100}, {0b11111111, 0b11111000},
      {0b11111111, 0b11110000}, {0b11111111, 0b11100000},
      {0b11111111, 0b11000000}, {0b11111111, 0b10000000},
      {0b11111111, 0b00000000}, {0b11111110, 0b00000000},
      {0b11111100, 0b00000000}, {0b11111000, 0b00000000},
      {0b11110000, 0b00000000}, {0b11100000, 0b00000000},
      {0b11000000, 0b00000000}, {0b10000000, 0b00000000},
      {0b00000000, 0b00000000}};  // outputs for AC relays are active-low
  for (uint8_t state_value = 0; state_value <= 16; state_value++)
    ASSERT_EQ((ACRelayBits{phase_bits_sequence[state_value].first,
                           phase_bits_sequence[state_value].second, 0xFF, 0xFF,
                           0xFF, 0xFF}),
              encode(RelayState{state_value, 0, 0, 0}));
}

TEST(EncodeValues, encodeBalancedRelayValues) {
  // chosen arbitrarily
  const uint8_t AC_balanced_value = 5;
  const uint8_t DC_value = 2;

  const uint8_t balanced_bits = 0b11100000;  // must match balanced value

  const RelayState balancedState{AC_balanced_value, AC_balanced_value,
                                 AC_balanced_value, DC_value};
  ASSERT_EQ((ACRelayBits{0xFF, balanced_bits, 0xFF, balanced_bits, 0xFF,
                         balanced_bits}),
            encode(balancedState));
}

TEST(EncodeValues, encodeUnbalancedRelayValues) {
  // chosen arbitrarily
  const uint8_t phaseA_val = 1;
  const uint8_t phaseB_val = 2;
  const uint8_t phaseC_val = 3;
  const uint8_t DC_value = 1;

  // must match chosen values
  const uint8_t phaseA_bits = 0b11111110;
  const uint8_t phaseB_bits = 0b11111100;
  const uint8_t phaseC_bits = 0b11111000;

  const RelayState unbalancedState{phaseA_val, phaseB_val, phaseC_val,
                                   DC_value};
  ASSERT_EQ(
      (ACRelayBits{0xFF, phaseA_bits, 0xFF, phaseB_bits, 0xFF, phaseC_bits}),
      encode(unbalancedState));
}
