#include <QApplication>
#include <QDebug>
#include <WaypointRoute.h>
#include "WaypointApplication.h"
#include "WaypointDatabase.h"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  WaypointDatabase database;
  auto routes = std::make_unique<WaypointRoute>(database);

  WaypointApplication main_window(routes.get());
  main_window.show();

  return a.exec();
}
