#include "WTranslationSelect.h"
#include <QMouseEvent>


WTranslationSelect::WTranslationSelect(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);
    ui.label->setStyleSheet("color:#FFFFFF");
    ui.minBtn->setIcon(QIcon{":/QtTest/icon/min_btn_white.png"});
    ui.closeBtn->setIcon(QIcon{ ":/QtTest/icon/close_btn_white.png" });
    ui.startBtn->SetText("Start");
    ui.imgLabel->setPixmap(QPixmap{":/QtTest/icon/exchange_big.png"});

    connect(ui.startBtn, &WButton::clicked, this, &WTranslationSelect::StartClicked);

    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

WTranslationSelect::~WTranslationSelect()
{
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

void WTranslationSelect::StartClicked()
{
    auto& ins = AiSound::GetInstance();

    auto srcSel = ui.comboSrc->SelectItem();
    auto srcLanguage = _vecSrc[srcSel].language;

    auto destSel = ui.comboDest->SelectItem();
    auto destLanguage = _vecDest[destSel].language;

    ins.TranslateConnect();

    ins.ShowTranslationMainWindow();
}
