#include "WConversationSuggestion.h"
#include "AiSound.h"

WConversationSuggestion::WConversationSuggestion(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);
    ui.labelText->setWordWrap(true);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    auto& ins = AiSound::GetInstance();
    auto& trans = ins.GetTranslation();
    connect(&trans, &Translation::conversationGuideReceived, this, &WConversationSuggestion::ConversationGuideReceived);
}

WConversationSuggestion::~WConversationSuggestion()
{
}

void WConversationSuggestion::SetText(const QString& text)
{
    ui.labelText->setText(text);
}

void WConversationSuggestion::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        _clickPos.setX(event->pos().x());
        _clickPos.setY(event->pos().y());
        _mouseHold = true;
    }
}

void WConversationSuggestion::mouseMoveEvent(QMouseEvent* event)
{
    if (_mouseHold && event->buttons() == Qt::LeftButton)
    {
        this->move(this->pos() + event->pos() - this->_clickPos);
    }
}

void WConversationSuggestion::mouseReleaseEvent(QMouseEvent* event)
{
    _mouseHold = false;
}

void WConversationSuggestion::paintEvent(QPaintEvent* event)
{
    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
    painter.fillRect(this->rect(), QColor{ 0, 0, 0, 204 });
}

void WConversationSuggestion::ConversationGuideReceived(const QString& message, int type)
{


}
