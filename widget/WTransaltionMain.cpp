#include "WTransaltionMain.h"
#include <QPainter>
#include <QMouseEvent>


WTransaltionMain::WTransaltionMain(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);

    ui.label->setStyleSheet("color:#FFFFFF");
    ui.srcLabel->setStyleSheet("color:#FFFFFF");
    ui.destLabel->setStyleSheet("color:#FFFFFF");
    ui.imgLabel->setPixmap(QPixmap{":/QtTest/icon/exchange.png"});
    ui.lockButton->setIcon(QIcon{":/QtTest/icon/lock_white.png"});
    ui.minBtn->setIcon(QIcon{":/QtTest/icon/min_btn_white.png"});
    ui.closeBtn->setIcon(QIcon{":/QtTest/icon/close_btn_white.png"});

    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    this->resize(1078, 252);

    ui.subtitleWidget->Subtitle()->SetTranslate("CHS", "EN");

    auto& translation = AiSound::GetInstance().GetTranslation();
    connect(&translation, &Translation::translationReceived, this, &WTransaltionMain::TranslationReceived);
}

WTransaltionMain::~WTransaltionMain()
{
}

void WTransaltionMain::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        _clickPos.setX(event->pos().x());
        _clickPos.setY(event->pos().y());
        _mouseHold = true;
    }
}

void WTransaltionMain::mouseMoveEvent(QMouseEvent* event)
{
    if (_mouseHold && event->buttons() == Qt::LeftButton)
    {
        this->move(this->pos() + event->pos() - this->_clickPos);
    }
}

void WTransaltionMain::mouseReleaseEvent(QMouseEvent* event)
{
    _mouseHold = false;
}

void WTransaltionMain::paintEvent(QPaintEvent* event)
{
    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
    painter.fillRect(this->rect(), QColor{0, 0, 0, 204});
}

void WTransaltionMain::TranslationReceived(const QString& src, const QString& dst, int type)
{
    ui.subtitleWidget->Subtitle()->AddSubtitle(src, dst);
}
