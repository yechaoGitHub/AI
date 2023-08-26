#include "WRadioBtn.h"
#include <QPainter>
#include <QPaintEvent>


#define  radius 12
WRadioBtn::WRadioBtn(QWidget*parent, QColor color)
    : QPushButton(parent)
    , _color(color)
{
    this->setStyleSheet("border:none;");
    this->setFixedSize(36,36);
}

WRadioBtn::~WRadioBtn()
{}

void WRadioBtn::setColor(QColor color)
{
    _color = color;
    update();
}

void WRadioBtn::setSel(bool sel)
{
    _select = sel;
    update();
}

void WRadioBtn::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::Qt4CompatiblePainting);

    if (_select) {
        //painter.setPen(QPen(QColor("qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #0066FF, stop:1 #BD00FF)")));
        //painter.setBrush(QColor("qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #0066FF, stop:1 #BD00FF);"));
        painter.setPen(QPen(QColor("#0066FF")));
        painter.setBrush(QColor("#0066FF"));
    }
    else {
        painter.setPen(QPen(QColor("#B9B9B9")));
        painter.setBrush(QColor("#B9B9B9"));
    }
    painter.drawEllipse(10, 10, radius*2, radius*2);
    painter.restore();

    painter.setPen(QPen(QColor("#ffffff")));
    painter.setBrush(QColor("#ffffff"));
    painter.drawEllipse(10+3, 10+3, radius+6, radius+6);

    painter.setPen(QPen(_color));
    painter.setBrush(_color);

    painter.drawEllipse(16, 16, radius, radius);

}