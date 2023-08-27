#pragma once
#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QElapsedTimer>

class WSpeechEffect : public QWidget
{
    Q_OBJECT
public:
    WSpeechEffect(QWidget* parent = nullptr);
    ~WSpeechEffect();

    void StartTimer(bool start);
    void Play(bool play);

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    QLabel*             _effect = nullptr;
    QMovie*             _movie = nullptr;

    QLabel*             _counter = nullptr;
    QElapsedTimer       _time;
    uint64_t            _passTime = 0;
    uint64_t            _preTime = 0;
    bool                _runCounter = false;
};
