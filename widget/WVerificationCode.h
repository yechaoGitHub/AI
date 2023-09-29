#pragma once
#include "WCommonWidget.h"

class WVerificationCode : public WCommonWidget
{
    Q_OBJECT
public:
    WVerificationCode(QWidget* parent = nullptr);
    ~WVerificationCode();

    void SetModuleType(const QString& type);

    void FlushVCode();
    QString Uuid();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void GetVertifyCodeCallback(int code, const QString& msg, const QString& img, const QString& uuid);

    QString _moduleType;

    QImage _vImg;
    QImage _refreshImg;
    QImage _speakerImg;

    QRect _imgRt;
    QRect _refreshRt;

    QString _uuid;
};

