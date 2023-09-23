#include "WRobotChat.h"
#include "WName.h"

#include "base/GlobalSetting.h"
#include "AiSound.h"
#include <QDateTime>
#include <QKeyEvent>
#include <qdebug.h>

WRobotChat::WRobotChat(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    auto& bot = AiSound::GetInstance().GetChatBot();
    connect(&bot, &ChatBot::receiveText, this, &WRobotChat::ReceiveBotText);

    //connect(ui.pbSave, &QPushButton::clicked, this, &WRobotChat::SaveBtnClicked);

    ui.textEdit->installEventFilter(this);
}

WRobotChat::~WRobotChat()
{}

QPushButton* WRobotChat::SaveBtn()
{
    return ui.pbSave;
}

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

    _curMessage = messageW;
    _curItem = item;

   /* messageW->setFixedWidth(this->width());
    QSize size = messageW->fontRect(msg);
    size.setHeight(1000);
    item->setSizeHint(size);*/

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

        chatMessageTime(time);

        WChatItem* messageW = new WChatItem(ui.listWidget->parentWidget());
        QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
        chatMessage(messageW, item, msg, time, WChatItem::User_Self);
        messageW->setTextSuccess();

        auto& bot = AiSound::GetInstance().GetChatBot();
        bot.SendMessage(msg);

        /*if (isSending) {
            chatMessageTime(time);

            WChatItem* messageW = new WChatItem(ui.listWidget->parentWidget());
            QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
            chatMessage(messageW, item, msg, time, WChatItem::User_Self);
            messageW->setTextSuccess();
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
        }*/
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

void WRobotChat::ReceiveBotText(int type, const QString& text)
{
    if (type == FIN)
    {
        _newMsg = true;
        CurItemStopAnimation();
    }
    else
    {
        if (_newMsg)
        {
            addRobotChatItem(text);
        }
        else
        {
            CurItemAppendText(text);
        }

        _newMsg = false;
    }
}

void WRobotChat::CurItemAppendText(const QString& text)
{
    if (_curMessage)
    {
        _curMessage->appendText(text);

        _curMessage->setFixedWidth(this->width());
        QSize size = _curMessage->fontRect();
        if (_curItem)
        {
            _curItem->setSizeHint(size);
        }
        ui.listWidget->setItemWidget(_curItem, _curMessage);
    }
}

void WRobotChat::CurItemStopAnimation()
{
    if (_curMessage)
    {
        _curMessage->stopAimation();
    }
}

//void WRobotChat::SaveBtnClicked()
//{
//    WName nameDlg{ parentWidget() };
//    nameDlg.resize(QSize{ 476, 116 });
//    nameDlg.move(0, 0);
//    nameDlg.show();
//
//    nameDlg.exec();
//
//    if (nameDlg.Confirmed())
//    {
//        auto name = nameDlg.Name();
//
//        auto& ins = AiSound::GetInstance();
//
//        auto callback = [this](int code, const QString& msg, int32_t current, int32_t pages, int32_t records, int32_t size, int32_t total)
//        {
//            if (code != 200)
//            {
//                auto& ins = AiSound::GetInstance();
//                ins.ShowTip(this, msg);
//            }
//        };
//
//        ins.SaveChat(name, callback);
//    }
//}

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

void WRobotChat::showEvent(QShowEvent* event)
{
    auto& token = AiSound::GetInstance().Token();
    auto& bot = AiSound::GetInstance().GetChatBot();

    auto modelID = SETTING.getRebotModel();

    bot.Connect(token, modelID);
}

void WRobotChat::hideEvent(QHideEvent* event)
{
    auto& bot = AiSound::GetInstance().GetChatBot();
    bot.Disconnect();
}

void WRobotChat::closeEvent(QCloseEvent* event)
{

}

bool WRobotChat::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui.textEdit &&
        event->type() == QEvent::KeyPress)
    {
        QKeyEvent* p = static_cast<QKeyEvent*>(event);
        if (p->key() == Qt::Key_Return)
        {
            on_pb_send_clicked();
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

