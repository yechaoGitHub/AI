#include "WSoundLine.h"
#include <qpainter.h>
#include <QPainterPath>


WSoundLine::WSoundLine(QWidget *parent)
    : QWidget(parent)
{
    connect(&_timer, &QTimer::timeout, this, [=] {
        update();
        });
}

WSoundLine::~WSoundLine()
{
    if (_timer.isActive()) {
        _timer.stop();
    }
}

void WSoundLine::startMovice(int volume)
{
    _volume = volume>16? 16: volume;

    if (!_timer.isActive()) {
        _timer.start(100);
    }
    _movice = true;
    update();
}

void WSoundLine::stopMovice()
{
    _volume = 0;
    if (_timer.isActive()) {
        _timer.stop();
    }
    _movice = false;
    update();
}

void WSoundLine::paintEvent(QPaintEvent* event)
{
    auto frame_rect = rect();
    QPainter p(this);
    QPen pen;
    pen.setWidth(4);
    p.setRenderHint(QPainter::Antialiasing);

    int range = 14;
    int start = 10;
    int total = frame_rect.width() / range;

    QPainterPath path;
    pen.setColor("#EDEDED");
    p.setPen(pen);
    for (int i = 0; i < total; i++) {
        if (i * range + 2 + start + 50 > frame_rect.width()) {
            break;
        }
        path.addRoundedRect(QRectF(i * range + 2 + start,1,3, frame_rect.height() - 2),4, 4);
        p.drawPath(path);
    }
    if (_movice) {
        QPainterPath path_tick;
        pen.setColor("#00ff00");
        p.setPen(pen);
        for (int i = 0; i < total; i++) {
            if (i * range + 2 + start + 50 > frame_rect.width()) {
                break;
            }

            if (i >= _volume) {
                break;
            }

            path_tick.addRoundedRect(QRectF(i * range + 2 + start, 1, 3, frame_rect.height() - 2), 4, 4);
            p.drawPath(path_tick);
        }
        /*_cur_line++;
        if (_cur_line > _volume) {
            _cur_line = 0;
        }*/
    }

    //pen.setColor("qlineargradient(spread:pad, x1:0, y1:0, x2:111, y2:20, stop:0 #0066FF, stop:1 #BD00FF)");

    //p.drawLine(QPoint(i*range+2+ start,1), QPoint(i * range + 2+10, frame_rect.height()-2));
}