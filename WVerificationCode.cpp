#include "WVerificationCode.h"
#include <QPainter.h>
#include <QMouseEvent>
#include "AiSound.h"

WVerificationCode::WVerificationCode(QWidget* parent):
    WCommonWidget{ parent },
    _refreshRt{ 135, 8, 16, 16 },
    _speakerRt{ 135, 31, 16, 16 }
{
    _vImg.load(":/QtTest/icon/image.png");
    _refreshImg.load(":/QtTest/icon/refresh.png");
    _speakerImg.load(":/QtTest/icon/speaker.png");

    auto& i = AiSound::GetInstance();
    i.GetVerifyCode(std::bind(&WVerificationCode::GetVertifyCodeCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

    this->setMouseTracking(true);
}

WVerificationCode::~WVerificationCode()
{
}

QString WVerificationCode::Uuid()
{
    return _uuid;
}

void WVerificationCode::paintEvent(QPaintEvent* event)
{
    WCommonWidget::paintEvent(event);

    QPainter painter{ this };
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

    auto imageRt = rect().marginsRemoved(QMargins{8, 8, 32, 8});

    painter.drawImage(imageRt, _vImg, _vImg.rect());
    painter.drawImage(_refreshRt, _refreshImg, _refreshImg.rect());
    painter.drawImage(_speakerRt, _speakerImg, _speakerImg.rect());
}

void WVerificationCode::mouseReleaseEvent(QMouseEvent* event)
{
    if (_refreshRt.contains(event->pos()))
    {
        auto& i = AiSound::GetInstance();
        i.GetVerifyCode(std::bind(&WVerificationCode::GetVertifyCodeCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    }
    else if (_speakerRt.contains(event->pos()))
    {
       
    }
}

void WVerificationCode::mouseMoveEvent(QMouseEvent* event)
{
    if (_refreshRt.contains(event->pos()))
    {
        setCursor(Qt::PointingHandCursor);
    }
    else if (_speakerRt.contains(event->pos()))
    {
        setCursor(Qt::PointingHandCursor);
    }
    else 
    {
        unsetCursor();
    }
}

void WVerificationCode::GetVertifyCodeCallback(int code, const QString& msg, const QString& img, const QString& uuid)
 {
    QByteArray data = QByteArray::fromBase64(img.toUtf8());
    _vImg.loadFromData(data);
    repaint();

    _uuid = uuid;
}
