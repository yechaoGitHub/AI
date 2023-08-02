#pragma once
#include "WCommonWidget.h"

class WVerificationCode : public WCommonWidget
{
    Q_OBJECT
public:
    WVerificationCode(QWidget* parent = nullptr);
    ~WVerificationCode();

    QString Uuid();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    void GetVertifyCodeCallback(int code, const QString& msg, const QString& img, const QString& uuid);

    QImage _vImg;
    QImage _refreshImg;
    QImage _speakerImg;

    QRect _refreshRt;
    QRect _speakerRt;

    QString _uuid;
};

