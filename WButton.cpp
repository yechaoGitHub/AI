#include "WButton.h"
#include <QPainter>
#include <QLinearGradient>
#include <QPainterPath>

WButton::WButton(QWidget* parent) :
    QWidget{ parent }
{   
}

WButton::~WButton()
{
}

void WButton::SetText(const QString& text)
{
    _text = text;
}

void WButton::paintEvent(QPaintEvent* event)
{
    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QLinearGradient g{0.0, 0.0, (float)this->rect().width(), (float)this->rect().height()};
    g.setSpread(QGradient::ReflectSpread);
    g.setColorAt(0, QColor{ 0, 102, 255});
    g.setColorAt(1, QColor{ 189, 0, 255});

    QPainterPath path;
    path.addRoundedRect(this->rect(), 8, 8);
    painter.fillPath(path, g);

    if (!_text.isEmpty()) 
    {
        QTextOption opt;
        opt.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        painter.setFont(this->font());

        QPen pen{ QColor{ 255, 255, 255 } };
        painter.setPen(pen);

        painter.drawText(this->rect(), _text, opt);
    }
}

void WButton::mouseReleaseEvent(QMouseEvent* event)
{
    emit clicked();
}
