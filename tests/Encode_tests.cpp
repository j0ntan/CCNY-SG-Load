#include <gmock/gmock.h>
#include <RelayState.h>
#include <Encode.h>
#include <cmath>

using namespace ::testing;

TEST(EncodeValues, setBitsForValuesZeroToSixteen) {
  const uint8_t RESET_VALUE = 0xFF;

  for (uint8_t value = 0; value <= 8; value++) {
    uint8_t msb = RESET_VALUE, lsb = RESET_VALUE;
    auto decimal_equivalent = (pow(2, 8) - 1) - (pow(2, value) - 1);
    auto expected = static_cast<uint8_t>(decimal_equivalent);

    helper::setPhaseBits(msb, lsb, value);
    EXPECT_EQ(msb, 0xFF);
    EXPECT_EQ(lsb, expected);
  }

  for (uint8_t value = 9; value <= 16; value++) {
    uint8_t msb = RESET_VALUE, lsb = RESET_VALUE;
    auto decimal_equivalent = (pow(2, 8) - 1) - (pow(2, value - 8) - 1);
    auto expected = static_cast<uint8_t>(decimal_equivalent);

    helper::setPhaseBits(msb, lsb, value);
    EXPECT_EQ(msb, expected);
    EXPECT_EQ(lsb, 0x00);
    msb = lsb = RESET_VALUE;
  }
}

bool operator==(const ACRelayBits& lhs, const ACRelayBits& rhs) {
  return lhs.phaseA_LSB == rhs.phaseA_LSB && lhs.phaseA_MSB == rhs.phaseA_MSB &&
         lhs.phaseB_LSB == rhs.phaseB_LSB && lhs.phaseB_MSB == rhs.phaseB_MSB &&
         lhs.phaseC_LSB == rhs.phaseC_LSB && lhs.phaseC_MSB == rhs.phaseC_MSB;
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
