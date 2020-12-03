// Copyright 2020 Toyota Research Institute
#pragma once

#include "maliput_malidrive/common/macros.h"

namespace malidrive {
namespace road_curve {

/// Describes a G¹ scalar function.
/// This interface will be the interface for other specializations which are
/// use by RoadCurve to compose the path of maliput::api::Lane.
///
/// In mathematical terms, let:
///
/// @f$ F(p) @f$ be a function of a real and non-negative parameter.
///
/// And @f$ F(p) @f$ has the following requirements:
///
/// - @f$ F(p) @f$ is G¹ in the interval @f$ [p_0; p_1] @f$.
/// - There exists @f$ F''(p) @f$ in the interval @f$ [p_0; p_1] @f$.
///
/// Provided that @f$ F(p) @f$ can be piecewise defined, implementations must
/// include proper G¹ checks in IsG1Contiguous().
class Function {
 public:
  /// Implementations may opt to allow a tolerance or be up to `kEpsilon` away
  /// from [p0(); p1()].
  static constexpr double kEpsilon = 0.;

  MALIDRIVE_NO_COPY_NO_MOVE_NO_ASSIGN(Function);
  virtual ~Function() = default;

  /// Evaluates @f$ F(p) @f$.
  ///
  /// @param p The parameter. It must be in the range @f$ [`p0()`; `p1()`] @f$.
  /// @throws maliput::common::assertion_error When @p p is not in
  ///         @f$ [`p0()`; `p1()`] @f$.
  /// @return The image of @f$ F(p) @f$.
  double f(double p) const { return do_f(p); }

  /// Evaluates @f$ F'(p) @f$.
  ///
  /// @param p The parameter. It must be in the range @f$ [`p0()`; `p1()`] @f$.
  /// @throws maliput::common::assertion_error When @p p is not in
  ///         @f$ [`p0()`; `p1()`] @f$.
  /// @return The image of @f$ F'(p) @f$.
  double f_dot(double p) const { return do_f_dot(p); }

  /// Evaluates @f$ F''(p) @f$.
  ///
  /// @param p The parameter. It must be in the range @f$ [`p0()`; `p1()`] @f$.
  /// @throws maliput::common::assertion_error When @p p is not in
  ///         @f$ [`p0()`; `p1()`] @f$.
  /// @return The image of @f$ F''(p) @f$.
  double f_dot_dot(double p) const { return do_f_dot_dot(p); }

  /// @returns The lower bound range of @f$ p @f$.
  double p0() const { return do_p0(); }

  /// @returns The upper bound range of @f$ p @f$.
  double p1() const { return do_p1(); }

  /// @return True when @f$ F(p) @f$ is G¹ in the interval @f$ [p_0; p_1] @f$.
  bool IsG1Contiguous() const { return DoIsG1Contiguous(); }

 protected:
  Function() = default;

 private:
  // Virtual private definitions of the public members. Public member
  // constraints apply the same.
  //@{
  virtual double do_f(double p) const = 0;
  virtual double do_f_dot(double p) const = 0;
  virtual double do_f_dot_dot(double p) const = 0;
  virtual double do_p0() const = 0;
  virtual double do_p1() const = 0;
  virtual bool DoIsG1Contiguous() const = 0;
  //@}
};

}  // namespace road_curve
}  // namespace malidrive
