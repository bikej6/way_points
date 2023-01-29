#ifndef CREATEROUTEDIALOG_H
#define CREATEROUTEDIALOG_H

#include <QDialog>

class CreateRouteDialog: public QDialog
{
  Q_OBJECT

public:
  explicit CreateRouteDialog(QWidget* parent = nullptr, const QStringList& points = QStringList());
  ~CreateRouteDialog() override;

signals:
  void saveRoute(const QStringList& points);

private slots:
  void accept() override;

  void addWaypoint() const;
  void removeWaypoint() const;

private:
  class Private;
  std::unique_ptr<Private> m_ptr;
};

#endif // CREATEROUTEDIALOG_H
