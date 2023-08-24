#include "WConversationSuggestion.h"
#include "AiSound.h"

WConversationSuggestion::WConversationSuggestion(QWidget* parent) :
    QWidget{ parent }
{
    ui.setupUi(this);
    ui.labelText->setWordWrap(true);

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

void WConversationSuggestion::ConversationGuideReceived(const QString& message, int type)
{


}
