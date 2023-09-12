#include "WLoginUI.h"
#include "AiSound.h"
#include "base/GlobalSetting.h"

WLoginUI::WLoginUI(QWidget *parent)
    : FrameLessWidget(parent)
{
    ui.setupUi(this);
    this->setWidgetType(true, DragType::Drag_Null, false);
    setAttribute(Qt::WA_TranslucentBackground);

    connect(ui.PageLogin->registerLabel, &QPushButton::clicked, this, &WLoginUI::RegisterClicked);
    connect(ui.PageLogin->loginBtn, &QPushButton::clicked, this, &WLoginUI::RegisterClicked);

    connect(ui.pb_min, &QPushButton::clicked, this, [=] {
        this->showMinimized();
        });
    connect(ui.pb_close, &QPushButton::clicked, this, [=] {
        this->close();
        });
}

WLoginUI::~WLoginUI()
{}

void WLoginUI::RegisterClicked()
{
    ui.stackedWidget->setCurrentIndex(1);
}

void WLoginUI::LoginClicked()
{
    auto& ins = AiSound::GetInstance();

    auto userName = ui.PageLogin->UserName();
    auto password = ui.PageLogin->Password();
    if (ui.PageLogin->remberPwd()) {
        SETTING.setPWD(password);
        SETTING.setUserName(userName);
    }
    SETTING.setRememberPWD(ui.PageLogin->remberPwd());
    auto callback = [this](int code, const QString& msg, const QString& token)->void
    {
        auto& ins = AiSound::GetInstance();

        if (code == 200)
        {
            ins.ShowTranslationWindow();

        }
        else
        {
            ins.ShowTip(this, msg);
        }
    };

    ins.PasswordLogin(userName, password, std::bind(callback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
