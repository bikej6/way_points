#ifndef WAYPOINTAPPLICATION_H
#define WAYPOINTAPPLICATION_H

#include <QMainWindow>

class WaypointRoute;

class WaypointApplication: public QMainWindow
{
  Q_OBJECT

public:
  explicit WaypointApplication(const WaypointRoute& database, QWidget* parent = nullptr);

  ~WaypointApplication();

private:
  class Private;
  std::unique_ptr<Private> m_ptr;
};

#endif // WAYPOINTAPPLICATION_H
