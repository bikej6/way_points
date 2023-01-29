#ifndef WAYPOINTDATABASE_H
#define WAYPOINTDATABASE_H

#include <optional>
#include "Data.h"

using RouteType = std::optional<Points>;

class WaypointDatabase
{
public:
  [[nodiscard]] bool exists(int id) const;
  [[nodiscard]] RouteType retrieveRoute(int id) const;
  [[nodiscard]] size_t routesNumber() const;
  void addRoute(const Data& route);

private:
  std::vector<Data> m_database = { { 0, { "Point1", "Point2", "Point3", "Point4", "Point5", "Point6" } },
                                   { 1, { "Point7", "Point8", "Point9", "Point10", "Point11", "Point12" } } };
};

#endif // WAYPOINTAPP_H
