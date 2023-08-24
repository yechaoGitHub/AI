#pragma once
#include <QWidget>
#include "ui_ConversationSuggestion.h"

class WConversationSuggestion : public QWidget
{
    Q_OBJECT
public:
    WConversationSuggestion(QWidget* parent = nullptr);
    ~WConversationSuggestion();

    void SetText(const QString& text);

private:
    void ConversationGuideReceived(const QString& message, int type);

    Ui::ConversationSuggestion ui;
};