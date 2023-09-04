#include "WRobotChatMainUI.h"
#include "AiSound.h"
#include <QPainter>


RobotChatMainUI::RobotChatMainUI(QWidget *parent)
    : FrameLessWidget(parent)
{
    ui.setupUi(this);
    this->setWidgetType(true, DragType::Drag_Null, false);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint | Qt::SubWindow);

    ui.stackedWidget->setCurrentWidget(ui.chat_desc_wgt);
    connect(ui.chat_desc_wgt, &WChatDesc::sig_startClick, this, &RobotChatMainUI::StartBtnClicked);

}

RobotChatMainUI::~RobotChatMainUI()
{}

void RobotChatMainUI::on_pb_min_clicked()
{
    this->showMinimized();
}

void RobotChatMainUI::on_pb_close_clicked()
{
    this->close();
}

void RobotChatMainUI::StartBtnClicked()
{
    ui.stackedWidget->setCurrentWidget(ui.chat_widget);
    ui.lb_title->setText("Chat");
}

void RobotChatMainUI::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    QRect frameRect = rect();
    frameRect.adjust(1, 1, 0, 0);
    p.setPen(QColor(0, 0, 0,204));
    p.setBrush(QColor(0, 0, 0, 204));
    p.drawRoundRect(frameRect,8,8);
}
