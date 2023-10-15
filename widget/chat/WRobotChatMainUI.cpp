#include "WRobotChatMainUI.h"
#include "AiSound.h"
#include "WName.h"

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
    ui.lb_title->setText(tr("Chat"));

    connect(ui.chat_widget->SaveBtn(), &QPushButton::clicked, this, &RobotChatMainUI::SaveBtnClicked);

    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

RobotChatMainUI::~RobotChatMainUI()
{

}

void RobotChatMainUI::Clear()
{
    ui.chat_widget->clearAll();
}

void RobotChatMainUI::Show()
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    //QRect deskRect = desktopWidget->availableGeometry();
    QRect screenRect = desktopWidget->screenGeometry();
    this->move(screenRect.width()-this->width()-20, screenRect.height()-this->height()-300);
    show();
}

void RobotChatMainUI::ShowRecord(const QString& chatId)
{
    Show();
    ui.chat_widget->ShowRecord(chatId);
}

void RobotChatMainUI::on_pb_min_clicked()
{
    this->showMinimized();
}

void RobotChatMainUI::on_pb_close_clicked()
{
    this->hide();
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
