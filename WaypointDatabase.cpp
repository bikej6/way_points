#include "WaypointDatabase.h"
#include <QDebug>
#include <algorithm>

bool WaypointDatabase::exists(int id) const
{
  return std::find_if(std::begin(m_database), std::end(m_database), [&id](const auto entry) { return id == entry.id; })
         != std::end(m_database);
}

RouteType WaypointDatabase::retrieveRoute(int id) const
{
  if (!exists(id))
  {
    return std::nullopt;
  }

  const auto route =
      std::find_if(std::begin(m_database), std::end(m_database), [&id](const auto entry) { return id == entry.id; });

  return (*route).points;
}

void WaypointDatabase::addRoute(int id, const Points& route)
{
  if (id != 0)
  {
    throw std::invalid_argument("Invalid id passed");
  }

  auto data = Data();
  data.id = id;
  data.points = route;

  m_database[id] = data;
}
