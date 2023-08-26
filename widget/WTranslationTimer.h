#pragma once

#include <QWidget>
#include <QLabel>

class WTranslationTimer : public QWidget
{
    Q_OBJECT
public:
    WTranslationTimer(QWidget *parent = nullptr);
    ~WTranslationTimer();

    void StartTimer();

    void Play(bool play);

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    QLabel*     _effect = nullptr;
    QMovie*     _movie = nullptr;
};
