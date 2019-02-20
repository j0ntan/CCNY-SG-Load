#include <gmock/gmock.h>
#include <RelayState.h>
#include <Encode.h>
#include <cmath>
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
  const RelayState balancedState{5, 5, 5, 2};
  auto result = encode(balancedState);
  auto zero_inverted =
      static_cast<uint8_t>(~static_cast<uint8_t>(pow(2, 0) - 1));
  auto five_inverted =
      static_cast<uint8_t>(~static_cast<uint8_t>(pow(2, 5) - 1));
  ACRelayBits equivalent{zero_inverted, five_inverted, zero_inverted,
                         five_inverted, zero_inverted, five_inverted};
  ASSERT_EQ(result, equivalent);
}

TEST(EncodeValues, encodeUnbalancedRelayValues) {
  const RelayState unbalancedState{1, 2, 3, 1};
  auto result = encode(unbalancedState);
  auto zero_inverted =
      static_cast<uint8_t>(~static_cast<uint8_t>(pow(2, 0) - 1));
  auto one_inverted =
      static_cast<uint8_t>(~static_cast<uint8_t>(pow(2, 1) - 1));
  auto two_inverted =
      static_cast<uint8_t>(~static_cast<uint8_t>(pow(2, 2) - 1));
  auto three_inverted =
      static_cast<uint8_t>(~static_cast<uint8_t>(pow(2, 3) - 1));
  ACRelayBits equivalent{zero_inverted, one_inverted,  zero_inverted,
                         two_inverted,  zero_inverted, three_inverted};
  ASSERT_EQ(result, equivalent);
}
