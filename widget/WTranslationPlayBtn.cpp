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

void WTranslationPlayBtn::SetState(State state)
{
    _state = state;
    repaint();
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
    auto textRt = this->rect();
    QTextOption opt;
    opt.setAlignment(Qt::AlignLeft | Qt::AlignCenter);

    if (_state == PLAY)
    {
        painter.drawText(textRt, "Stop", opt);
    }
    else
    {
        painter.drawText(textRt, "Play", opt);
    }
}

void WTranslationPlayBtn::mouseReleaseEvent(QMouseEvent* event)
{
    if (_state == STOP)
    {
        _state = PLAY;
    }
    else
    {
        _state = STOP;
    }

    emit stateChanged(_state);
    repaint();
}
