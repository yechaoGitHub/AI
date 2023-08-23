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

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    this->resize(1078, 252);
    ui.stopBtn->SetState(WTranslationPlayBtn::STOP);

    connect(ui.stopBtn, &WTranslationPlayBtn::stateChanged, this, &WTransaltionMain::StopBtnStateChanged);
    connect(ui.minBtn, &QPushButton::clicked, this, &WTransaltionMain::MinClicked);
    connect(ui.closeBtn, &QPushButton::clicked, this, &WTransaltionMain::CloseClicked);
    connect(ui.lockButton, &QPushButton::clicked, this, &WTransaltionMain::LockClicked);

    auto& translation = AiSound::GetInstance().GetTranslation();
    connect(&translation, &Translation::translationReceived, this, &WTransaltionMain::TranslationReceived);

    setMouseTracking(true);
}

WTransaltionMain::~WTransaltionMain()
{
}

void WTransaltionMain::SetLanguage(const TranslationLanguage& srcLan, const TranslationLanguage& destLan)
{
    _srcLan = srcLan;
    _destLan = destLan;

    ui.srcLabel->setText(_srcLan.name);
    ui.destLabel->setText(_destLan.name);
    ui.subtitleWidget->Subtitle()->SetTranslate(_srcLan.name, _destLan.name);

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
    if (_mouseHold)
    {
        painter.fillRect(this->rect(), QColor{0, 0, 0, 204});
    }
    else
    {
        painter.fillRect(this->rect(), QColor{ 0, 0, 0, 1 });
    }
}

void WTransaltionMain::showEvent(QShowEvent* event)
{
    auto& ins = AiSound::GetInstance();
    auto& token = ins.Token();
    ins.GetTranslation().Connect(token, _srcLan.language, _destLan.language);
}

void WTransaltionMain::enterEvent(QEvent* event)
{
    _mouseHold = true;
    repaint();
}

void WTransaltionMain::leaveEvent(QEvent* event)
{
    _mouseHold = false;
    repaint();
}

void WTransaltionMain::MinClicked()
{
    setWindowState(Qt::WindowMinimized);
}

void WTransaltionMain::CloseClicked()
{
    auto& translation = AiSound::GetInstance().GetTranslation();
    translation.Disconnect();
    close();
}

void WTransaltionMain::LockClicked()
{
    if (this->windowFlags() & Qt::WindowStaysOnTopHint)
    {
        this->setWindowFlags(windowFlags() & (~Qt::WindowStaysOnTopHint));
    }
    else
    {
        this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    }
    show();
}

void WTransaltionMain::TranslationReceived(const QString& src, const QString& dst, int type)
{
    if (_newSubtitle)
    {
        ui.subtitleWidget->Subtitle()->AddSubtitle(src, dst);
    }
    else
    {
        ui.subtitleWidget->Subtitle()->UpdateSubtitle(src, dst);
    }

    if (type == MID)
    {
        _newSubtitle = false;
    }
    else
    {
        _newSubtitle = true;
    }
}

void WTransaltionMain::StopBtnStateChanged(WTranslationPlayBtn::State state)
{
    auto& translation = AiSound::GetInstance().GetTranslation();
    if (state == WTranslationPlayBtn::STOP)
    {
        translation.StopMic();
    }
    else
    {
        translation.StartMic();
    }
}
