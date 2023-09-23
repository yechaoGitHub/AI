#include "WTransaltionMain.h"
#include "base/GlobalSetting.h"

#include <Windows.h>
#include <QStandardItemModel>
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <QListView>

//QString lockStyle = "background-image:url(:/QtTest/icon/Speech/lock.png);\
//background-color:rgb(0,0,0);\
//background-position:center;\
//background-repeat:no-repeat;\
//border:1px solid;\
//border-radius:4px;\
//border-color:rgba(255, 255, 255, 50 %);";
//
//QString lockActStyle = "background-image:url(:/QtTest/icon/Speech/lock_act.png);\
//background-color:rgb(0,0,0);\
//background-position:center;\
//background-repeat:no-repeat;\
//border:1px solid;\
//border-radius:4px;\
//border-color:rgba(255, 255, 255, 50 %);";


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

    ui.cbSrc->setView(new QListView{});
    ui.cbDest->setView(new QListView{});

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    SetPlayBtnState(true);

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

void WTransaltionMain::SetLanguage(const std::vector<TranslationLanguage>& srcLan, const std::vector<TranslationLanguage>& destLan)
{
    _srcLan = srcLan;
    _destLan = destLan;

    ui.cbSrc->clear();
    for (auto item : _srcLan)
    {
        ui.cbSrc->addItem(item.name);
    }

    ui.cbDest->clear();
    for (auto item : _destLan)
    {
        ui.cbDest->addItem(item.name);
    }
}

void WTransaltionMain::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        _clickPos.setX(event->pos().x());
        _clickPos.setY(event->pos().y());
        _mouseHold = true;

        ::SetCapture((HWND)winId());
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
    ::ReleaseCapture();
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
    ui.timerWidget->Clear();
    ui.cbSrc->setEnabled(true);
    ui.cbDest->setEnabled(true);
}

void WTransaltionMain::closeEvent(QCloseEvent* event)
{
    auto& trans = AiSound::GetInstance().GetTranslation();
    if (trans.IsRunning())
    {
        trans.Disconnect();
    }
}

void WTransaltionMain::enterEvent(QEvent* event)
{
    _mouseHold = true;

    ui.lockButton->setProperty("tt", true);
    ui.lockButton->style()->unpolish(ui.lockButton);

    repaint();
}

void WTransaltionMain::leaveEvent(QEvent* event)
{
    _mouseHold = false;

    ui.lockButton->setProperty("tt", false);
    ui.lockButton->style()->unpolish(ui.lockButton);

    repaint();
}

void WTransaltionMain::MinClicked()
{
    setWindowState(Qt::WindowMinimized);
}

void WTransaltionMain::CloseClicked()
{
    hide();
}

void WTransaltionMain::LockClicked()
{
    if (this->windowFlags() & Qt::WindowStaysOnTopHint)
    {
        this->setWindowFlags(windowFlags() & (~Qt::WindowStaysOnTopHint));
        //ui.lockButton->setStyleSheet(lockStyle);
    }
    else
    {
        this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        //ui.lockButton->setStyleSheet(lockActStyle);
    }
    show();
}

void WTransaltionMain::StopClicked()
{
    auto& ins = AiSound::GetInstance();
    auto& trans = AiSound::GetInstance().GetTranslation();
    bool enableConversation = ins.IsConversationSuggestionShow();
    auto& token = ins.Token();

    if (trans.IsRunning())
    {
        auto& translation = AiSound::GetInstance().GetTranslation();
        translation.Disconnect();

        trans.StopMic();
        ui.timerWidget->StartTimer(false);
        SetPlayBtnState(true);

        ui.cbSrc->setEnabled(true);
        ui.cbDest->setEnabled(true);
    }
    else
    {
        TranslationLanguage srcLan;
        TranslationLanguage destLan;
        if (!GetSelectSrcLanguage(srcLan) ||
            !GetSelectDestLanguage(destLan))
        {
            return;
        }

        ui.subtitleWidget->Subtitle()->SetTranslate(srcLan.name, destLan.name);

        trans.Connect(token, srcLan.language, destLan.language, enableConversation, SETTING.MicDeviceInfo(), SETTING.MonitorDeviceInfo());

        trans.StartMic();
        ui.timerWidget->StartTimer(true);
        SetPlayBtnState(false);

        auto& ins = AiSound::GetInstance();
        bool enableConversation = ins.IsConversationSuggestionShow();
        auto& token = ins.Token();
        ui.timerWidget->Clear();

        ui.cbSrc->setEnabled(false);
        ui.cbDest->setEnabled(false);
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
            SetPlayBtnState(false);
        }
        else
        {
            ui.timerWidget->StartTimer(false);
            SetPlayBtnState(true);
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

void WTransaltionMain::SetPlayBtnState(bool play)
{
    if (play)
    {
        ui.stopBtn->setStyleSheet(playStyle);
        ui.stopBtn->setText(QString::fromLocal8Bit("Play"));
    }
    else
    {
        ui.stopBtn->setStyleSheet(stopStyle);
        ui.stopBtn->setText(QString::fromLocal8Bit("Stpp"));
    }
}

bool WTransaltionMain::GetSelectSrcLanguage(TranslationLanguage& language)
{
    auto index = ui.cbSrc->currentIndex();
    if (index < 0)
    {
        return false;
    }

    if (index >= _srcLan.size())
    {
        return false;
    }

    language = _srcLan[index];

    return true;
}

bool WTransaltionMain::GetSelectDestLanguage(TranslationLanguage& language)
{
    auto index = ui.cbDest->currentIndex();
    if (index < 0)
    {
        return false;
    }

    if (index >= _srcLan.size())
    {
        return false;
    }

    language = _srcLan[index];

    return true;
}
