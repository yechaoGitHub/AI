#include "WSpeechEffect.h"
#include <QHBoxLayout>

WSpeechEffect::WSpeechEffect(QWidget* parent) :
    QWidget{ parent }
{
    QHBoxLayout* horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(14);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    _effect = new QLabel{ this };
    _effect->setStyleSheet("border:1px solid;border-radius:8px;border-color:rgba(255,255,255,50%);");

    _movie = new QMovie(":/QtTest/icon/color_sound.apng", "apng", this);
    _movie->start();
    _effect->setMovie(_movie);
    horizontalLayout->addWidget(_effect);

    _counter = new QLabel{ this };
    _counter->setText("00:00");
    _counter->setStyleSheet("color:#FFFFFF; font:14px; background-color:transparent");
    horizontalLayout->addWidget(_counter);

    startTimer(std::chrono::milliseconds{ 100 });
}

WSpeechEffect::~WSpeechEffect()
{
}

void WSpeechEffect::StartTimer(bool start)
{
    if (start)
    {
        if (!_runCounter && _time.isValid())
        {
            _preTime = _time.restart();
        }
        else
        {
            _time.start();
        }
        _runCounter = true;
    }
    else
    {
        _runCounter = false;
    }
}

void WSpeechEffect::Play(bool play)
{
    if (play)
    {
        _movie->start();
    }
    else
    {
        _movie->stop();
    }
}

void WSpeechEffect::timerEvent(QTimerEvent* event)
{
    if (_runCounter)
    {
        _passTime = _time.elapsed();
        auto t = QTime::fromMSecsSinceStartOfDay(_passTime + _preTime);
        QString text = QString{ "%1:%2" }.arg(QString::number(t.minute()), 2, '0').arg(QString::number(t.second()), 2, '0');
        _counter->setText(text);
    }
}
