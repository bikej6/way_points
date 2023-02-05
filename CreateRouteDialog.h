#ifndef CREATEROUTEDIALOG_H
#define CREATEROUTEDIALOG_H

#include <QDialog>

class WaypointRoute;

class CreateRouteDialog: public QDialog
{
  Q_OBJECT

public:
  explicit CreateRouteDialog(const WaypointRoute& database, QWidget* parent = nullptr);
  ~CreateRouteDialog() override;

  QStringList waypointsList();

private slots:
  void addWaypoint() const;
  void removeWaypoint() const;

private:
  class Private;
  std::unique_ptr<Private> m_ptr;
};

#endif // CREATEROUTEDIALOG_H
