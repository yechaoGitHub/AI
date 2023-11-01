#include "WSettingMainUi.h"
#include <QPainter>
#include <qmath.h>


WSettingMainUi::WSettingMainUi(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint| Qt::X11BypassWindowManagerHint);
    setAttribute(Qt::WA_TranslucentBackground);
    this->installEventFilter(this);
    connect(ui.pb_min, &QPushButton::clicked, this, [this] {
        this->showMinimized();
        });
    connect(ui.pb_close, &QPushButton::clicked, this, [this] {
        this->hide();
        });

    connect(ui.select_widget, &WSettingSelectWidget::sig_robot_clicked, this, [this](bool clicked) {
        if (clicked) {
            static bool first = true;
            if (first) {
                first = false;
                ui.chatbot_page->changeSelectBtn(WNavbarButton::BarType::Bar_Lib);
            }
            ui.chatbot_page->initCheck();
            ui.stackedWidget->setCurrentWidget(ui.chatbot_page);
        }
        else {
            ui.soundbot_page->initCheck();
            ui.stackedWidget->setCurrentWidget(ui.soundbot_page);
        }
    });

    connect(ui.chatbot_page, &WChatBotMainUI::sig_chatBotBack, this, [this] {
        ui.select_widget->initCheck();
        ui.stackedWidget->setCurrentWidget(ui.select_widget);
        });
    connect(ui.soundbot_page, &WSoundBotMainUi::sig_soundBack, this, [this] {
        ui.select_widget->initCheck();
        ui.stackedWidget->setCurrentWidget(ui.select_widget);
        });
    this->setFixedHeight(830);
}

WSettingMainUi::~WSettingMainUi()
{
    this->removeEventFilter(this);
}

void WSettingMainUi::Show(int type)
{
    if (!this->isHidden()) {

    }

    show();
    if (type == 1) {
        ui.chatbot_page->changeSelectBtn(WNavbarButton::BarType::Bar_Lib);
        ui.stackedWidget->setCurrentWidget(ui.chatbot_page);
    }
    ui.select_widget->initData();
}

void WSettingMainUi::resizeEvent(QResizeEvent* re)
{
    auto size = this->size();
    int i = 1;
}

void WSettingMainUi::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    if (isActiveWindow()) {
        p.save();
        p.setRenderHint(QPainter::Antialiasing, true);
        QColor color(QColor(0,0,0,10));
        for (int i = 0; i < 2; i++)
        {
            //color.setAlpha(40 - qSqrt(i) * 20);
            p.setPen(color);
            p.drawRoundedRect(rect().adjusted(4 - i, 4 - i, i - 4, i - 4), 4, 4);
        }
        p.restore();
    }
    auto ss = height();
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(QColor("#ffffff"));
    p.setBrush(QColor("#ffffff"));
    p.drawRoundedRect(4, 4, width() - 8, height() - 8, 8, 8);
}

bool WSettingMainUi::eventFilter(QObject* obj, QEvent* e)
{
    if (obj)
    {
        if (e->type() == QEvent::MouseButtonPress)
        {
            pressedPoint_ = static_cast<QMouseEvent*>(e)->pos();
        }
        else if (e->type() == QEvent::MouseMove)
        {
            if (pressedPoint_ != QPoint(-1, -1))
            {
                auto p = static_cast<QMouseEvent*>(e)->pos();
                if (p.y() < 90 && p.y() > 0) {
                    move(pos() + p - pressedPoint_);
                }
            }
        }
        else if (e->type() == QEvent::MouseButtonRelease)
        {
            pressedPoint_ = QPoint{ -1, -1 };
        }
    }
    return false;
}

void WSettingMainUi::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui.retranslateUi(this);
    }

    QWidget::changeEvent(event);
}