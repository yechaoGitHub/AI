#include "WTranslationPlayBtn.h"

#include <QPainter>

WTranslationPlayBtn::WTranslationPlayBtn(QWidget* parent) :
    QWidget{ parent }
{
    _icon.load(":/QtTest/icon/stop.png");
    _iconRect.setSize(QSize{12, 12});
}

WTranslationPlayBtn::~WTranslationPlayBtn()
{
}

void WTranslationPlayBtn::enterEvent(QEvent* event)
{
    setCursor(Qt::PointingHandCursor);
}

void WTranslationPlayBtn::leaveEvent(QEvent* event)
{
    setCursor(Qt::ArrowCursor);
}

void WTranslationPlayBtn::paintEvent(QPaintEvent* event)
{
    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    int y = (this->rect().height() - _iconRect.height()) / 2;
    QRect drawRect;
    drawRect.setY(y);
    drawRect.setSize(QSize{12, 12});
    painter.drawImage(drawRect, _icon, _icon.rect());

    painter.setPen(QColor{255, 255, 255, 255});
    painter.setFont(this->font());
    auto textRt = this->rect().marginsAdded({ -20, 0, 0, 0 });
    QTextOption opt;
    opt.setAlignment(Qt::AlignLeft | Qt::AlignCenter);
    painter.drawText(textRt, "Stop", opt);

}
