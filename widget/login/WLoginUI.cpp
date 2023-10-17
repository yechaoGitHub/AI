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
    ClearPage();
}

void WLoginUI::ForgetPasswordClicked()
{
    auto& ins = AiSound::GetInstance();
    auto&& phoneCode = ui.pageForgetPassword->DialingCode();
    auto&& userName = ui.pageForgetPassword->UserName();
    auto&& password = ui.pageForgetPassword->Password();
    auto&& rePassword = ui.pageForgetPassword->Repassword();
    auto&& vCode = ui.pageForgetPassword->VerifyCode();

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

    ins.ForgetPassword(phoneCode, userName, password, rePassword, vCode, callback);
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

    ins.Register(phoneCode, userName, password, rePassword, recommondCode, vCode, callback);
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

void WLoginUI::ClearPage()
{
    ui.pageForgetPassword->Clear();
    ui.pageRegister->Clear();
}
