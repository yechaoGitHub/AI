#include "WTranslationTimer.h"

#include "AiSound.h"

#include <QHBoxLayout>
#include <QMovie>
#include <QTime>

WTranslationTimer::WTranslationTimer(QWidget* parent) :
    QWidget{ parent }
{
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(14);
    horizontalLayout->setContentsMargins(11, 0, 11, 0);

    _effect = new QLabel{ this };
    _counter = new QLabel{ this };

    _effect->setStyleSheet("border:1px solid;border-radius:8px;border-color:#FFFFFF;");
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(1);
    _effect->setSizePolicy(sizePolicy);

    _effect->setMaximumSize(QSize{ 206, 36 });
    _effect->setMinimumSize(QSize{ 206, 36 });


    _movie = new QMovie(":/QtTest/icon/white_sound.apng", "apng", this);
    _effect->setMovie(_movie);
    horizontalLayout->addWidget(_effect);

    _counter->setText("00:00");
    _counter->setStyleSheet("color:#FFFFFF; font:14px");
    horizontalLayout->addWidget(_counter);

    startTimer(std::chrono::milliseconds{100});

    auto& trans = AiSound::GetInstance().GetTranslation();
    connect(&trans, &Translation::soundPlay, this, &WTranslationTimer::VcSoundPlay);
}

WTranslationTimer::~WTranslationTimer()
{
}

QLabel* WTranslationTimer::Effect()
{
    return _effect;
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
        _effect->setMovie(_movie);
    }
    else
    {
        _runCounter = false;
        Play(false);
        _counter->setText("00:00");

        _effect->setMovie(nullptr);
    }

    _effect->update();
}

void WTranslationTimer::Play(bool play)
{
    if (play && _runCounter)
    {
        _movie->start();
    }
    else
    {
        _movie->stop();
    }
}

void WTranslationTimer::Clear()
{
    StartTimer(false);
    _passTime = 0;
    _preTime = 0;
    _counter->setText("00:00");
}

void WTranslationTimer::timerEvent(QTimerEvent* event)
{
    if (_runCounter)
    {
        _passTime = _time.elapsed();
        auto t = QTime::fromMSecsSinceStartOfDay(_passTime /*+ _preTime*/);
        QString text = QString{ "%1:%2" }.arg(QString::number(t.minute()), 2, '0').arg(QString::number(t.second()), 2, '0');
        _counter->setText(text);
    }
}

void WTranslationTimer::VcSoundPlay(bool play)
{
    Play(play);
}
