#include "WaypointRoute.h"
#include <QDebug>
#include <iostream>

QStringList WaypointRoute::route(int id) const
{
  const auto route = m_database.retrieveRoute(id);

  if (!route)
  {
    throw std::invalid_argument("No such route for given id");
  }

  QStringList points;
  points.reserve(route->size());

  for (const auto& s : *route)
  {
    points << QString::fromStdString(s);
  }

  return points;
}

void WaypointRoute::addRoute(const QStringList& points)
{
  Points data;

  for (const auto& point : points)
  {
    data.emplace_back(point.toStdString());
  }

  m_database.addRoute(0, data);
}
