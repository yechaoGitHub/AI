#pragma once

#include <QWidget>
#include <QShortcut>
#include "ui_WRobotChat.h"
#include "WChatItem.h"


class WRobotChat : public QWidget
{
    Q_OBJECT

public:
    WRobotChat(QWidget *parent = nullptr);
    ~WRobotChat();

    // 机器人说话调用
    void addRobotChatItem(const QString& msg);
protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void on_pb_send_clicked();
    void on_pb_voice_clicked();

private:
    void chatMessage(WChatItem* messageW, QListWidgetItem* item, QString text, QString time, WChatItem::User_Type type);
    void chatMessageTime(QString curMsgTime);
    void ReceiveBotText(int type, const QString& text);
    void CurItemAppendText(const QString& text);
    void CurItemStopAnimation();

    Ui::WRobotChatClass     ui;
    QShortcut*              _shortCut = nullptr;
    WChatItem*              _curMessage = nullptr;
    QListWidgetItem*        _curItem = nullptr;
    bool                    _newMsg = true;
};
