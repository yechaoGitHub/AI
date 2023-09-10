#include "WTransaltionMain.h"
#include "base/GlobalSetting.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>


QString stopStyle = "background-image:url(:/QtTest/icon/stop.png);\
background-position:left;\
background-repeat:no-repeat;\
text-align:left;\
padding-left:20px;\
font-size:14px;\
color:#FFFFFF;";

QString playStyle = "background-image:url(:/QtTest/icon/Speech/pause.png);\
background-position:left;\
background-repeat:no-repeat;\
text-align:left;\
padding-left:20px;\
font-size:14px;\
color:#FFFFFF;";

WTransaltionMain::WTransaltionMain(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    this->resize(1078, 252);

    connect(ui.minBtn, &QPushButton::clicked, this, &WTransaltionMain::MinClicked);
    connect(ui.closeBtn, &QPushButton::clicked, this, &WTransaltionMain::CloseClicked);
    connect(ui.lockButton, &QPushButton::clicked, this, &WTransaltionMain::LockClicked);
    connect(ui.stopBtn, &QPushButton::clicked, this, &WTransaltionMain::StopClicked);

    auto& translation = AiSound::GetInstance().GetTranslation();
    connect(&translation, &Translation::translationReceived, this, &WTransaltionMain::TranslationReceived);
    connect(&translation, &Translation::soundPlay, this, &WTransaltionMain::PlayInternal);
    connect(&translation, &Translation::stateChanged, this, &WTransaltionMain::TransStateChanged);

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

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(this->rect(), 16, 16);
    if (_mouseHold)
    {
        painter.fillPath(path, QBrush(QColor(0, 0, 0, 204)));
    }
    else
    {
        painter.fillPath(path, QBrush(QColor(0, 0, 0, 1)));
    }
}

void WTransaltionMain::showEvent(QShowEvent* event)
{
    auto& ins = AiSound::GetInstance();
    bool enableConversation = ins.IsConversationSuggestionShow();
    auto& token = ins.Token();
    ui.timerWidget->Clear();

    ins.GetTranslation().Connect(token, _srcLan.language, _destLan.language, enableConversation, SETTING.MicDeviceInfo(), SETTING.MonitorDeviceInfo());
}

void WTransaltionMain::closeEvent(QCloseEvent* event)
{
    auto& translation = AiSound::GetInstance().GetTranslation();
    translation.Disconnect();
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

void WTransaltionMain::StopClicked()
{
    auto& trans = AiSound::GetInstance().GetTranslation();
    if (trans.IsRunning())
    {
        if (trans.IsMicWorking())
        {
            trans.StopMic();
            ui.timerWidget->StartTimer(false);

            ui.stopBtn->setStyleSheet(playStyle);
            ui.stopBtn->setText(QString::fromLocal8Bit("Play"));
        }
        else
        {
            trans.StartMic();
            ui.timerWidget->StartTimer(true);
            ui.stopBtn->setStyleSheet(stopStyle);
            ui.stopBtn->setText(QString::fromLocal8Bit("Stop"));
        }
    }
}

void WTransaltionMain::PlayInternal(bool play)
{
    ui.timerWidget->Play(play);
}

void WTransaltionMain::TransStateChanged()
{
    auto& trans = AiSound::GetInstance().GetTranslation();
    if (trans.IsRunning())
    {
        if (trans.IsMicWorking())
        {
            ui.timerWidget->StartTimer(true);
            ui.stopBtn->setStyleSheet(stopStyle);
            ui.stopBtn->setText(QString::fromLocal8Bit("Stop"));
        }
        else
        {
            ui.timerWidget->StartTimer(false);
            ui.stopBtn->setStyleSheet(playStyle);
            ui.stopBtn->setText(QString::fromLocal8Bit("Play"));
        }
    }
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
