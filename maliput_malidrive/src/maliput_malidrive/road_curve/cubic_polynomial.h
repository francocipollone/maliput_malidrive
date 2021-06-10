// Copyright 2020 Toyota Research Institute
#pragma once

#include "maliput_malidrive/common/macros.h"
#include "maliput_malidrive/road_curve/function.h"
#include "maliput_malidrive/road_curve/open_range_validator.h"

namespace malidrive {
namespace road_curve {

/// Describes cubic polynomial function behind Function interface.
///
/// Models the following function:
///
/// @f$ F(p) = a p^3 + b p^2 + c p + d / p ∈ [p0; p1] @f$.
///
/// Provided that it is a polynomial: @f$ F(p) is G¹ ⩝ p ∈ ℝ @f$.
///
/// Queries accept p ∈ [p0, p1] with a linear tolerance.
class CubicPolynomial : public Function {
 public:
  MALIDRIVE_NO_COPY_NO_MOVE_NO_ASSIGN(CubicPolynomial);

  /// Constructs a cubic polynomial.
  ///
  /// @param a The coefficient of third degree monomial.
  /// @param b The coefficient of second degree monomial.
  /// @param c The coefficient of first degree monomial.
  /// @param d The coefficient of independent monomial.
  /// @param p0 Lower bound extreme of the parameter range. It must not be
  ///        negative and must be less than @p p1.
  /// @param p1 Upper bound extreme of the parameter range. It must be greater
  ///        than @p p0.
  /// @param linear_tolerance Tolerance of the range [@p p0; @p p1] that will be
  ///        used to evaluate the parameter.
  /// @throws maliput::common::assertion_error When @p p0 is negative or @p p1
  ///         is less or equal to @p p0.
  /// @throws maliput::common::assertion_error When @p linear_tolerance is not
  ///         positive.
  CubicPolynomial(double a, double b, double c, double d, double p0, double p1, double linear_tolerance)
      : a_(a),
        b_(b),
        c_(c),
        d_(d),
        p0_(p0),
        p1_(p1),
        validate_p_(OpenRangeValidator::GetAbsoluteEpsilonValidator(p0_, p1_, linear_tolerance, Function::kEpsilon)) {
    MALIDRIVE_THROW_UNLESS(p0_ >= 0);
    MALIDRIVE_THROW_UNLESS(p1_ > p0_);
    MALIDRIVE_THROW_UNLESS(linear_tolerance > 0.);
  }

 private:
  double do_f(double p) const override {
    p = validate_p_(p);
    return a_ * p * p * p + b_ * p * p + c_ * p + d_;
  }

  double do_f_dot(double p) const override {
    p = validate_p_(p);
    return 3. * a_ * p * p + 2. * b_ * p + c_;
  }

  double do_f_dot_dot(double p) const override {
    p = validate_p_(p);
    return 6. * a_ * p + 2 * b_;
  }

  double do_p0() const override { return p0_; }
  double do_p1() const override { return p1_; }
  bool DoIsG1Contiguous() const override { return true; }

  const double a_{};
  const double b_{};
  const double c_{};
  const double d_{};
  const double p0_{};
  const double p1_{};
  // Validates that p is within [p0_, p1_] with linear_tolerance.
  const OpenRangeValidator validate_p_;
};

}  // namespace road_curve
}  // namespace malidrive
