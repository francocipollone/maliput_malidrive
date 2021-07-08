// Copyright 2021 Toyota Research Institute
#include "maliput_malidrive/test_utilities/road_geometry_configuration_for_xodrs.h"

#include <unordered_map>

#include <maliput/api/road_geometry.h>
#include <maliput/math/vector.h>

#include "maliput_malidrive/constants.h"

namespace malidrive {
namespace test {

std::optional<builder::RoadGeometryConfiguration> GetRoadGeometryConfigurationFor(const std::string& xodr_file_name) {
  const static maliput::math::Vector3 kZeroVector{0., 0., 0.};
  const static InertialToLaneMappingConfig kInertialToLaneMappingConfig(constants::kExplorationRadius,
                                                                        constants::kNumIterations);
  const builder::BuildPolicy kBuildPolicy{builder::BuildPolicy::Type::kSequential};
  const builder::RoadGeometryConfiguration::SimplificationPolicy kSimplificationPolicy{
      builder::RoadGeometryConfiguration::SimplificationPolicy::kNone};
  const builder::RoadGeometryConfiguration::ToleranceSelectionPolicy kToleranceSelectionPolicy{
      builder::RoadGeometryConfiguration::ToleranceSelectionPolicy::kAutomaticSelection};
  const builder::RoadGeometryConfiguration::StandardStrictnessPolicy kStandardStrictnessPolicy{
      builder::RoadGeometryConfiguration::StandardStrictnessPolicy::kPermissive};
  const bool kOmitNondrivableLanes{false};

  const static std::unordered_map<std::string, builder::RoadGeometryConfiguration> kXodrConfigurations{
      {"SingleLane.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"LineSingleLane"},
                                                             {"odr/SingleLane.xodr"},
                                                             5e-2,
                                                             1e-3,
                                                             constants::kScaleLength,
                                                             kZeroVector,
                                                             kInertialToLaneMappingConfig,
                                                             kBuildPolicy,
                                                             kSimplificationPolicy,
                                                             kToleranceSelectionPolicy,
                                                             kStandardStrictnessPolicy,
                                                             kOmitNondrivableLanes}},
      {"ArcLane.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"ArcSingleLane"},
                                                          {"odr/ArcLane.xodr"},
                                                          5e-2,
                                                          1e-3,
                                                          constants::kScaleLength,
                                                          kZeroVector,
                                                          kInertialToLaneMappingConfig,
                                                          kBuildPolicy,
                                                          kSimplificationPolicy,
                                                          kToleranceSelectionPolicy,
                                                          kStandardStrictnessPolicy,
                                                          kOmitNondrivableLanes}},
      {"BikingLineLane.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"BikingLineLane"},
                                                                 {"odr/BikingLineLane.xodr"},
                                                                 5e-2,
                                                                 1e-3,
                                                                 constants::kScaleLength,
                                                                 kZeroVector,
                                                                 kInertialToLaneMappingConfig,
                                                                 kBuildPolicy,
                                                                 kSimplificationPolicy,
                                                                 kToleranceSelectionPolicy,
                                                                 kStandardStrictnessPolicy,
                                                                 kOmitNondrivableLanes}},
      {"DisconnectedRoadInJunction.xodr",
       builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"DisconnectedRoadInJunction"},
                                          {"odr/DisconnectedRoadInJunction.xodr"},
                                          5e-2,
                                          1e-3,
                                          constants::kScaleLength,
                                          kZeroVector,
                                          kInertialToLaneMappingConfig,
                                          kBuildPolicy,
                                          kSimplificationPolicy,
                                          kToleranceSelectionPolicy,
                                          kStandardStrictnessPolicy,
                                          kOmitNondrivableLanes}},
      {"SShapeRoad.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"SShapeRoad"},
                                                             {"odr/SShapeRoad.xodr"},
                                                             5e-2,
                                                             1e-3,
                                                             constants::kScaleLength,
                                                             kZeroVector,
                                                             kInertialToLaneMappingConfig,
                                                             kBuildPolicy,
                                                             kSimplificationPolicy,
                                                             kToleranceSelectionPolicy,
                                                             kStandardStrictnessPolicy,
                                                             kOmitNondrivableLanes}},
      {"LShapeRoad.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"LShapeRoad"},
                                                             {"odr/LShapeRoad.xodr"},
                                                             5e-2,
                                                             1e-3,
                                                             constants::kScaleLength,
                                                             kZeroVector,
                                                             kInertialToLaneMappingConfig,
                                                             kBuildPolicy,
                                                             kSimplificationPolicy,
                                                             kToleranceSelectionPolicy,
                                                             kStandardStrictnessPolicy,
                                                             kOmitNondrivableLanes}},
      {"LShapeRoadVariableLanes.xodr",
       builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"LShapeRoadVariableLanes"},
                                          {"odr/LShapeRoadVariableLanes.xodr"},
                                          5e-2,
                                          1e-3,
                                          constants::kScaleLength,
                                          kZeroVector,
                                          kInertialToLaneMappingConfig,
                                          kBuildPolicy,
                                          kSimplificationPolicy,
                                          kToleranceSelectionPolicy,
                                          kStandardStrictnessPolicy,
                                          kOmitNondrivableLanes}},
      {"LineMultipleSections.xodr",
       builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"LineMultipleSections"},
                                          {"odr/LineMultipleSections.xodr"},
                                          5e-2,
                                          1e-3,
                                          constants::kScaleLength,
                                          kZeroVector,
                                          kInertialToLaneMappingConfig,
                                          kBuildPolicy,
                                          kSimplificationPolicy,
                                          kToleranceSelectionPolicy,
                                          kStandardStrictnessPolicy,
                                          kOmitNondrivableLanes}},
      {"LineMultipleSectionsMoreCases.xodr",
       builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"LineMultipleSectionsMoreCases"},
                                          {"odr/LineMultipleSectionsMoreCases.xodr"},
                                          5e-2,
                                          1e-3,
                                          constants::kScaleLength,
                                          kZeroVector,
                                          kInertialToLaneMappingConfig,
                                          kBuildPolicy,
                                          kSimplificationPolicy,
                                          kToleranceSelectionPolicy,
                                          kStandardStrictnessPolicy,
                                          kOmitNondrivableLanes}},
      {"LineMultipleSpeeds.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"LineMultipleSpeeds"},
                                                                     {"odr/LineMultipleSpeeds.xodr"},
                                                                     5e-2,
                                                                     1e-3,
                                                                     constants::kScaleLength,
                                                                     kZeroVector,
                                                                     kInertialToLaneMappingConfig,
                                                                     kBuildPolicy,
                                                                     kSimplificationPolicy,
                                                                     kToleranceSelectionPolicy,
                                                                     kStandardStrictnessPolicy,
                                                                     kOmitNondrivableLanes}},
      {"LineVariableOffset.xodr",
       builder::RoadGeometryConfiguration{
           maliput::api::RoadGeometryId{"LineVariableOffset"},
           {"odr/LineVariableOffset.xodr"},
           5e-2,
           1e-3,
           constants::kScaleLength,
           maliput::math::Vector3{0., 0., 0.},
           InertialToLaneMappingConfig(constants::kExplorationRadius, constants::kNumIterations)}},
      {"LineVariableWidth.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"LineVariableWidth"},
                                                                    {"odr/LineVariableWidth.xodr"},
                                                                    5e-2,
                                                                    1e-3,
                                                                    constants::kScaleLength,
                                                                    kZeroVector,
                                                                    kInertialToLaneMappingConfig,
                                                                    kBuildPolicy,
                                                                    kSimplificationPolicy,
                                                                    kToleranceSelectionPolicy,
                                                                    kStandardStrictnessPolicy,
                                                                    kOmitNondrivableLanes}},
      {"ParkingGarageRamp.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"ParkingGarageRamp"},
                                                                    {"odr/ParkingGarageRamp.xodr"},
                                                                    5e-2,
                                                                    1e-3,
                                                                    constants::kScaleLength,
                                                                    kZeroVector,
                                                                    kInertialToLaneMappingConfig,
                                                                    kBuildPolicy,
                                                                    kSimplificationPolicy,
                                                                    kToleranceSelectionPolicy,
                                                                    kStandardStrictnessPolicy,
                                                                    kOmitNondrivableLanes}},
      {"RRLongRoad.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"RRLongRoad"},
                                                             {"odr/RRLongRoad.xodr"},
                                                             5e-2,
                                                             1e-3,
                                                             constants::kScaleLength,
                                                             kZeroVector,
                                                             kInertialToLaneMappingConfig,
                                                             kBuildPolicy,
                                                             kSimplificationPolicy,
                                                             kToleranceSelectionPolicy,
                                                             kStandardStrictnessPolicy,
                                                             kOmitNondrivableLanes}},
      {"SShapeSuperelevatedRoad.xodr",
       builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"SShapeSuperelevatedRoad"},
                                          {"odr/SShapeSuperelevatedRoad.xodr"},
                                          5e-2,
                                          1e-3,
                                          constants::kScaleLength,
                                          kZeroVector,
                                          kInertialToLaneMappingConfig,
                                          kBuildPolicy,
                                          kSimplificationPolicy,
                                          kToleranceSelectionPolicy,
                                          kStandardStrictnessPolicy,
                                          kOmitNondrivableLanes}},
      {"TShapeRoad.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"TShapeRoad"},
                                                             {"odr/TShapeRoad.xodr"},
                                                             5e-2,
                                                             1e-3,
                                                             constants::kScaleLength,
                                                             kZeroVector,
                                                             kInertialToLaneMappingConfig,
                                                             kBuildPolicy,
                                                             kSimplificationPolicy,
                                                             kToleranceSelectionPolicy,
                                                             kStandardStrictnessPolicy,
                                                             kOmitNondrivableLanes}},
      {"Highway.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"Highway"},
                                                          {"odr/Highway.xodr"},
                                                          5e-2,
                                                          1e-3,
                                                          constants::kScaleLength,
                                                          kZeroVector,
                                                          kInertialToLaneMappingConfig,
                                                          kBuildPolicy,
                                                          kSimplificationPolicy,
                                                          kToleranceSelectionPolicy,
                                                          kStandardStrictnessPolicy,
                                                          kOmitNondrivableLanes}},
      {"Figure8.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"Figure8"},
                                                          {"odr/Figure8.xodr"},
                                                          1e-3,
                                                          1e-3,
                                                          constants::kScaleLength,
                                                          kZeroVector,
                                                          kInertialToLaneMappingConfig,
                                                          kBuildPolicy,
                                                          kSimplificationPolicy,
                                                          kToleranceSelectionPolicy,
                                                          kStandardStrictnessPolicy,
                                                          kOmitNondrivableLanes}},
      {"RRFigure8.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"RRFigure8"},
                                                            {"odr/RRFigure8.xodr"},
                                                            3e-1,
                                                            3e-1,
                                                            constants::kScaleLength,
                                                            kZeroVector,
                                                            kInertialToLaneMappingConfig,
                                                            kBuildPolicy,
                                                            kSimplificationPolicy,
                                                            kToleranceSelectionPolicy,
                                                            kStandardStrictnessPolicy,
                                                            kOmitNondrivableLanes}},
      {"StraightForward.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"StraightForward"},
                                                                  {"odr/StraightForward.xodr"},
                                                                  1e-3,
                                                                  1e-3,
                                                                  constants::kScaleLength,
                                                                  kZeroVector,
                                                                  kInertialToLaneMappingConfig,
                                                                  kBuildPolicy,
                                                                  kSimplificationPolicy,
                                                                  kToleranceSelectionPolicy,
                                                                  kStandardStrictnessPolicy,
                                                                  kOmitNondrivableLanes}},
      {"SingleRoadComplexDescription.xodr",
       builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"SingleRoadComplexDescription"},
                                          {"odr/SingleRoadComplexDescription.xodr"},
                                          5e-2,
                                          1e-3,
                                          constants::kScaleLength,
                                          kZeroVector,
                                          kInertialToLaneMappingConfig,
                                          kBuildPolicy,
                                          kSimplificationPolicy,
                                          kToleranceSelectionPolicy,
                                          kStandardStrictnessPolicy,
                                          kOmitNondrivableLanes}},
      {"SingleRoadComplexDescription2.xodr",
       builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"SingleRoadComplexDescription2"},
                                          {"odr/SingleRoadComplexDescription2.xodr"},
                                          5e-2,
                                          1e-3,
                                          constants::kScaleLength,
                                          kZeroVector,
                                          kInertialToLaneMappingConfig,
                                          kBuildPolicy,
                                          kSimplificationPolicy,
                                          kToleranceSelectionPolicy,
                                          kStandardStrictnessPolicy,
                                          kOmitNondrivableLanes}},
      {"SingleRoadNanValues.xodr",
       builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"SingleRoadNanValues"},
                                          {"odr/SingleRoadNanValues.xodr"},
                                          5e-2,
                                          1e-3,
                                          constants::kScaleLength,
                                          kZeroVector,
                                          kInertialToLaneMappingConfig,
                                          kBuildPolicy,
                                          kSimplificationPolicy,
                                          kToleranceSelectionPolicy,
                                          kStandardStrictnessPolicy,
                                          kOmitNondrivableLanes}},
      {"SingleRoadHighCoefficients.xodr",
       builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"SingleRoadHighCoefficients"},
                                          {"odr/SingleRoadHighCoefficients.xodr"},
                                          5e-2,
                                          1e-3,
                                          constants::kScaleLength,
                                          kZeroVector,
                                          kInertialToLaneMappingConfig,
                                          kBuildPolicy,
                                          kSimplificationPolicy,
                                          kToleranceSelectionPolicy,
                                          kStandardStrictnessPolicy,
                                          kOmitNondrivableLanes}},
      {"SingleRoadTinyGeometry.xodr",
       builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"SingleRoadTinyGeometry"},
                                          {"odr/SingleRoadTinyGeometry.xodr"},
                                          5e-2,
                                          1e-3,
                                          constants::kScaleLength,
                                          kZeroVector,
                                          kInertialToLaneMappingConfig,
                                          kBuildPolicy,
                                          kSimplificationPolicy,
                                          kToleranceSelectionPolicy,
                                          kStandardStrictnessPolicy,
                                          kOmitNondrivableLanes}},
  };
  return kXodrConfigurations.find(xodr_file_name) != kXodrConfigurations.end()
             ? std::make_optional<builder::RoadGeometryConfiguration>(kXodrConfigurations.at(xodr_file_name))
             : std::nullopt;
}

}  // namespace test
}  // namespace malidrive
