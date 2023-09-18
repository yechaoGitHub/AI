#include "WRobotChatMainUI.h"
#include "AiSound.h"
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

    //ui.stackedWidget->setCurrentWidget(ui.chat_desc_wgt);
    //connect(ui.chat_desc_wgt, &WChatDesc::sig_startClick, this, &RobotChatMainUI::StartBtnClicked);

    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

RobotChatMainUI::~RobotChatMainUI()
{}

void RobotChatMainUI::Show()
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    //QRect deskRect = desktopWidget->availableGeometry();
    QRect screenRect = desktopWidget->screenGeometry();
    this->move(screenRect.width()-this->width()-20, screenRect.height()-this->height()-300);
    show();
}

void RobotChatMainUI::on_pb_min_clicked()
{
    this->showMinimized();
}

void RobotChatMainUI::on_pb_close_clicked()
{
    this->close();
}

//void RobotChatMainUI::StartBtnClicked()
//{
//    ui.stackedWidget->setCurrentWidget(ui.chat_widget);
//    ui.lb_title->setText("Chat");
//}

void RobotChatMainUI::paintEvent(QPaintEvent* event)
{
    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(this->rect(), 16, 16);
    painter.fillPath(path, QBrush(QColor(0, 0, 0, 204)));
}
