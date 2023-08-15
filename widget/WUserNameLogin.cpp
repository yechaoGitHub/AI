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

    ui.keepLoggedCheckBox->SetText("Keep me logged in");
    ui.keepLoggedCheckBox->setVisible(false);
    ui.forgetLabel->setStyleSheet("color:qlineargradient(spread:pad, x1:0, y1:0, x2:111, y2:20, stop:0 #0066FF, stop:1 #BD00FF)");
    ui.forgetLabel->setCursor(Qt::PointingHandCursor);

    userNameEdit = ui.userNameEdit->textEdit;
    passwordEdit = ui.passWordEdit->textEdit;
    forgetLabel = ui.forgetLabel;

    ui.passWordEdit->SetPwd();

    bool ret = SETTING.getRememberPWD();
    if (ret) {
        userNameEdit->setText(SETTING.getUserName());
        passwordEdit->setText(SETTING.getPWD());
    }
    ui.checkBox->setChecked(ret);
}

bool  WUserNameLogin::isRember()
{
    return ui.checkBox->checkState() == Qt::CheckState::Checked;
}

WUserNameLogin::~WUserNameLogin()
{

}
