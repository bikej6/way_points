#include <QApplication>
#include <QDebug>
#include <WaypointRoute.h>
#include "WaypointApplication.h"
#include "WaypointDatabase.h"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  WaypointDatabase database;
  WaypointRoute routes(database);

  WaypointApplication main_window(routes);
  main_window.show();

  return a.exec();
}
