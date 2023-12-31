#include "WRobotChatMainUI.h"
#include "AiSound.h"
#include "WSaveTip.h"
#include "WName.h"
#include "base/GlobalSetting.h"

#include <QPainter>
#include <QPainterPath>
#include <QDesktopWidget>

RobotChatMainUI::RobotChatMainUI(QWidget *parent)
    : FrameLessWidget(parent)
{
    ui.setupUi(this);
    this->setWidgetType(true, DragType::Drag_Null, false);
    this->setLimit(60);
    ui.stackedWidget->setCurrentWidget(ui.chat_widget);

    connect(ui.chat_widget->SaveBtn(), &QPushButton::clicked, this, &RobotChatMainUI::SaveBtnClicked);

    this->setAttribute(Qt::WA_TranslucentBackground, true);

    auto& chatBot = AiSound::GetInstance().GetChatBot();
    QObject::connect(&chatBot, &ChatBot::showMessage, this, &RobotChatMainUI::ShowMessage);
}

RobotChatMainUI::~RobotChatMainUI()
{

}

void RobotChatMainUI::Clear()
{
    ui.chat_widget->clearAll();
}

void RobotChatMainUI::Flush()
{
    auto& token = AiSound::GetInstance().Token();
    auto& bot = AiSound::GetInstance().GetChatBot();

    auto modelID = SETTING.getRebotModel();

    bot.Connect(token, modelID, "");
}

void RobotChatMainUI::Show(bool record)
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    //QRect deskRect = desktopWidget->availableGeometry();
    QRect screenRect = desktopWidget->screenGeometry();
    this->move(screenRect.width()-this->width()-20, screenRect.height()-this->height()-300);
    Clear();
    show();
    if (record) {
        ui.chat_widget->reqTemplate();
    }
}

void RobotChatMainUI::ShowRecord(const QString& chatId)
{
    Show(true);
    ui.chat_widget->ShowRecord(chatId);
}

void RobotChatMainUI::on_pb_min_clicked()
{
    this->showMinimized();
}

void RobotChatMainUI::on_pb_close_clicked()
{
    auto pos = this->pos();
    bool ret = ui.chat_widget->notifyClose(pos.x(),pos.y());
    if (ret) {
        this->close();
    }
}

void RobotChatMainUI::SaveBtnClicked()
{
    auto pt = this->rect().center();
    int x = pt.x() - 476 / 2;
    int y = pt.y() - 116 / 2;

    WName nameDlg{ this };
    nameDlg.setModal(true);
    nameDlg.resize(QSize{ 476, 116 });
    nameDlg.move(x, y);
    nameDlg.show();
    nameDlg.exec();

    if (nameDlg.Confirmed())
    {
        auto name = nameDlg.Name();

        auto& ins = AiSound::GetInstance();

        auto callback = [this](int code, const QString& msg, int32_t current, int32_t pages, int32_t records, int32_t size, int32_t total)
        {
            if (code != 200)
            {
                auto& ins = AiSound::GetInstance();
                ins.ShowTip(this, msg);
            }
            else
            {
                ui.chat_widget->setSave();
                _saveCounter++;
            }
        };

        ins.SaveChat(name, callback);
    }
}

void RobotChatMainUI::paintEvent(QPaintEvent* event)
{
    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(this->rect(), 16, 16);
    painter.fillPath(path, QBrush(QColor(0, 0, 0, 204)));
}

void RobotChatMainUI::closeEvent(QCloseEvent* event)
{
    event->setAccepted(false);

    auto& bot = AiSound::GetInstance().GetChatBot();
    if (bot.IsRunning())
    {
        bot.Disconnect();
    }

    hide();
}

void RobotChatMainUI::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui.retranslateUi(this);
    }

    QWidget::changeEvent(event);
}

void RobotChatMainUI::ShowMessage(QString msg)
{
    auto& ai = AiSound::GetInstance();
    ai.ShowTip(this, msg);
}
