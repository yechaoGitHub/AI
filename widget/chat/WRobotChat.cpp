#include "WRobotChat.h"
#include <QDateTime>
#include <qdebug.h>


WRobotChat::WRobotChat(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    ui.listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

WRobotChat::~WRobotChat()
{}

void WRobotChat::on_pb_voice_clicked()
{
    QString msg = ui.textEdit->toPlainText();
    ui.textEdit->setText("");
    addRobotChatItem(msg);
}

void WRobotChat::addRobotChatItem(const QString& msg)
{
    if (msg.isEmpty()) {
        return;
    }

    QString time = QString::number(QDateTime::currentDateTime().toTime_t());
    chatMessageTime(time);

    WChatItem* messageW = new WChatItem(ui.listWidget->parentWidget());
    QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
    chatMessage(messageW, item, msg, time, WChatItem::User_Robot);
    ui.listWidget->setCurrentRow(ui.listWidget->count() - 1);
}

void WRobotChat::on_pb_send_clicked()
{
    QString msg = ui.textEdit->toPlainText();
    ui.textEdit->setText("");
    QString time = QString::number(QDateTime::currentDateTime().toTime_t());

    qDebug() << "addMessage" << msg << time << ui.listWidget->count();

    if (!msg.isEmpty()) {
        // 此处添加发送请求，异步的话还要处理一下
        bool isSending = true; // 发送中

        if (isSending) {
            chatMessageTime(time);

            WChatItem* messageW = new WChatItem(ui.listWidget->parentWidget());
            QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
            chatMessage(messageW, item, msg, time, WChatItem::User_Self);
        }
        else {
            bool isOver = true;
            for (int i = ui.listWidget->count() - 1; i > 0; i--) {
                WChatItem* messageW = (WChatItem*)ui.listWidget->itemWidget(ui.listWidget->item(i));
                if (messageW->text() == msg) {
                    isOver = false;
                    messageW->setTextSuccess();
                }
            }
            if (isOver) {
                chatMessageTime(time);

                WChatItem* messageW = new WChatItem(ui.listWidget->parentWidget());
                QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
                chatMessage(messageW, item, msg, time, WChatItem::User_Self);
                messageW->setTextSuccess();
            }
        }
    }

    ui.listWidget->setCurrentRow(ui.listWidget->count() - 1);
}

void WRobotChat::chatMessageTime(QString curMsgTime)
{
    bool isShowTime = false;
    if (ui.listWidget->count() > 0) {
        QListWidgetItem* lastItem = ui.listWidget->item(ui.listWidget->count() - 1);
        WChatItem* messageW = (WChatItem*)ui.listWidget->itemWidget(lastItem);
        int lastTime = messageW->time().toInt();
        int curTime = curMsgTime.toInt();
        qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 60); 
    }
    else {
        isShowTime = true;
    }
    if (isShowTime) {
        WChatItem* messageTime = new WChatItem(ui.listWidget->parentWidget());
        QListWidgetItem* itemTime = new QListWidgetItem(ui.listWidget);

        QSize size = QSize(this->width(), 40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        messageTime->setText(curMsgTime, curMsgTime, size, WChatItem::User_Time);
        ui.listWidget->setItemWidget(itemTime, messageTime);
    }
}

void WRobotChat::chatMessage(WChatItem* messageW, QListWidgetItem* item, QString text, QString time, WChatItem::User_Type type)
{
    messageW->setFixedWidth(this->width());
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setText(text, time, size, type);
    ui.listWidget->setItemWidget(item, messageW);
}

void WRobotChat::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
}