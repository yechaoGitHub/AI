#pragma once
#include <QWidget>
#include <QPainter>
#include <chrono>

class WTip : public QWidget
{
    Q_OBJECT

public:
    WTip();
    ~WTip();

    void SetMessage(const QString& msg);

Q_SIGNALS:
    void tipEnd();

protected:
    void paintEvent(QPaintEvent* event) override;
    void timerEvent(QTimerEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;

private:
    int                                     _alpha;
    QString                                 _msg;
    std::chrono::steady_clock::time_point   _showPoint;
};

