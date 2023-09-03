#include "WSimTrans.h"

#include <QMouseEvent>
#include <QPainter>

WSimTrans::WSimTrans(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

WSimTrans::~WSimTrans()
{
}

void WSimTrans::paintEvent(QPaintEvent* event)
{
    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
    painter.fillRect(this->rect(), QColor{ 0, 0, 0, 204 });
}

void WSimTrans::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        _clickPos.setX(event->pos().x());
        _clickPos.setY(event->pos().y());
        _mouseHold = true;
    }
}

void WSimTrans::mouseMoveEvent(QMouseEvent* event)
{
    if (_mouseHold && event->buttons() == Qt::LeftButton)
    {
        this->move(this->pos() + event->pos() - this->_clickPos);
    }
}

void WSimTrans::mouseReleaseEvent(QMouseEvent* event)
{
    _mouseHold = false;
}
