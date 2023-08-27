#include "WTranslationTimer.h"
#include <QHBoxLayout>
#include <QMovie>
#include <QTime>

WTranslationTimer::WTranslationTimer(QWidget* parent) :
    QWidget{ parent }
{
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(14);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);

    _effect = new QLabel{ this };
    _counter = new QLabel{ this };

    _movie = new QMovie(":/QtTest/icon/white_sound.apng", "apng", this);
    _movie->start();
    _effect->setMovie(_movie);
    horizontalLayout->addWidget(_effect);

    _counter->setText("00:00");
    _counter->setStyleSheet("color:#FFFFFF; font:12px");
    horizontalLayout->addWidget(_counter);

    startTimer(std::chrono::milliseconds{100});
}

WTranslationTimer::~WTranslationTimer()
{
}

void WTranslationTimer::StartTimer(bool start)
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

void WTranslationTimer::Play(bool play)
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

void WTranslationTimer::timerEvent(QTimerEvent* event)
{
    if (_runCounter)
    {
        _passTime = _time.elapsed();
        auto t = QTime::fromMSecsSinceStartOfDay(_passTime + _preTime);
        QString text = QString{ "%1:%2" }.arg(QString::number(t.minute()), 2, '0').arg(QString::number(t.second()), 2, '0');
        _counter->setText(text);
    }
}
