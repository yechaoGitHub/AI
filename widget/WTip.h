#pragma once
#include <QWidget>
#include <QPainter>
#include <chrono>

class WTip : public QWidget
{
    Q_OBJECT

public:
    WTip(QWidget* parent = nullptr);
    ~WTip();

    void SetShowLen(int32_t timeLen);
    void SetMessage(const QString& msg);

protected:
    void paintEvent(QPaintEvent* event) override;
    void timerEvent(QTimerEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    float                                   _percent = 0.0f;
    int32_t                                 _timeLen = 10;
    QRect                                   _xRect;
    QString                                 _msg;
    std::chrono::steady_clock::time_point   _showPoint;
};

