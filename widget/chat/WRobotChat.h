#pragma once

#include <QWidget>
#include <QShortcut>
#include <QMutex>
#include "ui_WRobotChat.h"
#include "WChatItem.h"
#include "function/Bussiness/Define.h"


class WRobotChat : public QWidget
{
    Q_OBJECT

public:
    WRobotChat(QWidget *parent = nullptr);
    ~WRobotChat();

    QPushButton* SaveBtn();

    // 机器人说话调用
    void addRobotChatItem(const QString& msg);

    // 插入历史记录
    void insertChatRecord(WChatItem::User_Type type,const QString& msg);

    void ShowRecord(const QString& chatId);
    void clearAll();
protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void on_pb_send_clicked();
    void on_pb_voice_clicked();

    void slot_chatRecordReplay(bool, int, const strc_PageInfo& page, const  QVector<strc_chatRecord>& chat_list);
private:
    void chatMessage(WChatItem* messageW, QListWidgetItem* item, QString text, QString time, WChatItem::User_Type type);
    void chatMessageTime(QString curMsgTime);
    void ReceiveBotText(int type, const QString& text);
    void CurItemAppendText(const QString& text);
    void CurItemStopAnimation();
    void loadPre();
    //void SaveBtnClicked();

    Ui::WRobotChatClass     ui;
    QShortcut*              _shortCut = nullptr;
    WChatItem*              _curMessage = nullptr;
    QListWidgetItem*        _curItem = nullptr;
    bool                    _newMsg = true;

    QMutex                  _mutex;
    QString                 _cur_chatId = 0;

    int                     _total_size = 0;
    int                     _total_page = 0;
    int                     _page_size = 20;
    int                     _cur_page = 1;
};
