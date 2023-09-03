#include "WTranslationSelect.h"
#include <QMouseEvent>

WTranslationSelect::WTranslationSelect(QWidget* parent) :
    QWidget{ parent }
{
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    ui.setupUi(this);
    ui.label->setStyleSheet("color:#FFFFFF");
    ui.minBtn->setIcon(QIcon{":/QtTest/icon/min_btn_white.png"});
    ui.closeBtn->setIcon(QIcon{ ":/QtTest/icon/close_btn_white.png" });
    ui.startBtn->SetText("Start");
    ui.imgLabel->setPixmap(QPixmap{":/QtTest/icon/exchange_big.png"});

    auto& ins = AiSound::GetInstance();
    connect(ui.startBtn, &WButton::clicked, this, &WTranslationSelect::StartClicked);
    connect(ui.closeBtn, &QPushButton::clicked, this, &WTranslationSelect::CloseClicked);
    connect(ui.chatBotButton, &QPushButton::clicked, this, &WTranslationSelect::ChatBotClicked);
}

WTranslationSelect::~WTranslationSelect()
{
}

void WTranslationSelect::SetFunctionType(FunctionType type)
{
    _type = type;
}

void WTranslationSelect::SetSrcLanguage(const std::vector<TranslationLanguage>& vecSrc)
{
    _vecSrc = vecSrc;

    for (auto& item : vecSrc)
    {
        ui.comboSrc->AddItem(item.name, nullptr);
    }
}

void WTranslationSelect::SetDestLanguage(const std::vector<TranslationLanguage>& vecDest)
{
    _vecDest = vecDest;

    for (auto& item : vecDest)
    {
        ui.comboDest->AddItem(item.name, nullptr);
    }
}

void WTranslationSelect::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        _clickPos.setX(event->pos().x());
        _clickPos.setY(event->pos().y());
        _mouseHold = true;
    }
}

void WTranslationSelect::mouseMoveEvent(QMouseEvent* event)
{
    if (_mouseHold && event->buttons() == Qt::LeftButton)
    {
        this->move(this->pos() + event->pos() - this->_clickPos);
    }
}

void WTranslationSelect::mouseReleaseEvent(QMouseEvent* event)
{
    _mouseHold = false;
}

void WTranslationSelect::paintEvent(QPaintEvent* event)
{
    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
    painter.fillRect(this->rect(), QColor{ 0, 0, 0, 204 });
}

void WTranslationSelect::showEvent(QShowEvent* event)
{
    auto& ins = AiSound::GetInstance();
    SetSrcLanguage(ins.GetTranslationSrourceListData());
    SetDestLanguage(ins.GetTranslationDestListData());
}

void WTranslationSelect::StartClicked()
{
    auto srcSel = ui.comboSrc->SelectItem();
    auto destSel = ui.comboDest->SelectItem();
    if (srcSel == -1 || destSel == -1)
    {
        return;
    }

    _selSrvLan = _vecSrc[srcSel];
    _selDestLan = _vecDest[destSel];
    auto& ins = AiSound::GetInstance();
    switch (_type)
    {
        case FunctionType::Translation:
            ins.ShowTranslationMainWindow(_selSrvLan, _selDestLan);
            this->close();
        break;

        case FunctionType::VoiceCompositor:
            ins.ShowVoiceCompositorMainWindow(_selSrvLan, _selDestLan);
            this->close();
        break;

        case FunctionType::ChatBot:
        break;

    }
}

void WTranslationSelect::CloseClicked()
{
    this->close();
}

void WTranslationSelect::ChatBotClicked()
{
    auto& ins = AiSound::GetInstance();
    if (ins.IsConversationSuggestionShow())
    {
        ins.ShowConversationSuggestion(false);
    }
    else
    {
        ins.ShowConversationSuggestion(true);
    }
}

