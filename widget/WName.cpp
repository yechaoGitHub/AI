#include "WName.h"
#include <QPainter>
#include <QPainterPath>

WName::WName(QWidget* parent) :
    QDialog{ parent }
{
    ui.setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    connect(ui.pbClose, &QPushButton::clicked, this, &WName::CloseClicked);
    connect(ui.pbConfirm, &QPushButton::clicked, this, &WName::ConfirmClicked);
}

WName::~WName()
{

}

void WName::paintEvent(QPaintEvent* event)
{
    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(this->rect(), 16, 16);
    painter.fillPath(path, QBrush(QColor(0, 0, 0, 204)));
}

QString WName::Name()
{
    return _name;
}

bool WName::Confirmed()
{
    return _confirmed;
}

void WName::CloseClicked()
{
    close();
}

void WName::ConfirmClicked()
{
    _confirmed = true;
    _name = ui.edName->text();
    close();
}
