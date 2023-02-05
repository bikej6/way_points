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
  void addRoute(int id, const Points& route);

private:
  std::vector<Data> m_database = { { 0, { "Point1", "Point2", "Point3", "Point4", "Point5", "Point6" } } };
};

#endif // WAYPOINTAPP_H
