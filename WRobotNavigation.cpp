#include "WRobotNavigation.h"
#include <QPainter>


WRobotNavigation::WRobotNavigation(QWidget *parent)
    : FrameLessWidget(parent)
{
    ui.setupUi(this);
    this->setWidgetType(true, DragType::Drag_Null, false);
    setAttribute(Qt::WA_TranslucentBackground);
    this->installEventFilter(this);

    _setting_menu = new WRobotSettingMenu(this);
    _setting_menu->hide();
    _setting_menu->move(9,9);
    connect(_setting_menu, &WRobotSettingMenu::sig_settingMenu_clicked, this, [=] (int index){
        if (index == 1) {
            emit sig_robot_clicked(Navig_Type::System_Set);
        }
        else if(index == 2) {
            emit sig_robot_clicked(Navig_Type::Chat_Set);
        }
        else {
            emit sig_robot_clicked(Navig_Type::Quite);
        }
        _setting_menu->hide();
        ui.frame->hide();
        });
}

WRobotNavigation::~WRobotNavigation()
{}

void WRobotNavigation::on_pb_chat_clicked()
{
    emit sig_robot_clicked(Navig_Type::Chat);
    _setting_menu->hide();
    ui.frame->hide();
}

void WRobotNavigation::on_pb_voice_clicked()
{
    emit sig_robot_clicked(Navig_Type::Voice);
    _setting_menu->hide();
    ui.frame->hide();
}

void WRobotNavigation::on_pb_speech_clicked()
{
    emit sig_robot_clicked(Navig_Type::Speech);
    _setting_menu->hide();
    ui.frame->hide();
}

void WRobotNavigation::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    QRect frameRect = rect();

    QPixmap pixmap(":/QtTest/icon/robot.png");
    auto ss = pixmap.size();
    auto it = this->size();
    p.drawPixmap(QRect(it.width()-ss.width(),it.height()-ss.height(),ss.width(),ss.height()), pixmap);
}

bool WRobotNavigation::eventFilter(QObject* obj, QEvent* e)
{
    if (obj)
    {
        if (e->type() == QEvent::MouseButtonPress)
        {
            auto _pressedPoint = static_cast<QMouseEvent*>(e)->pos();
            if (_pressedPoint.x() > 150 && _pressedPoint.y()>150) {
                if (qApp->mouseButtons() == Qt::RightButton) {
                    _setting_menu->show();
                    ui.frame->hide();
                }
                if (qApp->mouseButtons() == Qt::LeftButton) {
                    _setting_menu->hide();
                    ui.frame->show();
                }
            }
        }

    }
    return false;
}