#include "WTranslationMain.h"
#include "base/GlobalSetting.h"

#include <Windows.h>
#include <QStandardItemModel>
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <QListView>

QString stopStyle = "background-image:url(:/QtTest/icon/stop.png);\
background-position:left;\
background-repeat:no-repeat;\
text-align:left;\
padding-left:20px;\
font-size:14px;\
background-color: rgba(255, 255, 255,0);\
color:#FFFFFF;";

QString playStyle = "background-image:url(:/QtTest/icon/Speech/pause.png);\
background-position:left;\
background-repeat:no-repeat;\
text-align:left;\
padding-left:20px;\
font-size:14px;\
background-color: rgba(255, 255, 255,0);\
color:#FFFFFF;";

WTranslationMain::WTranslationMain(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);

    setWindowFlags(Qt::FramelessWindowHint);


    ui.cbSrc->setView(new QListView{});
    ui.cbDest->setView(new QListView{});

    SetPlayBtnState(true);

    this->resize(1078, 252);

    connect(ui.minBtn, &QPushButton::clicked, this, &WTranslationMain::MinClicked);
    connect(ui.closeBtn, &QPushButton::clicked, this, &WTranslationMain::CloseClicked);
    connect(ui.lockButton, &QPushButton::clicked, this, &WTranslationMain::LockClicked);
    connect(ui.stopBtn, &QPushButton::clicked, this, &WTranslationMain::StopClicked);

    auto& translation = AiSound::GetInstance().GetTranslation();
    connect(&translation, &Translation::translationReceived, this, &WTranslationMain::TranslationReceived);
    connect(&translation, &Translation::soundPlay, this, &WTranslationMain::PlayInternal);
    connect(&translation, &Translation::stateChanged, this, &WTranslationMain::TransStateChanged);

    ui.lockButton->setProperty("lock", false);
    ui.lockButton->style()->unpolish(ui.lockButton);

    setMouseTracking(true);
}

WTranslationMain::~WTranslationMain()
{
}

void WTranslationMain::SetLanguage(const std::vector<TranslationLanguage>& srcLan, const std::vector<TranslationLanguage>& destLan)
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

void WTranslationMain::Clear()
{
    ui.subtitleWidget->Subtitle()->Clear();
}

void WTranslationMain::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        _clickPos.setX(event->pos().x());
        _clickPos.setY(event->pos().y());
        _mouseHold = true;
    }
}

void WTranslationMain::mouseMoveEvent(QMouseEvent* event)
{
    if (_mouseHold && event->buttons() == Qt::LeftButton)
    {
        this->move(this->pos() + event->pos() - this->_clickPos);
    }
}

void WTranslationMain::mouseReleaseEvent(QMouseEvent* event)
{
    _mouseHold = false;
}

void WTranslationMain::paintEvent(QPaintEvent* event)
{
}

void WTranslationMain::showEvent(QShowEvent* event)
{
    ui.timerWidget->Clear();
    ui.cbSrc->setEnabled(true);
    ui.cbDest->setEnabled(true);
    Clear();
}

void WTranslationMain::closeEvent(QCloseEvent* event)
{
    auto& trans = AiSound::GetInstance().GetTranslation();
    if (trans.IsRunning())
    {
        trans.Disconnect();
    }
}

void WTranslationMain::enterEvent(QEvent* event)
{
    int ret = SETTING.getTransBk();
    if (ret == 2) {
        ui.frame->setStyleSheet("QFrame#frame{background-color: rgba(155, 155, 155, 204);border:none;border-radius:15px;}");
    }
    else {
        ui.frame->setStyleSheet("QFrame#frame{background-color: rgba(19, 19, 19,204);border:none;border-radius:15px;}");
    }
    _mouseHold = true;
    update();
}

void WTranslationMain::leaveEvent(QEvent* event)
{
    ui.frame->setStyleSheet("QFrame#frame{background-color: rgba(0, 0, 0,2);border:none;border-radius:15px;}");
    _mouseHold = false;
    update();
}

void WTranslationMain::focusInEvent(QFocusEvent* event)
{
    //_mouseHold = true;
    update();
}

void WTranslationMain::focusOutEvent(QFocusEvent* event)
{
    //_mouseHold = false;
    update();
}

void WTranslationMain::MinClicked()
{
    setWindowState(Qt::WindowMinimized);
}

void WTranslationMain::CloseClicked()
{
    hide();
}

void WTranslationMain::LockClicked()
{
    if (this->windowFlags() & Qt::WindowStaysOnTopHint)
    {
        this->setWindowFlags(windowFlags() & (~Qt::WindowStaysOnTopHint));
        ui.lockButton->setProperty("lock", false);
        ui.lockButton->style()->unpolish(ui.lockButton);
    }
    else
    {
        this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        ui.lockButton->setProperty("lock", true);
        ui.lockButton->style()->unpolish(ui.lockButton);
    }
    show();
}

void WTranslationMain::StopClicked()
{
    auto& ins = AiSound::GetInstance();
    auto& trans = AiSound::GetInstance().GetTranslation();
    bool enableConversation = ins.IsConversationSuggestionShow();
    auto& token = ins.Token();

    if (trans.IsRunning())
    {
        auto& translation = AiSound::GetInstance().GetTranslation();
        translation.Disconnect();

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

        trans.Connect(token, srcLan.language, destLan.language, enableConversation, SETTING.MicDeviceInfo(), SETTING.MonitorDeviceInfo(), SETTING.getTransTT());
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

void WTranslationMain::PlayInternal(bool play)
{
    ui.timerWidget->Play(play);
}

void WTranslationMain::TransStateChanged()
{
    auto& trans = AiSound::GetInstance().GetTranslation();
    if (trans.IsRunning())
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

void WTranslationMain::TranslationReceived(const QString& src, const QString& dst, int type)
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

void WTranslationMain::SetPlayBtnState(bool play)
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

bool WTranslationMain::GetSelectSrcLanguage(TranslationLanguage& language)
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

bool WTranslationMain::GetSelectDestLanguage(TranslationLanguage& language)
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
