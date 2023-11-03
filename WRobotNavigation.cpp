#include "WRobotNavigation.h"
#include <QPainter>


WRobotNavigation::WRobotNavigation(QWidget *parent)
    : FrameLessWidget(parent)
{
    ui.setupUi(this);
    this->setWidgetType(true, DragType::Drag_Null, false);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);
    this->installEventFilter(this);
    //setWindowFlags();
    this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool/*| Qt::SubWindow*/);
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
            //emit sig_robot_clicked(Navig_Type::Quite);
            exitAction2_->setText(tr("Float Window"));
            this->hide();
        }
        _setting_menu->hide();
        ui.frame->hide();
        });
    initTrayIcon();
}

WRobotNavigation::~WRobotNavigation()
{
    this->removeEventFilter(this);
    delete _setting_menu;
    _setting_menu = nullptr;
}

void WRobotNavigation::initTrayIcon()
{
    trayIcon_.reset(new QSystemTrayIcon);
    trayIcon_->setIcon(QIcon(":QtTest/icon/icon.ico"));
    trayIcon_->setToolTip(tr("AiSound"));
    connect(trayIcon_.data(), &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason reason) {
        switch (reason)
        {
        case QSystemTrayIcon::DoubleClick:
            //双击或者单击托盘图标
            showNormal();
            break;
        default:
            break;
        }
        });
    trayMenu_.reset(new QMenu);
    exitAction_.reset(new QAction(tr("Setting")));
    connect(exitAction_.data(), &QAction::triggered, this, [=] {
        emit sig_robot_clicked(Navig_Type::System_Set);
        });
    trayMenu_->addAction(exitAction_.data());

    exitAction2_.reset(new QAction(QString::fromLocal8Bit("√Float Window")));
    connect(exitAction2_.data(), &QAction::triggered, this, [=] {
        if (this->isHidden()) {
            this->show();
            exitAction2_->setText(QString::fromLocal8Bit("√Float Window"));
        }
        else {
            this->hide();
            exitAction2_->setText(tr("Float Window"));
        }
        });
    trayMenu_->addAction(exitAction2_.data());

    exitAction1_.reset(new QAction(tr("Exit")));
    connect(exitAction1_.data(), &QAction::triggered, this, [=] {
        trayIcon_->hide();
        this->close();
        emit sig_robot_clicked(Navig_Type::Quite);
        });
    trayMenu_->addAction(exitAction1_.data());

    trayIcon_->setContextMenu(trayMenu_.data());
    trayIcon_->show();
}

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

    QPixmap pixmap(":/QtTest/icon/robot_main.png");
    auto ss = pixmap.size();
    auto it = this->size();
    p.drawPixmap(QRect(it.width()-ss.width(),it.height()-ss.height(),ss.width(),ss.height()), pixmap);
}

void WRobotNavigation::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui.retranslateUi(this);
    }

    QWidget::changeEvent(event);
}

bool WRobotNavigation::eventFilter(QObject* obj, QEvent* e)
{
    if (obj)
    {
        if (e->type() == QEvent::MouseButtonPress)
        {
            auto _pressedPoint = static_cast<QMouseEvent*>(e)->pos();
            if (_pressedPoint.x() > 120 && _pressedPoint.y()>120) {
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