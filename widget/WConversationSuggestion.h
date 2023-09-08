#pragma once
#include <QWidget>
#include "ui_ConversationSuggestion.h"

class WConversationSuggestion : public QWidget
{
    Q_OBJECT
public:
    WConversationSuggestion(QWidget* parent = nullptr);
    ~WConversationSuggestion();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    void ConversationGuideReceived(const QString& message, int type);
    void minBtnClicked();
    void closeCBtnClicked();

    Ui::ConversationSuggestion      ui;
    QString                         _textCache;
    QPoint                          _clickPos;
    bool                            _mouseHold = false;
};