// Copyright 2020 Toyota Research Institute
#pragma once

#include <memory>

#include "maliput/api/lane_data.h"
#include "maliput/geometry_base/lane.h"

#include "maliput/math/vector.h"
#include "maliput_malidrive/base/world_to_opendrive_transform.h"
#include "maliput_malidrive/common/macros.h"
#include "maliput_malidrive/road_curve/function.h"
#include "maliput_malidrive/road_curve/road_curve.h"
#include "maliput_malidrive/road_curve/road_curve_offset.h"

namespace malidrive {

/// Describes a Lane whose centerline is a offset of a
/// road_curve::RoadCurve at a certain lane offset which is a
/// road_curve::Function. Lane's width varies with `s`.
///
/// In maps an XODR Lane within a certain XODR LaneSection.
class MalidriveLane : public maliput::geometry_base::Lane {
 public:
  MALIDRIVE_NO_COPY_NO_MOVE_NO_ASSIGN(MalidriveLane);

  /// Constructs a Lane.
  ///
  /// @param id Lane's ID.
  /// @param xodr_track The XODR Road ID. It must be non negative.
  /// @param xodr_lane_id The XODR Lane ID within a XODR LaneSection.
  /// @param elevation_bounds Lane's elevation boundaries. They are constant for
  ///        the entire volume.
  /// @param road_curve A pointer to a road_curve::RoadCurve. It must
  ///        not be nullptr. Ownership of this object is not managed by this
  ///        class.
  /// @param lane_width A road_curve::Function describing the width of the lane,
  ///        i.e. its lane bounds. It must not be nullptr. Its range must be
  ///        within `road_curve->linear_tolerance()` distance of
  ///        @p road_curve's.
  /// @param lane_offset A road_curve::Function describing the offset of this
  ///        Lane's centerline. It must not be nullptr. Its range must be within
  ///        `road_curve->linear_tolerance()` distance of @p road_curve's.
  /// @param p0 The value of the @f$ p @f$ parameter of @p road_curve that
  ///        matches the start of the Lane.
  /// @param p1 The value of the @f$ p @f$ parameter of @p road_curve that
  ///        matches the finish of the Lane.
  /// @note When the ground curve's arc length in range `p1` - `p0` is less than
  ///       `road_curve->linear_tolerance()`, an instance will not host a
  ///       RoadCurveOffset to populate `p_from_s_` and `s_from_p_`. Instead,
  ///       it will create linear functions to convert back and forth `s` and
  ///       `p` parameters.
  /// @throws maliput::common::assertion_error When @p xodr_track is negative.
  /// @throws maliput::common::assertion_error When @p lane_width,
  ///         @p lane_offset or @p road_curve are nullptr.
  /// @throws maliput::common::assertion_error When @p lane_width's or
  ///         @p lane_offset's range are not within
  ///         `road_curve->linear_tolerance()` of [ @p p0, @p p1 ] range.
  MalidriveLane(const maliput::api::LaneId& id, int xodr_track, int xodr_lane_id,
                const maliput::api::HBounds& elevation_bounds, const road_curve::RoadCurve* road_curve,
                std::unique_ptr<road_curve::Function> lane_width, std::unique_ptr<road_curve::Function> lane_offset,
                double p0, double p1);

  /// @return The OpenDRIVE Road Id, which is also referred to as Track Id. It
  ///         is a non-negative number.
  int get_track() const { return xodr_track_; }

  /// @return The OpenDRIVE Lane Id Road Id.
  int get_lane_id() const { return xodr_lane_id_; }

  /// @return The TRACK Frame start `s` coordinate of the XODR LaneSection this
  ///         lane is part of. It is a non-negative quantity.
  double get_track_s_start() const { return p0_; }

  /// @return The TRACK Frame end `s` coordinate of the XODR LaneSection this
  ///         lane is part of.
  double get_track_s_end() const { return p1_; }

  /// Converts `lane_s` coordinate in the LANE Frame to the TRACK Frame `s`
  /// coordinate the ODRM uses.
  ///
  /// @param lane_s The LANE Frame `s` coordinate to convert. It must be in
  ///        [0; maliput::api::Lane::length()].
  /// @return The TRACK Frame `s` coordinate which matches `lane_s`.
  /// @throws maliput::common::assertion_error When @p lane_s is not in range.
  double TrackSFromLaneS(double lane_s) const { return p_from_s_(lane_s); }

  /// Converts `track_s` coordinate in the TRACK Frame `s` coordinate the ODRM uses
  /// to the `s` coordinate in the LANE frame.
  ///
  /// @param track_s The TRACK Frame `s` coordinate to convert. It must be in
  ///        [0; `get_track_s_end()` - get_track_s_start()].
  /// @return The LANE Frame `s` coordinate which matches `track_s`.
  /// @throws maliput::common::assertion_error When @p track_s is not in range.
  double LaneSFromTrackS(double track_s) const { return s_from_p_(track_s); }

  /// Performs the same computation as `ToGeoPosition()` but, instead of
  /// returning a coordinate in the World Inertial Frame, it returns the
  /// coordinate in the OpenDRIVE Inertial Frame.
  maliput::api::GeoPosition ToInertialPosition(const maliput::api::LanePosition& lane_pos) const;

 private:
  // maliput::api::Lane private virtual method implementations.
  //@{
  double do_length() const override { return length_; }
  maliput::api::RBounds do_lane_bounds(double s) const override;
  maliput::api::RBounds do_segment_bounds(double s) const override;
  maliput::api::HBounds do_elevation_bounds(double, double) const override { return elevation_bounds_; }
  maliput::api::GeoPosition DoToGeoPosition(const maliput::api::LanePosition& lane_pos) const override;
  maliput::api::LanePositionResult DoToLanePosition(const maliput::api::GeoPosition& geo_pos_world) const override;
  maliput::api::Rotation DoGetOrientation(const maliput::api::LanePosition& lane_pos) const override;
  maliput::api::LanePosition DoEvalMotionDerivatives(const maliput::api::LanePosition& position,
                                                     const maliput::api::IsoLaneVelocity& velocity) const override;
  //@}

  // @returns The r-coordinate in LANE Frame from `(p, r)` in the `road_curve`
  //          Frame.
  double to_lane_r(double p, double r) const { return r - lane_offset_->f(p); }

  // @returns The r-coordinate in `road_curve` Frame from `(p, r)` in the LANE
  //          Frame.
  double to_reference_r(double p, double r) const { return r + lane_offset_->f(p); }

  // @returns The WorldToOpenDriveTransform to convert back and forth from the
  //          INERTIAL Frame and OpenDRIVE INERTIAL Frame.
  const WorldToOpenDriveTransform& get_world_to_opendrive_transform() const;

  // @returns The prh coordinate in LANE Frame from `xyz` in the INERTIAL Frame.
  maliput::math::Vector3 InertialFrameToLaneFrame(const maliput::math::Vector3& xyz) const;

  const int xodr_track_{};
  const int xodr_lane_id_{};
  const maliput::api::HBounds elevation_bounds_{};
  const road_curve::RoadCurve* road_curve_{};
  const double p0_{};
  const double p1_{};
  const road_curve::RoadCurveOffset road_curve_offset_;
  std::unique_ptr<road_curve::Function> lane_width_{};
  std::unique_ptr<road_curve::Function> lane_offset_{};
  double length_{};
  std::function<double(double)> p_from_s_{};
  std::function<double(double)> s_from_p_{};
  std::function<double(double)> s_range_validation_{};
};

}  // namespace malidrive
