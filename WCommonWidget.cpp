#include "WCommonWidget.h"
#include <QPainter>

WCommonWidget::WCommonWidget(QWidget *parent)
    : QWidget(parent),
    _select{ false }
{

}

WCommonWidget::~WCommonWidget()
{

}

void WCommonWidget::SetSelect(bool select)
{
    _select = select;
}

void WCommonWidget::paintEvent(QPaintEvent* event)
{
    auto rt = this->rect();
    //rt = rt.marginsRemoved(QMargins{ 1, 1, 1, 1 });
    
    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    if (_select) 
    {
        QLinearGradient g{ 0.0, 0.0, (float)rt.width(), (float)rt.height() };
        g.setSpread(QGradient::ReflectSpread);
        g.setColorAt(0, QColor{ 0, 102, 255 });
        g.setColorAt(1, QColor{ 189, 0, 255 });

        QPen pen{};
        pen.setBrush(g);
        painter.setPen(pen);
    }
    else 
    {
        QPen pen{ QColor{80, 80, 80} };
        pen.setWidth(1);
        painter.setPen(pen);
    }

    QPainterPath path;
    path.addRoundedRect(rt, 8, 8);
    painter.drawPath(path);
}
