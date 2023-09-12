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
    forgetLabel = ui.forgetLabel;

    ui.passWordEdit->SetPwd();

    bool ret = SETTING.getRememberPWD();
    if (ret) {
        userNameEdit->setText(SETTING.getUserName());
        passwordEdit->setText(SETTING.getPWD());
    }
    //ui.checkBox->setChecked(ret);
}

//bool  WUserNameLogin::isRember()
//{
//    return ui.->checkState() == Qt::CheckState::Checked;
//}

WUserNameLogin::~WUserNameLogin()
{

}
