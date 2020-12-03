// Copyright 2020 Toyota Research Institute
#include "maliput_malidrive/xodr/geometry.h"

namespace malidrive {
namespace xodr {
namespace {

// Map for Type to string conversion.
const std::map<Geometry::Type, std::string> type_to_str_map{{Geometry::Type::kLine, "line"},
                                                            {Geometry::Type::kArc, "arc"}};

// Map for string to Type conversion.
const std::map<std::string, Geometry::Type> str_to_type_map{{"line", Geometry::Type::kLine},
                                                            {"arc", Geometry::Type::kArc}};

}  // namespace

std::string Geometry::type_to_str(Geometry::Type type) { return type_to_str_map.at(type); }

Geometry::Type Geometry::str_to_type(const std::string& type) {
  if (str_to_type_map.find(type) == str_to_type_map.end()) {
    MALIDRIVE_THROW_MESSAGE(type + " geometry type is not available.");
  }
  return str_to_type_map.at(type);
}

bool Geometry::operator==(const Geometry& other) const {
  return s_0 == other.s_0 && start_point == other.start_point && orientation == other.orientation &&
         length == other.length && type == other.type && description == other.description;
}

bool Geometry::operator!=(const Geometry& other) const { return !(*this == other); }

}  // namespace xodr
}  // namespace malidrive
