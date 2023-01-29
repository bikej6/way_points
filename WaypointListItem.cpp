#include "WaypointListItem.h"

#include <QDebug>
#include <QIcon>
#include <QMouseEvent>

#include "ui_WaypointListItem.h"

class WaypointListItem::Private: public Ui::WaypointListItemBase
{
public:
  explicit Private(const QString& init, WaypointListItem* parent);
  ~Private() = default;

  bool isEmpty() const;

private:
  bool m_is_line_empty = true;
  WaypointListItem* m_parent = nullptr;
};

WaypointListItem::Private::Private(const QString& init, WaypointListItem* parent)
  : m_parent(parent)
{
  setupUi(parent);

  ui_remove->setIcon(QIcon(":images/remove.svg"));
  ui_remove->setIconSize(QSize(15, 15));
  ui_remove->hide();

  connect(ui_remove, &QPushButton::clicked, m_parent, &WaypointListItem::deleteItem);

  connect(ui_line_edit, &QLineEdit::textChanged, [this]() {
    if (const auto is_empty = ui_line_edit->text().isEmpty(); is_empty != m_is_line_empty)
    {
      m_is_line_empty = is_empty;
      emit m_parent->emptinessChanged();
    }
  });

  if (!init.isEmpty())
  {
    ui_line_edit->setText(init);
  }
}

bool WaypointListItem::Private::isEmpty() const
{
  return m_is_line_empty;
}

WaypointListItem::WaypointListItem(QWidget* parent)
  : WaypointListItem(QString(), parent)
{}

WaypointListItem::WaypointListItem(const QString& init, QWidget* parent)
  : QWidget(parent)
  , m_ptr(new Private(init, this))
{
}

WaypointListItem::~WaypointListItem() = default;

bool WaypointListItem::isEmpty() const
{
  return m_ptr->isEmpty();
}

void WaypointListItem::setRemovable(bool removable) const
{
  if (removable)
  {
    m_ptr->ui_remove->show();
  }
  else
  {
    m_ptr->ui_remove->hide();
  }
}

QString WaypointListItem::pointName() const
{
  return m_ptr->ui_line_edit->text();
}
