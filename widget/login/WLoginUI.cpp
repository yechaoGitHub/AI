#include "WLoginUI.h"
#include "AiSound.h"

WLoginUI::WLoginUI(QWidget *parent)
    : FrameLessWidget(parent)
{
    ui.setupUi(this);
    this->setWidgetType(true, DragType::Drag_Null, false);
    setAttribute(Qt::WA_TranslucentBackground);

    connect(ui.PageLogin->registerLabel, &QPushButton::clicked, this, &WLoginUI::RegisterClicked);
    connect(ui.PageLogin->forgotPasswordBtn, &QPushButton::clicked, this, &WLoginUI::ForgotClicked);
    connect(ui.pbBack, &QPushButton::clicked, this, &WLoginUI::BackClicked);

    connect(ui.pb_min, &QPushButton::clicked, this, [=] {
        this->showMinimized();
        });
    connect(ui.pb_close, &QPushButton::clicked, this, [=] {
        this->close();
        });

    ui.stackedWidget->setCurrentIndex(0);
    ui.pbBack->setVisible(false);
}

WLoginUI::~WLoginUI()
{}

void WLoginUI::BackClicked()
{
    ui.stackedWidget->setCurrentIndex(0);
    ui.pbBack->setVisible(false);
}

void WLoginUI::RegisterClicked()
{
    ui.stackedWidget->setCurrentIndex(2);
    ui.pbBack->setVisible(true);
}

void WLoginUI::ForgotClicked()
{
    ui.stackedWidget->setCurrentIndex(2);
    ui.pbBack->setVisible(true);
}
