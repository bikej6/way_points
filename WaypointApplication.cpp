#include "WaypointApplication.h"
#include "CreateRouteDialog.h"
#include "WaypointRoute.h"
#include "ui_WaypointApplication.h"

class WaypointApplication::Private: public Ui::WaypointAppBase
{
public:
  explicit Private(const WaypointRoute& database, WaypointApplication* parent = nullptr);
  ~Private() = default;

private:
  WaypointApplication* m_parent;
  WaypointRoute m_database;
};

WaypointApplication::Private::Private(const WaypointRoute& database, WaypointApplication* parent)
  : m_parent(parent)
  , m_database(database)
{
  setupUi(parent);

  connect(ui_create_route_button, &QPushButton::clicked, [this]() {
    CreateRouteDialog dialog(m_database, m_parent);

    connect(&dialog, &CreateRouteDialog::accepted, [&] { m_database.addRoute(dialog.waypointsList()); });
    dialog.exec();
  });
}

WaypointApplication::WaypointApplication(const WaypointRoute& database, QWidget* parent)
  : QMainWindow(parent)
  , m_ptr(new Private(database, this))
{}

WaypointApplication::~WaypointApplication() = default;
