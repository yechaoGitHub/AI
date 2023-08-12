#include "WSettingMainUi.h"
#include <QPainter>
#include <qmath.h>


WSettingMainUi::WSettingMainUi(QWidget *parent)
    : FrameLessWidget(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setWidgetType(true, DragType::Drag_Null, false);

    connect(ui.pb_min, &QPushButton::clicked, this, [this] {
        this->showMinimized();
        });
    connect(ui.pb_close, &QPushButton::clicked, this, [this] {
        this->close();
        });

    connect(ui.select_widget, &WSettingSelectWidget::sig_page_change, this, [this](bool max) {
        if (max) {
            this->setFixedSize(852, 830);
        }
        else {
            this->setFixedSize(664, 490);
        }
        });
    this->setFixedSize(664, 490);

    connect(ui.select_widget, &WSettingSelectWidget::sig_robot_clicked, this, [this](bool clicked) {
        if (clicked) {
            this->setFixedSize(939, 830);
            ui.stackedWidget->setCurrentWidget(ui.chatbot_page);
        }
        else {
            this->setFixedSize(852, 830);
            ui.stackedWidget->setCurrentWidget(ui.soundbot_page);
        }

    });

    connect(ui.chatbot_page, &WChatBotMainUI::sig_chatBotBack, this, [this] {
        this->setFixedSize(664, 490);
        ui.stackedWidget->setCurrentWidget(ui.select_widget);
        });
    connect(ui.soundbot_page, &WSoundBotMainUi::sig_soundBack, this, [this] {
        this->setFixedSize(664, 490);
        ui.stackedWidget->setCurrentWidget(ui.select_widget);
        });
}

WSettingMainUi::~WSettingMainUi()
{}

void WSettingMainUi::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    if (isActiveWindow()) {
        p.save();
        p.setRenderHint(QPainter::Antialiasing, true);
        QColor color(0, 0, 0);
        for (int i = 0; i < 4; i++)
        {
            color.setAlpha(60 - qSqrt(i) * 20);
            p.setPen(color);
            p.drawRoundedRect(rect().adjusted(4 - i, 4 - i, i - 4, i - 4), 4, 4);
        }
        p.restore();
    }

    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(QColor("#ffffff"));
    p.setBrush(QColor("#ffffff"));
    p.drawRoundedRect(4, 4, width() - 8, height() - 8, 8, 8);
}