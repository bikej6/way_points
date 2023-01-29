#include "CreateRouteDialog.h"

#include <QDebug>
#include <QMessageBox>

#include "WaypointListItem.h"
#include "ui_CreateRouteDialog.h"

class CreateRouteDialog::Private: public Ui::CreateRouteDialogBase
{
public:
  explicit Private(CreateRouteDialog* parent, const QStringList& points);
  ~Private() = default;

  void addWaypoint();
  void removeWaypoint(WaypointListItem* item);
  QStringList waypointsNames();

private:
  void initializeWaypoints(const QStringList& points);
  void addWaypoint(const QString& init);
  void insertConnectItem(WaypointListItem* item);
  int itemPosition(WaypointListItem* item) const;
  void setRemovable() const;
  void toggleAddWaypointButton() const;
  void clearLayout();
  void outputPoints();

  CreateRouteDialog* m_parent;

  static constexpr auto LAYOUT_MIN_ELEMENTS = 1;
  static constexpr auto MIN_ELEMENTS_TO_REMOVE = 3;
};

CreateRouteDialog::Private::Private(CreateRouteDialog* parent, const QStringList& points)
  : m_parent(parent)
{
  setupUi(parent);

  connect(ui_add_waypoint_button, &QPushButton::clicked, m_parent, &CreateRouteDialog::addWaypoint);
  connect(ui_output, &QPushButton::clicked, [this]() { outputPoints(); });
  connect(ui_button_box, &QDialogButtonBox::rejected, m_parent, &CreateRouteDialog::reject);
  connect(ui_button_box, &QDialogButtonBox::accepted, m_parent, &CreateRouteDialog::accept);

  if (points.isEmpty())
  {
    const auto start_point = dynamic_cast<WaypointListItem*>(ui_waypoint_items_list_layout->itemAt(0)->widget());
    const auto end_point = dynamic_cast<WaypointListItem*>(ui_waypoint_items_list_layout->itemAt(1)->widget());

    connect(start_point, &WaypointListItem::emptinessChanged, [this]() { toggleAddWaypointButton(); });
    connect(start_point, &WaypointListItem::deleteItem, m_parent, &CreateRouteDialog::removeWaypoint);

    connect(end_point, &WaypointListItem::emptinessChanged, [this]() { toggleAddWaypointButton(); });
    connect(end_point, &WaypointListItem::deleteItem, m_parent, &CreateRouteDialog::removeWaypoint);
  }
  else
  {
    initializeWaypoints(points);
  }
}

void CreateRouteDialog::Private::initializeWaypoints(const QStringList& points)
{
  clearLayout();

  for (const auto& point : points)
  {
    if (!point.isEmpty())
    {
      addWaypoint(point);
    }
  }
}

void CreateRouteDialog::Private::addWaypoint()
{
  const auto item = new WaypointListItem;
  insertConnectItem(item);
}

void CreateRouteDialog::Private::addWaypoint(const QString& init)
{
  const auto item = new WaypointListItem(init);
  insertConnectItem(item);
}

void CreateRouteDialog::Private::insertConnectItem(WaypointListItem* item)
{
  const auto position = ui_waypoint_items_list_layout->count() - LAYOUT_MIN_ELEMENTS;

  ui_waypoint_items_list_layout->insertWidget(position, item);

  connect(item, &WaypointListItem::emptinessChanged, [this]() { toggleAddWaypointButton(); });
  connect(item, &WaypointListItem::deleteItem, m_parent, &CreateRouteDialog::removeWaypoint);

  setRemovable();
  toggleAddWaypointButton();
}

void CreateRouteDialog::Private::removeWaypoint(WaypointListItem* item)
{
  const auto pos = itemPosition(item);
  const auto layout_item = ui_waypoint_items_list_layout->takeAt(pos);

  delete layout_item->widget();
  delete layout_item;

  setRemovable();
  toggleAddWaypointButton();
}

QStringList CreateRouteDialog::Private::waypointsNames()
{
  QStringList output;

  for (const auto item : ui_waypoint_items_list->findChildren<WaypointListItem*>())
  {
    if (const auto& name = item->pointName(); !name.isEmpty())
    {
      output << name;
    }
  }

  return output;
}

void CreateRouteDialog::Private::clearLayout()
{
  while (ui_waypoint_items_list_layout->count() > LAYOUT_MIN_ELEMENTS)
  {
    const auto child = ui_waypoint_items_list_layout->takeAt(0);
    delete child->widget();
    delete child;
  }
}

void CreateRouteDialog::Private::outputPoints()
{
  QString message;

  if (const auto& names = waypointsNames(); names.isEmpty())
  {
    message = "No points were added to route";
  }
  else
  {
    for (const auto& name : names)
    {
      message += name + "\n";
    }
  }

  QMessageBox::information(m_parent, "Route points", message, QMessageBox::Ok);
}

void CreateRouteDialog::Private::setRemovable() const
{
  const auto waypoints_nr = ui_waypoint_items_list_layout->count() - LAYOUT_MIN_ELEMENTS;

  const auto set_removable = [this, waypoints_nr](const auto state) {
    auto id = 0;
    while (id < waypoints_nr)
    {
      const auto point = dynamic_cast<WaypointListItem*>(ui_waypoint_items_list_layout->itemAt(id)->widget());
      point->setRemovable(state);
      ++id;
    }
  };

  if (waypoints_nr >= MIN_ELEMENTS_TO_REMOVE)
  {
    set_removable(true);
  }
  else
  {
    set_removable(false);
  }
}

int CreateRouteDialog::Private::itemPosition(WaypointListItem* item) const
{
  for (auto i = 0; i < ui_waypoint_items_list_layout->count(); i++)
  {
    if (const auto widget = ui_waypoint_items_list_layout->itemAt(i)->widget(); widget == item)
    {
      return i;
    }
  }

  throw std::invalid_argument("Cannot find item in layout");
}

void CreateRouteDialog::Private::toggleAddWaypointButton() const
{
  for (const auto item : ui_waypoint_items_list->findChildren<WaypointListItem*>())
  {
    if (item->isEmpty())
    {
      ui_add_waypoint_button->setDisabled(true);
      return;
    }
  }

  ui_add_waypoint_button->setEnabled(true);
}

CreateRouteDialog::CreateRouteDialog(QWidget* parent, const QStringList& points)
  : QDialog(parent)
  , m_ptr(new Private(this, points))
{
  setFixedSize(size());
};

CreateRouteDialog::~CreateRouteDialog() = default;

void CreateRouteDialog::addWaypoint() const
{
  m_ptr->addWaypoint();
}

void CreateRouteDialog::removeWaypoint() const
{
  const auto item = dynamic_cast<WaypointListItem*>(sender());
  m_ptr->removeWaypoint(item);
}

void CreateRouteDialog::accept()
{
  if (const auto& waypoints = m_ptr->waypointsNames(); !waypoints.isEmpty())
  {
    emit saveRoute(waypoints);
  }

  QDialog::accept();
}
