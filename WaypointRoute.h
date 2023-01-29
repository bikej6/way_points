#ifndef WAYPOINTROUTE_H
#define WAYPOINTROUTE_H

#include <QStringList>
#include <WaypointDatabase.h>

class WaypointRoute
{
public:
  WaypointRoute() = default;
  explicit WaypointRoute(const WaypointDatabase& database)
    : m_database(database)
  {}

  QStringList route(int id) const;
  void addRoute(const QStringList& points);

private:
  WaypointDatabase m_database;
};

#endif // WAYPOINTROUTE_H
