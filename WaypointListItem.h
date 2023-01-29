#ifndef WAYPOINTLISTITEM_H
#define WAYPOINTLISTITEM_H

#include <QWidget>

class WaypointListItem: public QWidget
{
  Q_OBJECT

public:
  explicit WaypointListItem(QWidget* parent = nullptr);
  explicit WaypointListItem(const QString& init, QWidget* parent = nullptr);
  ~WaypointListItem() override;

  void setRemovable(bool removable) const;
  bool isEmpty() const;
  QString pointName() const;

signals:
  void emptinessChanged();
  void deleteItem();
  void selectItem();

private:
  class Private;
  std::unique_ptr<Private> m_ptr;
};

#endif // WAYPOINTLISTITEM_H
