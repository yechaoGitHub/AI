#include "WTranslationTimer.h"

#include <QTime>

WTranslationTimer::WTranslationTimer(QWidget* parent) :
    QWidget{ parent }
{
    startTimer(std::chrono::milliseconds{100});
}

WTranslationTimer::~WTranslationTimer()
{
}

void WTranslationTimer::StartTimer()
{
    //QTime t = QTime::fromMSecsSinceStartOfDay(76131);


}

void WTranslationTimer::timerEvent(QTimerEvent* event)
{
}
