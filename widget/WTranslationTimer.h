#pragma once

#include <QWidget>

class WTranslationTimer : public QWidget
{
    Q_OBJECT
public:
    WTranslationTimer(QWidget *parent = nullptr);
    ~WTranslationTimer();

    void StartTimer();

protected:
    void timerEvent(QTimerEvent* event) override;

};
