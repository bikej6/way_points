#include "WaypointApplication.h"
#include <CreateRouteDialog.h>
#include <QDebug>
#include "ui_WaypointApplication.h"

class WaypointApplication::Private: public Ui::WaypointAppBase
{
public:
  explicit Private(WaypointApplication* parent = nullptr);
  explicit Private(WaypointRoute* route, WaypointApplication* parent = nullptr);

  void createDialog(const QStringList& init = QStringList()) const;
  ~Private() = default;

private:
  WaypointRoute* m_routes;
  WaypointApplication* m_parent;
};

WaypointApplication::Private::Private(WaypointApplication* parent)
  : m_parent(parent)
{
  setupUi(parent);
  connect(ui_create_route_button, &QPushButton::clicked, [this]() { createDialog(); });
}

WaypointApplication::Private::Private(WaypointRoute* route, WaypointApplication* parent)
  : WaypointApplication::Private(parent)
{
  m_routes = route;

  connect(ui_load_1_button, &QPushButton::clicked, [this]() { createDialog(m_routes->route(0)); });
  connect(ui_load_2_button, &QPushButton::clicked, [this]() { createDialog(m_routes->route(1)); });
}

void WaypointApplication::Private::createDialog(const QStringList& init) const
{
  CreateRouteDialog dialog(m_parent, init);
  connect(&dialog, &CreateRouteDialog::saveRoute, [this](const auto& route) { m_routes->addRoute(route); });
  dialog.exec();
}

WaypointApplication::WaypointApplication(QWidget* parent)
  : WaypointApplication(nullptr, parent)
{}

WaypointApplication::WaypointApplication(WaypointRoute* route, QWidget* parent)
  : QMainWindow(parent)
  , m_ptr(new Private(route, this))
{
}

WaypointApplication::~WaypointApplication() = default;
