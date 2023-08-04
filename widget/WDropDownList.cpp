#include "WDropDownList.h"
#include <QPainter>
#include <Windows.h>


WDropDownListItemWidget::WDropDownListItemWidget(QWidget* parent) :
    QWidget{ parent },
    _mouseIn{ false }
{
    this->setMouseTracking(true);
}

WDropDownListItemWidget::~WDropDownListItemWidget()
{

}

void WDropDownListItemWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    auto rt = rect().marginsRemoved(QMargins{16, 0, 16, 0});
    QTextOption opt;
    opt.setAlignment(Qt::AlignVCenter);
    
    QPen pen{ QColor{ 55, 65, 81} };
    painter.setPen(pen);
    painter.drawText(rt, text, opt);
}

WDropDownList::WDropDownList(QWidget* parent) :
    QWidget{ parent }
{
    setWindowFlags(Qt::Popup);
   
    listWidget = new QListWidget(this);
    listWidget->move(0, 0);
    listWidget->resize(this->width(), this->height());

    connect(listWidget, &QListWidget::itemClicked, this, &WDropDownList::ItemClicked);
}

WDropDownList::~WDropDownList()
{
}

int WDropDownList::RowHeight()
{
    return 44;
}

int WDropDownList::AddItem(const QString& label)
{
    QListWidgetItem* item = new QListWidgetItem(listWidget);
    item->setSizeHint(QSize{width() - 20, RowHeight()});
    listWidget->addItem(item);

    auto w = new WDropDownListItemWidget{};
    w->resize(width() - 20, RowHeight());
    w->text = label;
    listWidget->setItemWidget(item, w);

    return listWidget->row(item);
}

void WDropDownList::resizeEvent(QResizeEvent* event)
{
    listWidget->resize(this->width(), this->height());
    auto c = listWidget->count();

    for (int i = 0; i < c; i++)
    {
        auto item = listWidget->item(i);
        item->setSizeHint(QSize{ width() - 20, RowHeight() });
        auto w = dynamic_cast<WDropDownListItemWidget*>(listWidget->itemWidget(item));
        w->resize(width() - 20, RowHeight());
    }
}

void WDropDownList::showEvent(QShowEvent* event)
{
    ::SetCapture((HWND)winId());
}

void WDropDownList::hideEvent(QHideEvent* event)
{
    emit windowHiden();
    ::ReleaseCapture();
}

void WDropDownList::ItemClicked(QListWidgetItem* item)
{
    int row = listWidget->row(item);
    auto w = dynamic_cast<WDropDownListItemWidget*>(listWidget->itemWidget(item));
    emit itemClicked(row, w->text);
}


