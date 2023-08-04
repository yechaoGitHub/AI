#include "WCheckBox.h"
#include <QPainter>

WCheckBox::WCheckBox(QWidget* parent) :
    QWidget{ parent }
{
}

WCheckBox::~WCheckBox()
{
}

void WCheckBox::SetText(const QString& text)
{
    _text = text;
}

void WCheckBox::paintEvent(QPaintEvent* event)
{
    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing, true);

    painter.setClipping(true);
    QRect checkBoxRt{ 0, 0, 20, 20 };
    painter.setClipRect(checkBoxRt);

    QPen pen{ QColor{80, 80, 80} };
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRoundedRect(QRect{0, 0, 20, 20}, 4, 4);

    if (!_text.isEmpty()) 
    {
        painter.setClipping(false);

        QRect textRt;
        textRt.setX(32);
        textRt.setY(0);

        QTextOption opt;
        opt.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        QFontMetrics fm = painter.fontMetrics();
        textRt.setWidth(fm.width(_text));
        textRt.setHeight(fm.height());

        painter.setFont(this->font());
        painter.drawText(textRt, _text, opt);
    }
}

