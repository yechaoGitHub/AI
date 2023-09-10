#pragma once

#include <QWidget>
#include <QLabel>
#include <QElapsedTimer>

class WTranslationTimer : public QWidget
{
    Q_OBJECT
public:
    WTranslationTimer(QWidget *parent = nullptr);
    ~WTranslationTimer();

    QLabel* Effect();

    void StartTimer(bool start);
    void Play(bool play);
    void Clear();

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    void VcSoundPlay(bool play);

    QLabel*             _effect = nullptr;
    QMovie*             _movie = nullptr;
    QLabel*             _counter = nullptr;
    QElapsedTimer       _time;
    uint64_t            _passTime = 0;
    uint64_t            _preTime = 0;
    bool                _runCounter = false;
};
