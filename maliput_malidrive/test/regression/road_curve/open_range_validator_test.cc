// Copyright 2020 Toyota Research Institute
#include "maliput_malidrive/road_curve/open_range_validator.h"

#include <gtest/gtest.h>
#include <maliput/common/assertion_error.h>

namespace malidrive {
namespace road_curve {
namespace test {

class OpenRangeValidatorConstructorValidation : public ::testing::Test {
 protected:
  void SetUp() override {}

  static constexpr double kMin{0.5};
  static constexpr double kMax{1034.};
  static constexpr double kTolerance{1e-3};
  static constexpr double kEpsilon{1e-5};
};

// no throw
TEST_F(OpenRangeValidatorConstructorValidation, WellConstructed) {
  EXPECT_NO_THROW({ OpenRangeValidator::GetAbsoluteEpsilonValidator(kMin, kMax, kTolerance, kEpsilon); });
}

// relative epsilon > tolerance
// kEpsilon * (kMax - kMin) > tolerance
TEST_F(OpenRangeValidatorConstructorValidation, RelativeEpsilonGreaterThanTolerance) {
  EXPECT_THROW({ OpenRangeValidator::GetRelativeEpsilonValidator(kMin, kMax, kTolerance, kEpsilon); },
               maliput::common::assertion_error);
}

// min > max
TEST_F(OpenRangeValidatorConstructorValidation, MinGreaterThanMax) {
  EXPECT_THROW({ OpenRangeValidator::GetAbsoluteEpsilonValidator(kMax, kMin, kTolerance, kEpsilon); },
               maliput::common::assertion_error);
}

// epsilon > tolerance
TEST_F(OpenRangeValidatorConstructorValidation, EpsilonGreaterThanTolerance) {
  EXPECT_THROW({ OpenRangeValidator::GetAbsoluteEpsilonValidator(kMin, kMax, kEpsilon, kTolerance); },
               maliput::common::assertion_error);
}

// min + epsilon > max (applies the other way around, it's specially provided
// in constructor code to validate numerical error).
TEST_F(OpenRangeValidatorConstructorValidation, MinPlusEpsilonGreaterThanMax) {
  EXPECT_THROW({ OpenRangeValidator::GetAbsoluteEpsilonValidator(kMin, kMax, 2 * kMax, kMax); },
               maliput::common::assertion_error);
}

class OpenRangeValidatorAbsoluteEpsilonRange : public ::testing::Test {
 protected:
  void SetUp() override {}

  static constexpr double kMin{0.5};
  static constexpr double kMax{3.};
  static constexpr double kTolerance{1e-3};
  static constexpr double kEpsilon{1e-5};
  const OpenRangeValidator dut{OpenRangeValidator::GetAbsoluteEpsilonValidator(kMin, kMax, kTolerance, kEpsilon)};
};

// In the middle of the range.
TEST_F(OpenRangeValidatorAbsoluteEpsilonRange, MiddleOfRange) {
  const double kS{2.};
  EXPECT_DOUBLE_EQ(dut(kS), kS);
}

// In the maximum of the range.
TEST_F(OpenRangeValidatorAbsoluteEpsilonRange, MaxLimitOfRange) {
  const double kS{kMax};
  EXPECT_DOUBLE_EQ(dut(kS), kS - kEpsilon);
}

// In the minimum of the range.
TEST_F(OpenRangeValidatorAbsoluteEpsilonRange, MinLimitOfRange) {
  const double kS{kMin};
  EXPECT_DOUBLE_EQ(dut(kS), kS + kEpsilon);
}

// Exceeding the maximum but within linear tolerance.
TEST_F(OpenRangeValidatorAbsoluteEpsilonRange, ExceedsMaximum) {
  const double kS{kMax + kTolerance / 2.};
  EXPECT_DOUBLE_EQ(dut(kS), kMax - kEpsilon);
}

// Exceeding the minimum but within linear tolerance.
TEST_F(OpenRangeValidatorAbsoluteEpsilonRange, ExceedsMinimum) {
  const double kS{kMin - kTolerance / 2.};
  EXPECT_DOUBLE_EQ(dut(kS), kMin + kEpsilon);
}

// Expects throw because of out of bounds.
TEST_F(OpenRangeValidatorAbsoluteEpsilonRange, OutOfBounds) {
  const double kS{kMax + 10 * kTolerance};
  EXPECT_THROW({ dut(kS); }, maliput::common::assertion_error);
}

class OpenRangeValidatorRelativeEpsilonRange : public ::testing::Test {
 protected:
  void SetUp() override {}

