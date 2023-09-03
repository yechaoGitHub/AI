#include "AiDebug.h"

#ifdef _AI_DEBUG
AiDebug AiDebug::INSTANCE;
#endif

AiDebug::AiDebug()
{

}

AiDebug::~AiDebug()
{
}

void AiDebug::Output(const QString& text)
{
#ifdef _AI_DEBUG
    if (INSTANCE._debugWindow)
    {
        emit INSTANCE.addText(text);
    }
#endif
}

void AiDebug::ShowDebugWindow(bool show)
{
#ifdef _AI_DEBUG

    if (!INSTANCE._debugWindow)
    {
        INSTANCE._debugWindow = new WDebugWindow{};
        connect(&INSTANCE, &AiDebug::addText, INSTANCE._debugWindow, &WDebugWindow::AddText);
    }

    if (show)
    {
        INSTANCE._debugWindow->show();
    }
    else
    {
        INSTANCE._debugWindow->hide();
    }
#endif
}

WDebugWindow::WDebugWindow() :
    QWidget{ nullptr }
{
    setWindowFlags(Qt::Tool);

    //_scrollArea = new QScrollArea{ this };
    //_scrollArea->setFrameStyle(QFrame::NoFrame);
    //_scrollArea->setStyleSheet("background:transparent");
    //_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    _label = new QPlainTextEdit{ this };
    _label->setWordWrapMode(QTextOption::WordWrap);
    _label->setSizeAdjustPolicy(QPlainTextEdit::AdjustToContents);
    _label->setReadOnly(true);
    //_scrollArea->setWidget(_label);

    resize(QSize{400, 300});

    _label->resize(this->size());
}

WDebugWindow::~WDebugWindow()
{
}

void WDebugWindow::AddText(const QString& text)
{
    _label->appendPlainText(text + '\n');
    //_label->adjustSize();
}

void WDebugWindow::resizeEvent(QResizeEvent* event)
{
    _label->resize(this->size());
}

