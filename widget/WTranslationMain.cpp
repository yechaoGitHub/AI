#include "WTranslationMain.h"
#include "base/GlobalSetting.h"

#include <Windows.h>
#include <QStandardItemModel>
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <QListView>

WTranslationMain::WTranslationMain(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);

    setWindowFlags(Qt::FramelessWindowHint);
    ui.cbSrc->setView(new QListView{});
    ui.cbDest->setView(new QListView{});

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

    SyncUI();
}

WTranslationMain::~WTranslationMain()
{
}

void WTranslationMain::Clear()
{
    ui.subtitleWidget->Subtitle()->Clear();
}

void WTranslationMain::Flush()
{
    ui.timerWidget->Clear();
    ui.cbSrc->setEnabled(true);
    ui.cbDest->setEnabled(true);
    Clear();
    SyncUI();

    auto& ins = AiSound::GetInstance();

    ins.GetTranslationSrourceList([this](int code, const QString& msg, std::vector<TranslationLanguage> languageList)
        {
            if (code == 200)
            {
                for (auto item : languageList)
                {
                    ui.cbSrc->addItem(item.name);
                }
            }
        });

    ins.GetTranslationDestList([this](int code, const QString& msg, std::vector<TranslationLanguage> languageList)
        {
            if (code == 200)
            {
                for (auto item : languageList)
                {
                    ui.cbDest->addItem(item.name);
                }
            }
        });
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

void WTranslationMain::closeEvent(QCloseEvent* event)
{
    event->setAccepted(false);

    auto& trans = AiSound::GetInstance().GetTranslation();
    if (trans.IsRunning())
    {
        trans.Disconnect();
    }

    hide();
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
    //showMinimized();

    ::ShowWindow((HWND)this->winId(), SW_SHOWMINIMIZED);
}

void WTranslationMain::CloseClicked()
{
    close();
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

        auto& ins = AiSound::GetInstance();

        bool enableConversation = ins.IsConversationSuggestionShow();
        auto& token = ins.Token();

        auto monitor = SETTING.getMonitorDeviceName();
        auto moniotorDevInfo = ins.GetOutputDeviceFormName(monitor, "default");

        auto mic = SETTING.getMicDeviceName();
        auto micDevInfo = ins.GetInputDeviceFormName(mic, "default");

        trans.Connect(token, srcLan.language, destLan.language, enableConversation, moniotorDevInfo, micDevInfo, SETTING.getTransTT());
        ui.subtitleWidget->Subtitle()->SetTranslate(srcLan.name, destLan.name);
    }
}

void WTranslationMain::PlayInternal(bool play)
{
    ui.timerWidget->Play(play);
}

void WTranslationMain::TransStateChanged(int state)
{
    switch (state)
    {
        case TR_RUNING:
            ui.timerWidget->Clear();
            ui.timerWidget->StartTimer(true);
            ui.cbSrc->setEnabled(false);
            ui.cbDest->setEnabled(false);

            ui.stopBtn->setProperty("play", true);
            ui.stopBtn->style()->unpolish(ui.stopBtn);
            ui.stopBtn->setText(tr("Stop"));
        break;

        case TR_STOP:
            ui.timerWidget->StartTimer(false);
            ui.cbSrc->setEnabled(true);
            ui.cbDest->setEnabled(true);

            ui.stopBtn->setProperty("play", false);
            ui.stopBtn->style()->unpolish(ui.stopBtn);
            ui.stopBtn->setText(tr("Play"));
        break;
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

bool WTranslationMain::GetSelectSrcLanguage(TranslationLanguage& language)
{
    auto index = ui.cbSrc->currentIndex();
    if (index < 0)
    {
        return false;
    }

    auto& ins = AiSound::GetInstance();
    const auto& data = ins.GetTranslationSrourceListData();

    if (index >= data.size())
    {
        return false;
    }

    language = data[index];

    return true;
}

bool WTranslationMain::GetSelectDestLanguage(TranslationLanguage& language)
{
    auto index = ui.cbDest->currentIndex();
    if (index < 0)
    {
        return false;
    }

    auto& ins = AiSound::GetInstance();
    const auto& data = ins.GetTranslationDestListData();

    if (index >= data.size())
    {
        return false;
    }

    language = data[index];

    return true;
}

void WTranslationMain::SyncUI()
{
    auto& trans = AiSound::GetInstance().GetTranslation();
    if (trans.IsRunning())
    {
        ui.stopBtn->setProperty("play", true);
        ui.stopBtn->style()->unpolish(ui.stopBtn);
        ui.stopBtn->setText(tr("Stop"));
    }
    else
    {
        ui.stopBtn->setProperty("play", false);
        ui.stopBtn->style()->unpolish(ui.stopBtn);
        ui.stopBtn->setText(tr("Play"));
    }
}

void WTranslationMain::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui.retranslateUi(this);
    }

    QWidget::changeEvent(event);
}