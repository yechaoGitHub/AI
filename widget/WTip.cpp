#include "WTip.h"
#include <Windows.h>
#include <QPainter>

WTip::WTip(QWidget* parent) :
    QWidget{ parent },
    _alpha{ 255 }
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

    _xRect.setSize(QSize{14, 14});
    _xRect.moveTo(405, 17);

    this->resize(440, 48);
}

WTip::~WTip()
{
}

void WTip::SetMessage(const QString& msg)
{
    _msg = msg;
}

void WTip::paintEvent(QPaintEvent* event)
{
    static uint8_t a{};

    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
    painter.fillRect(this->rect(), QColor{ 118, 202, 102, 25 });

    QPen pen;
    pen.setColor(QColor{ 118, 202, 102, 255 });
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setFont(this->font());

    painter.drawRoundedRect(this->rect(), 7, 7);

    auto textRt = this->rect().marginsRemoved({ 24, 0, 28, 0});
    QTextOption opt;
    opt.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    painter.drawText(textRt, "This is a success message.", opt);

    painter.drawLine(_xRect.topLeft(), _xRect.bottomRight());
    painter.drawLine(_xRect.bottomLeft(), _xRect.topRight());
}

void WTip::timerEvent(QTimerEvent* event)
{
    auto cur = std::chrono::steady_clock::now();
    auto diff = cur - _showPoint;
    auto nonaDiff = diff.count();
    auto showLen = std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::seconds{ 5 }).count();
    auto percent = (std::min)((double)nonaDiff / (double)showLen, 1.0);
    _alpha *= percent;
    repaint();

    if (std::chrono::duration_cast<std::chrono::seconds>(diff).count() >= 5)
    {
        hide();
    }
}

void WTip::showEvent(QShowEvent* event)
{
    _alpha = 255;
    //_showPoint = std::chrono::steady_clock::now();
    //startTimer(10);
}

void WTip::hideEvent(QHideEvent* event)
{
    emit tipEnd();
}
