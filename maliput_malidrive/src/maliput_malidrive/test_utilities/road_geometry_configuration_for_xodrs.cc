// Copyright 2021 Toyota Research Institute
#include "maliput_malidrive/test_utilities/road_geometry_configuration_for_xodrs.h"

#include <unordered_map>

#include "maliput/api/road_geometry.h"
#include "maliput_malidrive/constants.h"

namespace malidrive {
namespace test {

std::optional<builder::RoadGeometryConfiguration> GetRoadGeometryConfigurationFor(const std::string& xodr_file_name) {
  const static std::unordered_map<std::string, builder::RoadGeometryConfiguration> kXodrConfigurations{
      {"SingleLane.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"LineSingleLane"},
                                                             {"SingleLane.xodr"},
                                                             5e-2,
                                                             1e-3,
                                                             constants::kScaleLength,
                                                             InertialToLaneMappingConfig(constants::kExplorationRadius,
                                                                                         constants::kNumIterations)}},
      {"ArcLane.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"ArcSingleLane"},
                                                          {"ArcLane.xodr"},
                                                          5e-2,
                                                          1e-3,
                                                          constants::kScaleLength,
                                                          InertialToLaneMappingConfig(constants::kExplorationRadius,
                                                                                      constants::kNumIterations)}},
      {"SShapeRoad.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"SShapeRoad"},
                                                             {"SShapeRoad.xodr"},
                                                             5e-2,
                                                             1e-3,
                                                             constants::kScaleLength,
                                                             InertialToLaneMappingConfig(constants::kExplorationRadius,
                                                                                         constants::kNumIterations)}},
      {"LShapeRoad.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"LShapeRoad"},
                                                             {"LShapeRoad.xodr"},
                                                             5e-2,
                                                             1e-3,
                                                             constants::kScaleLength,
                                                             InertialToLaneMappingConfig(constants::kExplorationRadius,
                                                                                         constants::kNumIterations)}},
      {"LShapeRoadVariableLanes.xodr",
       builder::RoadGeometryConfiguration{
           maliput::api::RoadGeometryId{"LShapeRoadVariableLanes"},
           {"LShapeRoadVariableLanes.xodr"},
           5e-2,
           1e-3,
           constants::kScaleLength,
           InertialToLaneMappingConfig(constants::kExplorationRadius, constants::kNumIterations)}},
      {"LineMultipleSections.xodr",
       builder::RoadGeometryConfiguration{
           maliput::api::RoadGeometryId{"LineMultipleSections"},
           {"LineMultipleSections.xodr"},
           5e-2,
           1e-3,
           constants::kScaleLength,
           InertialToLaneMappingConfig(constants::kExplorationRadius, constants::kNumIterations)}},
      {"LineVariableOffset.xodr",
       builder::RoadGeometryConfiguration{
           maliput::api::RoadGeometryId{"LineVariableOffset"},
           {"LineVariableOffset.xodr"},
           5e-2,
           1e-3,
           constants::kScaleLength,
           InertialToLaneMappingConfig(constants::kExplorationRadius, constants::kNumIterations)}},
      {"LineVariableWidth.xodr",
       builder::RoadGeometryConfiguration{
           maliput::api::RoadGeometryId{"LineVariableWidth"},
           {"LineVariableWidth.xodr"},
           5e-2,
           1e-3,
           constants::kScaleLength,
           InertialToLaneMappingConfig(constants::kExplorationRadius, constants::kNumIterations)}},
      {"ParkingGarageRamp.xodr",
       builder::RoadGeometryConfiguration{
           maliput::api::RoadGeometryId{"ParkingGarageRamp"},
           {"ParkingGarageRamp.xodr"},
           5e-2,
           1e-3,
           constants::kScaleLength,
           InertialToLaneMappingConfig(constants::kExplorationRadius, constants::kNumIterations)}},
      {"RRLongRoad.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"RRLongRoad"},
                                                             {"RRLongRoad.xodr"},
                                                             5e-2,
                                                             1e-3,
                                                             constants::kScaleLength,
                                                             InertialToLaneMappingConfig(constants::kExplorationRadius,
                                                                                         constants::kNumIterations)}},
      {"SShapeSuperelevatedRoad.xodr",
       builder::RoadGeometryConfiguration{
           maliput::api::RoadGeometryId{"SShapeSuperelevatedRoad"},
           {"SShapeSuperelevatedRoad.xodr"},
           5e-2,
           1e-3,
           constants::kScaleLength,
           InertialToLaneMappingConfig(constants::kExplorationRadius, constants::kNumIterations)}},
      {"TShapeRoad.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"TShapeRoad"},
                                                             {"TShapeRoad.xodr"},
                                                             5e-2,
                                                             1e-3,
                                                             constants::kScaleLength,
                                                             InertialToLaneMappingConfig(constants::kExplorationRadius,
                                                                                         constants::kNumIterations)}},
      {"Highway.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"Highway"},
                                                          {"Highway.xodr"},
                                                          5e-2,
                                                          1e-3,
                                                          constants::kScaleLength,
                                                          InertialToLaneMappingConfig(constants::kExplorationRadius,
                                                                                      constants::kNumIterations)}},
      {"Figure8.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"Figure8"},
                                                          {"Figure8.xodr"},
                                                          1e-3,
                                                          1e-3,
                                                          constants::kScaleLength,
                                                          InertialToLaneMappingConfig(constants::kExplorationRadius,
                                                                                      constants::kNumIterations)}},
      {"RRFigure8.xodr", builder::RoadGeometryConfiguration{maliput::api::RoadGeometryId{"RRFigure8"},
                                                            {"RRFigure8.xodr"},
                                                            3e-1,
                                                            3e-1,
                                                            constants::kScaleLength,
                                                            InertialToLaneMappingConfig(constants::kExplorationRadius,
                                                                                        constants::kNumIterations)}},
  };
  return kXodrConfigurations.find(xodr_file_name) != kXodrConfigurations.end()
             ? std::make_optional<builder::RoadGeometryConfiguration>(kXodrConfigurations.at(xodr_file_name))
             : std::nullopt;
}

}  // namespace test
}  // namespace malidrive
