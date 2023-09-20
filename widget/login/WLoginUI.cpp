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
    connect(ui.pageRegister->signBtn, &QPushButton::clicked, this, &WLoginUI::SignUpClicked);
    connect(ui.pageForgetPassword->verifyBtn, &QPushButton::clicked, this, &WLoginUI::ForgetPasswordClicked);

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

void WLoginUI::ForgetPasswordClicked()
{
    auto& ins = AiSound::GetInstance();
    auto&& phoneCode = ui.pageRegister->DialingCode();
    auto&& userName = ui.pageRegister->UserName();
    auto&& password = ui.pageRegister->Password();
    auto&& rePassword = ui.pageRegister->Repassword();
    auto&& vCode = ui.pageRegister->VerifyCode();
    auto&& recommondCode = ui.pageRegister->RecommondCode();

    auto callback = [this](int code, const QString& msg)
    {
        if (code == 200)
        {
            ui.stackedWidget->setCurrentIndex(0);
            ui.pbBack->setVisible(false);
        }
        else
        {
            auto& ins = AiSound::GetInstance();
            ins.ShowTip(this, msg);
        }
    };

    ins.ForgetPassword(userName, password, rePassword, phoneCode, vCode, callback);
}

void WLoginUI::SignUpClicked()
{
    auto& ins = AiSound::GetInstance();
    auto&& phoneCode = ui.pageRegister->DialingCode();
    auto&& userName = ui.pageRegister->UserName();
    auto&& password = ui.pageRegister->Password();
    auto&& rePassword = ui.pageRegister->Repassword();
    auto&& vCode = ui.pageRegister->VerifyCode();
    auto&& recommondCode = ui.pageRegister->RecommondCode();

    auto callback = [this](int code, const QString& msg)
    {
        if (code == 200)
        {
            ui.stackedWidget->setCurrentIndex(0);
            ui.pbBack->setVisible(false);
        }
        else
        {
            auto& ins = AiSound::GetInstance();
            ins.ShowTip(this, msg);
        }
    };

    ins.Register(userName, password, rePassword, phoneCode, recommondCode, vCode, callback);
}

void WLoginUI::RegisterClicked()
{
    ui.stackedWidget->setCurrentIndex(1);
    ui.pbBack->setVisible(true);
}

void WLoginUI::ForgotClicked()
{
    ui.stackedWidget->setCurrentIndex(2);
    ui.pbBack->setVisible(true);
}
