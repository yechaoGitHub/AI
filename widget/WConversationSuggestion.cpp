#include "WConversationSuggestion.h"
#include "AiSound.h"

#include <QPainterPath>

WConversationSuggestion::WConversationSuggestion(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);
    ui.plainTextEdit->setWordWrapMode(QTextOption::WordWrap);
    ui.plainTextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    ui.plainTextEdit->setSizeAdjustPolicy(QPlainTextEdit::AdjustToContents);
    ui.plainTextEdit->setReadOnly(true);
    ui.plainTextEdit->setStyleSheet("color:rgba(255, 255, 255, 255);border:0px;background-color:rgba(0, 0, 0, 0);");

    ui.plainTextEdit->setPlaceholderText("Meeting Name: Discussing AI in design field\n\nthe meeting on the scope of AI in the design field provided a platform for professionals to discuss the current landscape and future possibilities. It highlighted the positive impact of AI in design while emphasizing the need for responsible and ethical integration. The meeting concluded with a shared vision of a harmonious collaboration between designers and AI systems, leveraging the strengths of both to drive innovation in the field.\n\n\
During the meeting, several key points were highlighted.Firstly, participants acknowledged that AI has already made significant contributions to the design field, particularly in areas such as automated design generation, predictive analytics, and data - driven decision - making.AI - powered tools and algorithms have facilitated faster and more efficient design processes, enabling designers to focus on more creative aspects of their work.");

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    connect(ui.minBtn, &QPushButton::clicked, this, &WConversationSuggestion::minBtnClicked);
    connect(ui.closeBtn, &QPushButton::clicked, this, &WConversationSuggestion::closeCBtnClicked);

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

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(this->rect(), 16, 16);
    painter.fillPath(path, QBrush(QColor(0, 0, 0, 204)));
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

void WConversationSuggestion::minBtnClicked()
{
    this->showMinimized();
}

void WConversationSuggestion::closeCBtnClicked()
{
    this->close();
}
