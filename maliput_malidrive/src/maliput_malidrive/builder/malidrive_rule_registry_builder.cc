// Copyright 2020 Toyota Research Institute
#include "maliput_malidrive/builder/malidrive_rule_registry_builder.h"

#include "maliput_malidrive/builder/malidrive_builder_tools.h"
#include "maliput_malidrive/constants.h"

namespace malidrive {
namespace builder {

using maliput::api::rules::DiscreteValueRule;
using maliput::api::rules::RangeValueRule;
using maliput::api::rules::Rule;

std::unique_ptr<maliput::api::rules::RuleRegistry> MalidriveRuleRegistryBuilder::operator()() {
  auto rule_registry = std::make_unique<maliput::api::rules::RuleRegistry>();
  AddDiscreteValueRuleTypes(rule_registry.get());
  AddSpeedLimitRuleType(rule_registry.get());

  return rule_registry;
}

void MalidriveRuleRegistryBuilder::AddDiscreteValueRuleTypes(maliput::api::rules::RuleRegistry* rule_registry) const {
  MALIDRIVE_THROW_UNLESS(rule_registry != nullptr);

  const Rule::RelatedRules empty_related_rules{};
  const Rule::RelatedUniqueIds empty_related_unique_ids{};
  const int severity{Rule::State::kStrict};

  for (const auto& rule_type_values : RuleTypesAndValues()) {
    std::vector<DiscreteValueRule::DiscreteValue> discrete_values;
    for (const std::string& discrete_value : rule_type_values.second) {
      discrete_values.push_back(
          DiscreteValueRule::DiscreteValue{severity, empty_related_rules, empty_related_unique_ids, discrete_value});
    }
    rule_registry->RegisterDiscreteValueRule(rule_type_values.first, discrete_values);
  }
  const auto& direction_usage_rule_type = maliput::BuildDirectionUsageRuleType();
  rule_registry->RegisterDiscreteValueRule(direction_usage_rule_type.first, direction_usage_rule_type.second);
}

std::unordered_map<DiscreteValueRule::TypeId, std::vector<std::string>>
MalidriveRuleRegistryBuilder::RuleTypesAndValues() const {
  return {
      // Defines whether or not vehicles and pedestrians can travel along a route.
      {rules::VehicleUsageRuleTypeId(),
       {
           "NonVehicles",     // Vehicles are not allowed to travel.
           "NonPedestrians",  // Pedestrian are not allowed to travel. When this value is set, it is
                              // usually paired with `VehicleExclusiveRuleType` to better specify which type
                              // of vehicle is allowed.
           "Unrestricted",    // No restriction for vehicles nor pedestrians to travel simultaneously.
                              // When this value is set, it is usually paired with `VehicleExclusiveRuleType`
                              // to better specify which type of vehicle is allowed.
       }},
      // Defines which type of vehicle can exclusively travel along a route.
      {rules::VehicleExclusiveRuleTypeId(),
       {
           "BusOnly",                   // Exclusive route for buses.
           "EmergencyVehiclesOnly",     // Exclusive route for emergency vehicles.
           "HighOccupancyVehicleOnly",  // Exclusive route for high occupancy vehicles, such as vehicles with at
                                        // least one passenger.
           "MotorizedVehicleOnly",      // Exclusive route for motorized vehicles, such as cars and motorbikes.
           "NonMotorizedVehicleOnly",   // Exclusive route for non motorized vehicles, such as bikes.
       }},
  };
}

void MalidriveRuleRegistryBuilder::AddSpeedLimitRuleType(maliput::api::rules::RuleRegistry* rule_registry) const {
  MALIDRIVE_THROW_UNLESS(rule_registry != nullptr);

  const Rule::RelatedRules empty_related_rules{};
  const Rule::RelatedUniqueIds empty_related_unique_ids{};
  std::set<RangeValueRule::Range> speed_limit_range_set;
  // Finds the maximum speed limits in the RoadGeometry. Then registers with
  // the RuleRegistry a maliput::SpeedLimitRuleTypeId() RangeValueRule
  // containing the default minimum speed limit and the discovered maximum
  // speed limit.
  for (const auto lane_id_lane : rg_->ById().GetLanes()) {
    const auto max_speed_limits = GetMaxSpeedLimitFor(dynamic_cast<const MalidriveLane*>(lane_id_lane.second));
    for (const auto& speed_limit : max_speed_limits) {
      speed_limit_range_set.insert(RangeValueRule::Range{Rule::State::kStrict, empty_related_rules,
                                                         empty_related_unique_ids, "m/s",
                                                         constants::kDefaultMinSpeedLimit, speed_limit.max});
    }
  }
  rule_registry->RegisterRangeValueRule(
      maliput::SpeedLimitRuleTypeId(),
      std::vector<RangeValueRule::Range>(speed_limit_range_set.begin(), speed_limit_range_set.end()));
}

}  // namespace builder
}  // namespace malidrive