  static constexpr double kMin{0.5};
  static constexpr double kMax{100.5};
  static constexpr double kRange{kMax - kMin};
  static constexpr double kTolerance{1e-3};
  static constexpr double kEpsilon{1e-8};
  static constexpr double kRelativeEpsilon{kEpsilon * kRange};
  const OpenRangeValidator dut{OpenRangeValidator::GetRelativeEpsilonValidator(kMin, kMax, kTolerance, kEpsilon)};
};

// In the middle of the range.
TEST_F(OpenRangeValidatorRelativeEpsilonRange, MiddleOfRange) {
  const double kS{kMax - kMin};
  EXPECT_DOUBLE_EQ(dut(kS), kS);
}

// In the maximum of the range.
TEST_F(OpenRangeValidatorRelativeEpsilonRange, MaxLimitOfRange) {
  const double kS{kMax};
  EXPECT_DOUBLE_EQ(dut(kS), kS - kRelativeEpsilon);
}

// In the minimum of the range.
TEST_F(OpenRangeValidatorRelativeEpsilonRange, MinLimitOfRange) {
  const double kS{kMin};
  EXPECT_DOUBLE_EQ(dut(kS), kS + kRelativeEpsilon);
}

// Exceeding the maximum but within linear tolerance.
TEST_F(OpenRangeValidatorRelativeEpsilonRange, ExceedsMaximum) {
  const double kS{kMax + kTolerance / 2.};
  EXPECT_DOUBLE_EQ(dut(kS), kMax - kRelativeEpsilon);
}

// Exceeding the minimum but within linear tolerance.
TEST_F(OpenRangeValidatorRelativeEpsilonRange, ExceedsMinimum) {
  const double kS{kMin - kTolerance / 2.};
  EXPECT_DOUBLE_EQ(dut(kS), kMin + kRelativeEpsilon);
}

// Expects throw because of out of bounds.
TEST_F(OpenRangeValidatorRelativeEpsilonRange, OutOfBounds) {
  const double kS{kMax + 10 * kTolerance};
  EXPECT_THROW({ dut(kS); }, maliput::common::assertion_error);
}

// Tests behavior when working close to the limit of the precision for both relative and absolute use of epsilon value.
// Considering that the number of useful digits for the double type is about 15(or 16) digits.
class OpenRangeValidatorOutOfPrecisionTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  static constexpr double kMin{0.5};
  static constexpr double kMax{100000.5};
  static constexpr double kTolerance{1e-3};
  static constexpr double kEpsilon{1e-14};
};

TEST_F(OpenRangeValidatorOutOfPrecisionTest, WithAbsoluteEpsilon) {
  const OpenRangeValidator dut{OpenRangeValidator::GetAbsoluteEpsilonValidator(kMin, kMax, kTolerance, kEpsilon)};
  // In the maximum of the range.
  const double kS{kMax};
  // The value isn't clamped because it is beyond of the double precision.
  EXPECT_DOUBLE_EQ(dut(kS), kS);
}

TEST_F(OpenRangeValidatorOutOfPrecisionTest, WithRelativeEpsilon) {
  const double kRelativeEpsilon{kEpsilon * (kMax - kMin)};
  const OpenRangeValidator dut{OpenRangeValidator::GetRelativeEpsilonValidator(kMin, kMax, kTolerance, kEpsilon)};
  // In the maximum of the range.
  const double kS{kMax};
  // The value is clamped because the epsilon value is weighten by the length of the range.
  EXPECT_DOUBLE_EQ(dut(kS), kS - kRelativeEpsilon);
}

}  // namespace test
}  // namespace road_curve
}  // namespace malidrive
