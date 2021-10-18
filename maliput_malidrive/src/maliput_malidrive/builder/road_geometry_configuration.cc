// Copyright 2021 Toyota Research Institute
#include "maliput_malidrive/builder/road_geometry_configuration.h"

#include <map>
#include <sstream>

#include "maliput_malidrive/common/macros.h"

namespace malidrive {
namespace builder {
namespace {

// Holds the conversion from string(keys) to BuildPolicy::Type(values);
const std::map<std::string, BuildPolicy::Type> str_to_build_policy_type{{"sequential", BuildPolicy::Type::kSequential},
                                                                        {"parallel", BuildPolicy::Type::kParallel}};

// Holds the conversion from BuildPolicy::Type(keys) to string(values);
const std::map<BuildPolicy::Type, std::string> build_policy_type_to_str{{BuildPolicy::Type::kSequential, "sequential"},
                                                                        {BuildPolicy::Type::kParallel, "parallel"}};

// Holds the conversion from string(keys) to SimplificationPolicy(values);
const std::map<std::string, RoadGeometryConfiguration::SimplificationPolicy> str_to_simplification_policy{
    {"none", RoadGeometryConfiguration::SimplificationPolicy::kNone},
    {"simplify", RoadGeometryConfiguration::SimplificationPolicy::kSimplifyWithinToleranceAndKeepGeometryModel}};

// Holds the conversion from SimplificationPolicy(keys) to string(values);
const std::map<RoadGeometryConfiguration::SimplificationPolicy, std::string> simplification_policy_to_str{
    {RoadGeometryConfiguration::SimplificationPolicy::kNone, "none"},
    {RoadGeometryConfiguration::SimplificationPolicy::kSimplifyWithinToleranceAndKeepGeometryModel, "simplify"}};

// Holds the conversion from string(keys) to StandardStrictnessPolicy(values);
const std::map<std::string, RoadGeometryConfiguration::StandardStrictnessPolicy> str_to_standard_strictness_policy{
    {"strict", RoadGeometryConfiguration::StandardStrictnessPolicy::kStrict},
    {"allow_schema_errors", RoadGeometryConfiguration::StandardStrictnessPolicy::kAllowSchemaErrors},
    {"allow_semantic_errors", RoadGeometryConfiguration::StandardStrictnessPolicy::kAllowSemanticErrors},
    {"permissive", RoadGeometryConfiguration::StandardStrictnessPolicy::kPermissive}};

// Holds the conversion from StandardStrictnessPolicy(keys) to string(values);
const std::map<RoadGeometryConfiguration::StandardStrictnessPolicy, std::string> standard_strictness_policy_to_str{
    {RoadGeometryConfiguration::StandardStrictnessPolicy::kStrict, "strict"},
    {RoadGeometryConfiguration::StandardStrictnessPolicy::kAllowSchemaErrors, "allow_schema_errors"},
    {RoadGeometryConfiguration::StandardStrictnessPolicy::kAllowSemanticErrors, "allow_semantic_errors"},
    {RoadGeometryConfiguration::StandardStrictnessPolicy::kPermissive, "permissive"}};

// Returns a vector of strings as a result of splitting @p text by @p character.
// Example:
//    If `text` is "first|second|three" and the `token` is '|' then
//    the return value is the collection: {"first", "second", "three"}"
std::vector<std::string> SplitTextBy(const std::string& text, char token) {
  std::istringstream ss{text};
  std::vector<std::string> strings;
  std::string split_text;
  while (std::getline(ss, split_text, token)) {
    strings.push_back(split_text);
  }
  return strings;
}

// Parses @p bool_str to obtain a boolean type.
//
// @returns True when `bool_str` is "true", "True", "TRUE", "on", "On" or "ON".
// @returns False when `bool_str` is "false", "False", "FALSE", "off", "Off" or "OFF".
//
// @throws maliput::common::assertion_error When `bool_str` doesn't match any of the aforementioned strings.
bool ParseBoolean(const std::string& bool_str) {
  if (bool_str == "true" || bool_str == "True" || bool_str == "TRUE" || bool_str == "on" || bool_str == "On" ||
      bool_str == "ON") {
    return true;
  }
  if (bool_str == "false" || bool_str == "False" || bool_str == "FALSE" || bool_str == "off" || bool_str == "Off" ||
      bool_str == "OFF") {
    return false;
  }
  MALIPUT_THROW_MESSAGE(bool_str + " is not a valid boolean type.");
}

}  // namespace

RoadGeometryConfiguration RoadGeometryConfiguration::FromMap(
    const std::map<std::string, std::string>& road_geometry_configuration) {
  RoadGeometryConfiguration rg_config{};
  auto it = road_geometry_configuration.find(kStrRoadGeometryId);
  if (it != road_geometry_configuration.end()) {
    rg_config.id = maliput::api::RoadGeometryId{it->second};
  }

  it = road_geometry_configuration.find(kStrOpendriveFile);
  if (it != road_geometry_configuration.end()) {
    rg_config.opendrive_file = it->second;
  }

  it = road_geometry_configuration.find(kStrLinearTolerance);
  if (it != road_geometry_configuration.end()) {
    rg_config.tolerances.linear_tolerance = std::stod(it->second);
  }

  it = road_geometry_configuration.find(kStrMaxLinearTolerance);
  if (it != road_geometry_configuration.end()) {
    rg_config.tolerances.max_linear_tolerance = std::stod(it->second);
  }

  it = road_geometry_configuration.find(kStrAngularTolerance);
  if (it != road_geometry_configuration.end()) {
    rg_config.tolerances.angular_tolerance = std::stod(it->second);
  }

  it = road_geometry_configuration.find(kStrScaleLength);
  if (it != road_geometry_configuration.end()) {
    rg_config.scale_length = std::stod(it->second);
  }

  it = road_geometry_configuration.find(kStrInertialToBackendFrameTranslation);
  if (it != road_geometry_configuration.end()) {
    rg_config.inertial_to_backend_frame_translation = maliput::math::Vector3::FromStr(it->second);
  }

  it = road_geometry_configuration.find(kStrBuildPolicy);
  if (it != road_geometry_configuration.end()) {
    const BuildPolicy::Type build_policy_type = BuildPolicy::FromStrToType(it->second);
    it = road_geometry_configuration.find(kStrNumThreads);
    const std::optional<int> num_threads{
        it != road_geometry_configuration.end() ? std::make_optional(std::stoi(it->second)) : std::nullopt};
    rg_config.build_policy = BuildPolicy{build_policy_type, num_threads};
  }

  it = road_geometry_configuration.find(kStrSimplificationPolicy);
  if (it != road_geometry_configuration.end()) {
    rg_config.simplification_policy = FromStrToSimplificationPolicy(it->second);
  }

  it = road_geometry_configuration.find(kStrStandardStrictnessPolicy);
  if (it != road_geometry_configuration.end()) {
    rg_config.standard_strictness_policy = FromStrToStandardStrictnessPolicy(it->second);
  }

  it = road_geometry_configuration.find(kStrOmitNonDrivableLanes);
  if (it != road_geometry_configuration.end()) {
    rg_config.omit_nondrivable_lanes = ParseBoolean(it->second);
  }
  return rg_config;
}

std::map<std::string, std::string> RoadGeometryConfiguration::ToStringMap() const {
  std::map<std::string, std::string> config_map{};
  config_map.emplace(kStrRoadGeometryId, id.string());
  config_map.emplace(kStrOpendriveFile, opendrive_file);
  if (tolerances.linear_tolerance.has_value()) {
    config_map.emplace(kStrLinearTolerance, std::to_string(tolerances.linear_tolerance.value()));
  }
  if (tolerances.max_linear_tolerance.has_value()) {
    config_map.emplace(kStrMaxLinearTolerance, std::to_string(tolerances.max_linear_tolerance.value()));
  }
  config_map.emplace(kStrAngularTolerance, std::to_string(tolerances.angular_tolerance));
  config_map.emplace(kStrScaleLength, std::to_string(scale_length));
  config_map.emplace(kStrInertialToBackendFrameTranslation, inertial_to_backend_frame_translation.to_str());
  config_map.emplace(kStrSimplificationPolicy, FromSimplificationPolicyToStr(simplification_policy));
  config_map.emplace(kStrStandardStrictnessPolicy, FromStandardStrictnessPolicyToStr(standard_strictness_policy));
  config_map.emplace(kStrOmitNonDrivableLanes, omit_nondrivable_lanes ? "true" : "false");
  config_map.emplace(kStrBuildPolicy, BuildPolicy::FromTypeToStr(build_policy.type));
  if (build_policy.num_threads.has_value()) {
    config_map.emplace(kStrNumThreads, std::to_string(build_policy.num_threads.value()));
  }
  return config_map;
}

BuildPolicy::Type BuildPolicy::FromStrToType(const std::string& type) {
  const auto it = str_to_build_policy_type.find(type);
  if (it == str_to_build_policy_type.end()) {
    MALIDRIVE_THROW_MESSAGE("Unknown build policy type: " + type);
  }
  return str_to_build_policy_type.at(type);
}

std::string BuildPolicy::FromTypeToStr(const BuildPolicy::Type& type) { return build_policy_type_to_str.at(type); }

RoadGeometryConfiguration::SimplificationPolicy RoadGeometryConfiguration::FromStrToSimplificationPolicy(
    const std::string& policy) {
  const auto it = str_to_simplification_policy.find(policy);
  if (it == str_to_simplification_policy.end()) {
    MALIDRIVE_THROW_MESSAGE("Unknown simplification policy: " + policy);
  }
  return str_to_simplification_policy.at(policy);
}

std::string RoadGeometryConfiguration::FromSimplificationPolicyToStr(
    const RoadGeometryConfiguration::SimplificationPolicy& policy) {
  return simplification_policy_to_str.at(policy);
}

RoadGeometryConfiguration::StandardStrictnessPolicy RoadGeometryConfiguration::FromStrToStandardStrictnessPolicy(
    const std::string& policy) {
  const char token = '|';
  const auto keys = SplitTextBy(policy, token);
  RoadGeometryConfiguration::StandardStrictnessPolicy strictness{
      RoadGeometryConfiguration::StandardStrictnessPolicy::kStrict};
  for (const auto& key : keys) {
    const auto it = str_to_standard_strictness_policy.find(key);
    if (it == str_to_standard_strictness_policy.end()) {
      MALIDRIVE_THROW_MESSAGE("Unknown standard strictness policy: " + key);
    }
    strictness = strictness | str_to_standard_strictness_policy.at(key);
  }
  return strictness;
}

std::string RoadGeometryConfiguration::FromStandardStrictnessPolicyToStr(
    const RoadGeometryConfiguration::StandardStrictnessPolicy& policy) {
  switch (policy) {
    case RoadGeometryConfiguration::StandardStrictnessPolicy::kStrict:
      return standard_strictness_policy_to_str.at(RoadGeometryConfiguration::StandardStrictnessPolicy::kStrict);
      break;
    case RoadGeometryConfiguration::StandardStrictnessPolicy::kPermissive:
      return standard_strictness_policy_to_str.at(RoadGeometryConfiguration::StandardStrictnessPolicy::kPermissive);
      break;
    case RoadGeometryConfiguration::StandardStrictnessPolicy::kAllowSchemaErrors:
      return standard_strictness_policy_to_str.at(
          RoadGeometryConfiguration::StandardStrictnessPolicy::kAllowSchemaErrors);
      break;
    case RoadGeometryConfiguration::StandardStrictnessPolicy::kAllowSemanticErrors:
      return standard_strictness_policy_to_str.at(
          RoadGeometryConfiguration::StandardStrictnessPolicy::kAllowSemanticErrors);
      break;
    default:
      MALIPUT_THROW_MESSAGE("Unknown standard strictness policy.");
      break;
  }
}

RoadGeometryConfiguration::BuildTolerance::BuildTolerance(double angular_tolerance_in)
    : angular_tolerance(angular_tolerance_in) {
  // Verification of values is made downstream at the RoadGeometryBuilder entity.
}

RoadGeometryConfiguration::BuildTolerance::BuildTolerance(double linear_tolerance_in, double angular_tolerance_in)
    : linear_tolerance(linear_tolerance_in), angular_tolerance(angular_tolerance_in) {
  // Verification of values is made downstream at the RoadGeometryBuilder entity.
}

RoadGeometryConfiguration::BuildTolerance::BuildTolerance(double min_linear_tolerance_in,
                                                          double max_linear_tolerance_in, double angular_tolerance_in)
    : BuildTolerance(min_linear_tolerance_in, angular_tolerance_in) {
  max_linear_tolerance = max_linear_tolerance_in;
  // Verification of values is made downstream at the RoadGeometryBuilder entity.
}

RoadGeometryConfiguration::StandardStrictnessPolicy operator|(
    const RoadGeometryConfiguration::StandardStrictnessPolicy& first,
    const RoadGeometryConfiguration::StandardStrictnessPolicy& second) {
  return RoadGeometryConfiguration::StandardStrictnessPolicy(static_cast<unsigned int>(first) |
                                                             static_cast<unsigned int>(second));
}

RoadGeometryConfiguration::StandardStrictnessPolicy operator&(
    const RoadGeometryConfiguration::StandardStrictnessPolicy& first,
    const RoadGeometryConfiguration::StandardStrictnessPolicy& second) {
  return RoadGeometryConfiguration::StandardStrictnessPolicy(static_cast<unsigned int>(first) &
                                                             static_cast<unsigned int>(second));
}

}  // namespace builder
}  // namespace malidrive
