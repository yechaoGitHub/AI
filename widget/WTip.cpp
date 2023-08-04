#include "WTip.h"
#include <Windows.h>
#include <QPainter>

WTip::WTip() :
    _alpha{ 255 }
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

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
    painter.fillRect(this->rect(), QColor{ 255, 128, 127, _alpha });
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
    _showPoint = std::chrono::steady_clock::now();
    startTimer(10);
}

void WTip::hideEvent(QHideEvent* event)
{
    emit tipEnd();
}
