#include "WRobotChat.h"
#include "WName.h"
#include "Bussiness/SettingInterfaceBussiness.h"
#include "model/WConformDlg.h"
#include "base/GlobalSetting.h"
#include "AiSound.h"
#include <QDateTime>
#include <QKeyEvent>
#include <qdebug.h>
#include <QScrollBar>
#include "AiSound.h"


WRobotChat::WRobotChat(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    auto& bot = AiSound::GetInstance().GetChatBot();
    connect(&bot, &ChatBot::receiveText, this, &WRobotChat::ReceiveBotText);

    _conformDlg = new WConformDlg(nullptr);
    _conformDlg->hide();
    //connect(ui.pbSave, &QPushButton::clicked, this, &WRobotChat::SaveBtnClicked);

    ui.textEdit->installEventFilter(this);
    ui.listWidget->verticalScrollBar()->setMaximum(20);
    ui.listWidget->verticalScrollBar()->setSingleStep(2);
    connect(ui.listWidget->verticalScrollBar(), &QScrollBar::valueChanged, this, [=](int value) {
        if (value <= 2 && !_cur_chatId.isEmpty()) {
            loadPre();
        }
        else if (ui.listWidget->verticalScrollBar()->maximum() == value) {
            //int i = 2;
        }
        });

    qRegisterMetaType<strc_PageInfo>("strc_PageInfo");
    qRegisterMetaType<QVector<strc_chatRecord>>("QVector<strc_chatRecord>");
    connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_chatRecordReplay, this, &WRobotChat::slot_chatRecordReplay);

    auto& chatBot = AiSound::GetInstance().GetChatBot();
    connect(&chatBot, &ChatBot::newConnect, this, &WRobotChat::NewConnect);
}

WRobotChat::~WRobotChat()
{
    if (_conformDlg) {
        delete _conformDlg;
    }

    ui.textEdit->removeEventFilter(this);
}

void WRobotChat::reqTemplate()
{

}

QPushButton* WRobotChat::SaveBtn()
{
    return ui.pbSave;
}

void WRobotChat::loadPre()
{
    if (_cur_page + 1 <= _total_page) {
        SettingInterfaceBussiness::getInstance()->getChatRecord(_page_size, _cur_page+1, _cur_chatId);
    }
}

void WRobotChat::NewConnect()
{
    auto callback = [this](int code, const QString& msg, const QString& description, int id, const QString& initMessage, bool isRecommend, const QString& name, int type)
    {
        if (code == 200)
        {
            addRobotChatItem(initMessage);
        }
    };

    auto modelID = AiSound::GetInstance().GetChatBot().TemplateID();
    AiSound::GetInstance().GetTemplateMessage(modelID, callback);
}

bool WRobotChat::notifyClose(int px,int py)
{
    if (_upload_chat) {
        if (this->parentWidget()) {
            _conformDlg->move(px + (this->parentWidget()->width() - _conformDlg->width()) / 2, py + (this->parentWidget()->height() - _conformDlg->height()) / 2);
        }
        int ret = _conformDlg->Show(tr("Confirmation"), tr("Chat record not saved, exiting will lose the record"));
        if (ret == QDialog::Accepted) {
            _upload_chat = false;
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return true;
    }
}

void WRobotChat::ShowRecord(const QString& chatId)
{
    if (_cur_chatId != chatId) {
        clearAll();
        _cur_chatId = chatId;

        _cur_page = 1;
        _total_page = 1;
        _total_size = 0;

        SettingInterfaceBussiness::getInstance()->getChatRecord(_page_size, _cur_page, _cur_chatId);

        auto& chatBot = AiSound::GetInstance().GetChatBot();
        if (chatBot.IsRunning())
        {
            auto id = chatBot.TemplateID();
            chatBot.Disconnect();
            auto& token = AiSound::GetInstance().Token();
            chatBot.Connect(token, id, chatId);
        }
    }
}

void WRobotChat::slot_chatRecordReplay(bool success, int code, const strc_PageInfo& page, const  QVector<strc_chatRecord>& chat_list)
{
    if (success && code) {
        _cur_page = page.cur_page;
        _total_page = page.total_pages;
        _total_size = page.total_size;

        for (auto it : chat_list) {
            WChatItem::User_Type type = WChatItem::User_Robot;
            if ((it).role == "user") {
                type = WChatItem::User_Self;
            }

            insertChatRecord(type,it.content);
        }

        ui.listWidget->verticalScrollBar()->setValue(ui.listWidget->verticalScrollBar()->maximum());
    }
}

void WRobotChat::clearAll()
{
    QMutexLocker lk(&_mutex);
    _cur_chatId.clear();
    int count = ui.listWidget->count();
    if (count > 0) {
        for (int i = 0; i < ui.listWidget->count();)
        {
            if (ui.listWidget->itemWidget(ui.listWidget->item(i)))
            {
                QListWidgetItem* cur_item = ui.listWidget->item(i);
                this->update();
                delete cur_item;
            }
        }
        ui.listWidget->clear();
    }
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
    QMutexLocker lk(&_mutex);
    QString time = QString::number(QDateTime::currentDateTime().toTime_t());
    chatMessageTime(time);

    WChatItem* messageW = new WChatItem(ui.listWidget->parentWidget());
    QListWidgetItem* item = new QListWidgetItem(ui.listWidget);

    _curMessage = messageW;
    _curItem = item;

    chatMessage(messageW, item, msg, time, WChatItem::User_Robot);
    ui.listWidget->setCurrentRow(ui.listWidget->count() - 1);
    _upload_chat = true;
}

void WRobotChat::insertChatRecord(WChatItem::User_Type type,const QString& msg)
{
    if (msg.isEmpty()) {
        return;
    }
    QMutexLocker lk(&_mutex);
    WChatItem* messageW = new WChatItem(ui.listWidget->parentWidget());
    QListWidgetItem* item = new QListWidgetItem();
    chatMessage(messageW, item, msg, "", type/*WChatItem::User_Robot*/);
    ui.listWidget->insertItem(0, item);
    ui.listWidget->setItemWidget(item, messageW);
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

void WRobotChat::chatMessage(WChatItem* messageW, QListWidgetItem* item, QString text, QString time, WChatItem::User_Type type)
{
    messageW->setFixedWidth(this->width());
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setText(text, time, size, type);
    ui.listWidget->setItemWidget(item, messageW);
    ui.listWidget->insertItem(0,item);
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

    bot.Connect(token, modelID, "");
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

