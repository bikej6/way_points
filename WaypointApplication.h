#ifndef WAYPOINTAPPLICATION_H
#define WAYPOINTAPPLICATION_H

#include <QMainWindow>
#include "WaypointRoute.h"

class WaypointApplication: public QMainWindow
{
  Q_OBJECT

public:
  explicit WaypointApplication(QWidget* parent = nullptr);
  explicit WaypointApplication(WaypointRoute* route, QWidget* parent = nullptr);

  ~WaypointApplication();

private:
  class Private;
  std::unique_ptr<Private> m_ptr;
};

#endif // WAYPOINTAPPLICATION_H
