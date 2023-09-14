#include "WuserNameLogin.h"
#include "base/GlobalSetting.h"


WUserNameLogin::WUserNameLogin(QWidget* parent) :
    QWidget{ parent },
    userNameEdit{ nullptr },
    passwordEdit{ nullptr }
{
    ui.setupUi(this);
    ui.userNameEdit->textEdit->setPlaceholderText("UserName");
    ui.userNameEdit->SetImage(":QtTest/icon/user_active.png");
    ui.passWordEdit->SetImage(":QtTest/icon/lock.png");

    userNameEdit = ui.userNameEdit->textEdit;
    passwordEdit = ui.passWordEdit->textEdit;

    ui.passWordEdit->SetPwd();

    bool ret = SETTING.getRememberPWD();
    if (ret) {
        userNameEdit->setText(SETTING.getUserName());
        passwordEdit->setText(SETTING.getPWD());
    }
}

WUserNameLogin::~WUserNameLogin()
{

}
