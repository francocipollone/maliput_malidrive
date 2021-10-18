// Copyright 2020 Toyota Research Institute
#include "maliput_malidrive/builder/road_network_builder.h"

#include <map>
#include <optional>
#include <utility>
#include <vector>

#include <maliput/base/intersection_book.h>
#include <maliput/base/intersection_book_loader.h>
#include <maliput/base/manual_discrete_value_rule_state_provider.h>
#include <maliput/base/manual_phase_provider.h>
#include <maliput/base/manual_phase_ring_book.h>
#include <maliput/base/manual_range_value_rule_state_provider.h>
#include <maliput/base/manual_rulebook.h>
#include <maliput/base/phase_based_right_of_way_rule_state_provider.h>
#include <maliput/base/phase_ring_book_loader.h>
#include <maliput/base/rule_registry.h>
#include <maliput/base/traffic_light_book.h>
#include <maliput/base/traffic_light_book_loader.h>
#include <maliput/common/logger.h>
#include <maliput/common/maliput_unused.h>

#include "maliput_malidrive/builder/builder_tools.h"
#include "maliput_malidrive/builder/direction_usage_builder.h"
#include "maliput_malidrive/builder/discrete_value_rule_state_provider_builder.h"
#include "maliput_malidrive/builder/range_value_rule_state_provider_builder.h"
#include "maliput_malidrive/builder/road_geometry_builder.h"
#include "maliput_malidrive/builder/road_network_configuration.h"
#include "maliput_malidrive/builder/road_rulebook_builder.h"
#include "maliput_malidrive/builder/rule_registry_builder.h"
#include "maliput_malidrive/builder/speed_limit_builder.h"
#include "maliput_malidrive/builder/xodr_parser_configuration.h"
#include "maliput_malidrive/common/macros.h"
#include "maliput_malidrive/constants.h"
#include "maliput_malidrive/xodr/parser_configuration.h"
#include "maliput_malidrive/xodr/unit.h"

namespace malidrive {
namespace builder {

std::unique_ptr<maliput::api::RoadNetwork> RoadNetworkBuilder::operator()() const {
  const auto rn_config{RoadNetworkConfiguration::FromMap(road_network_configuration_)};
  const auto& rg_config = rn_config.road_geometry_configuration;
  MALIDRIVE_VALIDATE(!rg_config.opendrive_file.empty(), std::runtime_error, "opendrive_file cannot be empty");

  const xodr::ParserConfiguration parser_config = XodrParserConfigurationFromRoadGeometryConfiguration(rg_config);
  maliput::log()->trace("Loading database from file: {} ...", rg_config.opendrive_file);
  auto db_manager = xodr::LoadDataBaseFromFile(rg_config.opendrive_file, parser_config);
  maliput::log()->trace("Building RoadGeometry...");
  std::unique_ptr<const maliput::api::RoadGeometry> rg =
      builder::RoadGeometryBuilder(std::move(db_manager), rg_config)();

  auto direction_usages = DirectionUsageBuilder(rg.get())();
  maliput::common::unused(direction_usages);
  auto speed_limits = SpeedLimitBuilder(rg.get())();
  maliput::common::unused(speed_limits);

  maliput::log()->trace("Building RuleRegistry...");
  auto rule_registry = RuleRegistryBuilder(rg.get())();
  maliput::log()->trace("Built RuleRegistry...");

  maliput::log()->trace("Building RuleRoadBook...");
  auto rule_book =
      RoadRuleBookBuilder(rg.get(), rule_registry.get(), rn_config.road_rule_book, direction_usages, speed_limits)();
  maliput::log()->trace("Built RuleRoadBook.");

  maliput::log()->trace("Building TrafficLightBook...");
  auto traffic_light_book = !rn_config.traffic_light_book.has_value()
                                ? std::make_unique<maliput::TrafficLightBook>()
                                : maliput::LoadTrafficLightBookFromFile(rn_config.traffic_light_book.value());
  maliput::log()->trace("Built TrafficLightBook.");

  maliput::log()->trace("Building PhaseRingBook...");
  auto phase_ring_book = !rn_config.phase_ring_book.has_value()
                             ? std::make_unique<maliput::ManualPhaseRingBook>()
                             : maliput::LoadPhaseRingBookFromFile(rule_book.get(), traffic_light_book.get(),
                                                                  rn_config.phase_ring_book.value());
  maliput::log()->trace("Built PhaseRingBook.");

  maliput::log()->trace("Building PhaseProvider...");
  auto manual_phase_provider = std::make_unique<maliput::ManualPhaseProvider>();
  maliput::log()->trace("Built PhaseProvider.");

  maliput::log()->trace("Building DiscreteValueRuleStateProvider...");
  auto discrete_value_rule_state_provider =
      DiscreteValueRuleStateProviderBuilder(rule_book.get(), phase_ring_book.get(), manual_phase_provider.get())();
  maliput::log()->trace("Built DiscreteValueRuleStateProvider.");

  maliput::log()->trace("Building RangeValueRuleStateProvider...");
  auto range_value_rule_state_provider = RangeValueRuleStateProviderBuilder(rule_book.get())();
  maliput::log()->trace("Built RangeValueRuleStateProvider.");

  maliput::log()->trace("Building IntersectionBook...");
  auto intersection_book = !rn_config.intersection_book.has_value()
                               ? std::make_unique<maliput::IntersectionBook>()
                               : maliput::LoadIntersectionBookFromFile(rn_config.intersection_book.value(), *rule_book,
                                                                       *phase_ring_book, manual_phase_provider.get());
  maliput::log()->trace("Built IntersectionBook.");

  maliput::log()->trace("Building RuleStateProvider...");
  auto state_provider = std::make_unique<maliput::PhaseBasedRightOfWayRuleStateProvider>(phase_ring_book.get(),
                                                                                         manual_phase_provider.get());
  maliput::log()->trace("Built RuleStateProvider.");

  return std::make_unique<maliput::api::RoadNetwork>(
      std::move(rg), std::move(rule_book), std::move(traffic_light_book), std::move(intersection_book),
      std::move(phase_ring_book), std::move(state_provider), std::move(manual_phase_provider), std::move(rule_registry),
      std::move(discrete_value_rule_state_provider), std::move(range_value_rule_state_provider));
}

}  // namespace builder
}  // namespace malidrive
