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

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void ConversationGuideReceived(const QString& message, int type);

    Ui::ConversationSuggestion      ui;
    QPoint                          _clickPos;
    bool                            _mouseHold = false;
};