#include "WSpeechTimer.h"
#include "AiSound.h"
#include <QHBoxLayout>

WSpeechTimer::WSpeechTimer(QWidget* parent) :
    QWidget{ parent }
{
    QHBoxLayout* horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(14);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    _effect = new QLabel{ this };
    _effect->setStyleSheet("border:1px solid;border-radius:8px;border-color:rgba(255,255,255,50%);");
    _effect->setMaximumSize(QSize{ 300, 44 });
    _effect->setMinimumSize(QSize{ 300, 44 });

    _movie = new QMovie(":/QtTest/icon/color_sound.apng", "apng", this);
    _effect->setMovie(_movie);
    horizontalLayout->addWidget(_effect);

    _counter = new QLabel{ this };
    _counter->setText("00:00");
    _counter->setStyleSheet("color:#FFFFFF; font:14px; background-color:transparent");
    horizontalLayout->addWidget(_counter);

    startTimer(std::chrono::milliseconds{ 100 });

    auto& vc = AiSound::GetInstance().GetVoiceCompositor();
    connect(&vc, &VoiceCompositor::soundPlay, this, &WSpeechTimer::VcSoundPlay);

    //_movie->start();
    //_movie->stop();
}

WSpeechTimer::~WSpeechTimer()
{
}

void WSpeechTimer::StartTimer(bool start)
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
        Play(false);
        _counter->setText("00:00");
    }
}

void WSpeechTimer::Play(bool play)
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

void WSpeechTimer::timerEvent(QTimerEvent* event)
{
    if (_runCounter)
    {
        _passTime = _time.elapsed();
        auto t = QTime::fromMSecsSinceStartOfDay(_passTime /*+ _preTime*/);
        QString text = QString{ "%1:%2" }.arg(QString::number(t.minute()), 2, '0').arg(QString::number(t.second()), 2, '0');
        _counter->setText(text);
    }
}

void WSpeechTimer::VcSoundPlay(bool play)
{
    Play(play);
}

void  WSpeechTimer::Clear()
{
    StartTimer(false);
    _passTime = 0;
    _preTime = 0;
    _counter->setText("00:00");
}

