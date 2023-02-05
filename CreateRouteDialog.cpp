#include "CreateRouteDialog.h"

#include <QDebug>
#include <QMessageBox>
#include "WaypointListItem.h"
#include "WaypointRoute.h"
#include "ui_CreateRouteDialog.h"

class CreateRouteDialog::Private: public Ui::CreateRouteDialogBase
{
public:
  explicit Private(const WaypointRoute& database, CreateRouteDialog* parent);
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
  void toggleSaveButton() const;
  void clearLayout();

  CreateRouteDialog* m_parent;
  WaypointRoute m_database;

  static constexpr auto LAYOUT_MIN_ELEMENTS = 1;
  static constexpr auto MIN_ELEMENTS_TO_SAVE = 2;
  static constexpr auto MIN_ELEMENTS_TO_REMOVE = 3;
};

CreateRouteDialog::Private::Private(const WaypointRoute& database, CreateRouteDialog* parent)
  : m_parent(parent)
  , m_database(database)
{
  setupUi(parent);

  connect(ui_add_waypoint_button, &QPushButton::clicked, m_parent, &CreateRouteDialog::addWaypoint);
  connect(ui_edit, &QPushButton::clicked, [this]() { initializeWaypoints(m_database.route(0)); });
  connect(ui_cancel_button, &QPushButton::clicked, m_parent, &CreateRouteDialog::reject);
  connect(ui_save_button, &QPushButton::clicked, m_parent, &CreateRouteDialog::accept);

  const auto start_point = dynamic_cast<WaypointListItem*>(ui_waypoint_items_list_layout->itemAt(0)->widget());
  const auto end_point = dynamic_cast<WaypointListItem*>(ui_waypoint_items_list_layout->itemAt(1)->widget());

  connect(start_point, &WaypointListItem::emptinessChanged, [this]() {
    toggleAddWaypointButton();
    toggleSaveButton();
  });
  connect(start_point, &WaypointListItem::deleteItem, m_parent, &CreateRouteDialog::removeWaypoint);

  connect(end_point, &WaypointListItem::emptinessChanged, [this]() {
    toggleAddWaypointButton();
    toggleSaveButton();
  });
  connect(end_point, &WaypointListItem::deleteItem, m_parent, &CreateRouteDialog::removeWaypoint);
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

  connect(item, &WaypointListItem::emptinessChanged, [this]() {
    toggleAddWaypointButton();
    toggleSaveButton();
  });
  connect(item, &WaypointListItem::deleteItem, m_parent, &CreateRouteDialog::removeWaypoint);

  setRemovable();
  toggleAddWaypointButton();
  toggleSaveButton();
}

void CreateRouteDialog::Private::removeWaypoint(WaypointListItem* item)
{
  const auto pos = itemPosition(item);
  const auto layout_item = ui_waypoint_items_list_layout->takeAt(pos);

  delete layout_item->widget();
  delete layout_item;

  setRemovable();
  toggleAddWaypointButton();
  toggleSaveButton();
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

void CreateRouteDialog::Private::toggleSaveButton() const
{
  auto non_empty = 0;

  for (const auto item : ui_waypoint_items_list->findChildren<WaypointListItem*>())
  {
    if (!item->isEmpty())
    {
      ++non_empty;
    }
  }

  if (non_empty >= MIN_ELEMENTS_TO_SAVE)
  {
    ui_save_button->setEnabled(true);
    ui_save_button->setToolTip("");
  }
  else
  {
    ui_save_button->setDisabled(true);
    ui_save_button->setToolTip("Cannot save route with less than 2 elements");
  }
}

CreateRouteDialog::CreateRouteDialog(const WaypointRoute& database, QWidget* parent)
  : QDialog(parent)
  , m_ptr(new Private(database, this))
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

QStringList CreateRouteDialog::waypointsList()
{
  return m_ptr->waypointsNames();
}
