#include "WTranslationTimer.h"
#include <QHBoxLayout>
#include <QMovie>
#include <QTime>

WTranslationTimer::WTranslationTimer(QWidget* parent) :
    QWidget{ parent }
{
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);

    _effect = new QLabel{ this };

    _movie = new QMovie(":/QtTest/icon/white_sound.apng", "apng", this);
    _movie->start();
    _effect->setMovie(_movie);
    horizontalLayout->addWidget(_effect);
}

WTranslationTimer::~WTranslationTimer()
{
}

void WTranslationTimer::StartTimer()
{

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
}
