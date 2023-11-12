#include "WTip.h"
#include <Windows.h>
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>

WTip::WTip(QWidget* parent) :
    QWidget{ parent },
    _percent{ 0.0f }
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

    _xRect.setSize(QSize{14, 14});
    _xRect.moveTo(405, 17);

    this->resize(440, 48);
    this->setMouseTracking(true);
}

WTip::~WTip()
{
}

void WTip::SetShowLen(int32_t timeLen)
{
    _timeLen = timeLen;
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

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(this->rect(), 8, 8);
    painter.fillPath(path, QBrush{ QColor{ 255, 255, 255, (int)(255 * _percent) } });

    QPen pen;
    pen.setColor(QColor{ 127, 127, 127, (int)(255 * _percent) });
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setFont(this->font());

    painter.drawRoundedRect(this->rect(), 8, 8);
    painter.drawLine(_xRect.topLeft(), _xRect.bottomRight());
    painter.drawLine(_xRect.bottomLeft(), _xRect.topRight());

    QPen textPen;
    textPen.setColor(QColor{ 0, 0, 0, (int)(255 * _percent) });
    painter.setPen(textPen);

    auto textRt = this->rect().marginsRemoved({ 24, 0, 28, 0});
    QTextOption opt;
    opt.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    painter.drawText(textRt, _msg, opt);
}

void WTip::timerEvent(QTimerEvent* event)
{
    auto cur = std::chrono::steady_clock::now();
    auto diff = cur - _showPoint;
    auto nonaDiff = diff.count();
    auto showLen = std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::seconds{ _timeLen }).count();
    _percent = 1.0f - (std::min)((double)nonaDiff / (double)showLen, 1.0);
    repaint();

    if (std::chrono::duration_cast<std::chrono::seconds>(diff).count() >= _timeLen)
    {
        close();
        this->deleteLater();
    }
}

void WTip::showEvent(QShowEvent* event)
{
    _percent = 1.0f;
    _showPoint = std::chrono::steady_clock::now();
    startTimer(50);
}

void WTip::mouseMoveEvent(QMouseEvent* event)
{
    auto pos = event->pos();
    if (_xRect.marginsAdded({ 7, 7, 7, 7 }).contains(pos))
    {
        setCursor(Qt::PointingHandCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }
}

void WTip::mouseReleaseEvent(QMouseEvent* event)
{
    auto pos = event->pos();
    if (_xRect.marginsAdded({ 7, 7, 7, 7 }).contains(pos))
    {
        close();
        this->deleteLater();
    }
}

