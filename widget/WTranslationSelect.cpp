#include "WTranslationSelect.h"
#include <QMouseEvent>
#include <QPainterPath>

WTranslationSelect::WTranslationSelect(QWidget* parent) :
    QWidget{ parent }
{
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    ui.setupUi(this);

    auto& ins = AiSound::GetInstance();
    connect(ui.startBtn, &QPushButton::clicked, this, &WTranslationSelect::StartClicked);
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
        ui.comboSrc->addItem(item.name);
    }
}

void WTranslationSelect::SetDestLanguage(const std::vector<TranslationLanguage>& vecDest)
{
    _vecDest = vecDest;
    for (auto& item : vecDest)
    {
        ui.comboDest->addItem(item.name);
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

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(this->rect(), 16, 16);
    painter.fillPath(path, QBrush(QColor(0, 0, 0, 204)));
}

void WTranslationSelect::showEvent(QShowEvent* event)
{
    auto& ins = AiSound::GetInstance();
    SetSrcLanguage(ins.GetTranslationSrourceListData());
    SetDestLanguage(ins.GetTranslationDestListData());
}

void WTranslationSelect::StartClicked()
{
    auto srcSel = ui.comboSrc->currentIndex();
    auto destSel = ui.comboDest->currentIndex();
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

