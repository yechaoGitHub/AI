#include "WVerificationCode.h"
#include <QPainter.h>
#include <QMouseEvent>
#include "AiSound.h"

WVerificationCode::WVerificationCode(QWidget* parent):
    WCommonWidget{ parent }
{
    _vImg.load(":/QtTest/icon/image.png");
    _refreshImg.load(":/QtTest/icon/refresh.png");

    //auto& i = AiSound::GetInstance();
    //i.GetVerifyCode(std::bind(&WVerificationCode::GetVertifyCodeCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

    this->setMouseTracking(true);
}

WVerificationCode::~WVerificationCode()
{
}

void WVerificationCode::SetModuleType(const QString& type)
{
    _moduleType = type;
}

void WVerificationCode::FlushVCode()
{
    auto& i = AiSound::GetInstance();
    i.GetVerifyCode(_moduleType, std::bind(&WVerificationCode::GetVertifyCodeCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

QString WVerificationCode::Uuid()
{
    return _uuid;
}

void WVerificationCode::paintEvent(QPaintEvent* event)
{
    WCommonWidget::paintEvent(event);

    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing, true);
    auto imageRt = rect().marginsRemoved(QMargins{8, 8, 32, 8});

    painter.drawImage(imageRt, _vImg, _vImg.rect());
    painter.drawImage(_refreshRt, _refreshImg, _refreshImg.rect());
    //painter.drawImage(_speakerRt, _speakerImg, _speakerImg.rect());
}

void WVerificationCode::mouseReleaseEvent(QMouseEvent* event)
{
    if (_refreshRt.contains(event->pos()))
    {
        auto& i = AiSound::GetInstance();
        i.GetVerifyCode(_moduleType, std::bind(&WVerificationCode::GetVertifyCodeCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    }
    //else if (_speakerRt.contains(event->pos()))
    //{

    //}
}

void WVerificationCode::mouseMoveEvent(QMouseEvent* event)
{
    if (_refreshRt.contains(event->pos()))
    {
        setCursor(Qt::PointingHandCursor);
    }
    //else if (_speakerRt.contains(event->pos()))
    //{
    //    setCursor(Qt::PointingHandCursor);
    //}
    else
    {
        unsetCursor();
    }
}

void WVerificationCode::resizeEvent(QResizeEvent* event)
{
    _imgRt = rect().marginsRemoved(QMargins{ 8, 8, 32, 8 });
    _refreshRt.setX(_imgRt.right() + 8);
    _refreshRt.setY(8);
    _refreshRt.setSize(QSize{16, 16});
}

void WVerificationCode::GetVertifyCodeCallback(int code, const QString& msg, const QString& img, const QString& uuid)
 {
    QByteArray data = QByteArray::fromBase64(img.toUtf8());
    _vImg.loadFromData(data);
    repaint();

    _uuid = uuid;
}
