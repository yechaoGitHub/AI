#include "WConversationSuggestion.h"
#include "AiSound.h"

WConversationSuggestion::WConversationSuggestion(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);
    ui.plainTextEdit->setWordWrapMode(QTextOption::WordWrap);
    ui.plainTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    ui.plainTextEdit->setSizeAdjustPolicy(QPlainTextEdit::AdjustToContents);
    ui.plainTextEdit->setReadOnly(true);
    ui.plainTextEdit->setStyleSheet("color:rgba(255, 255, 255, 255);border:0px;background-color:rgba(0, 0, 0, 0);");

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    auto& ins = AiSound::GetInstance();
    auto& trans = ins.GetTranslation();
    connect(&trans, &Translation::conversationGuideReceived, this, &WConversationSuggestion::ConversationGuideReceived);
}

WConversationSuggestion::~WConversationSuggestion()
{
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
    if (type == MID)
    {
        _textCache.append(message);
    }
    else if (type == FIN)
    {
        _textCache.append("\n");
        ui.plainTextEdit->appendPlainText(_textCache);
        _textCache.clear();
    }
}
