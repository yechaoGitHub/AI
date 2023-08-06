#include "WLoginFrame.h"
#include <QtWidgets/QApplication>
#include "AiSound.h"
#include "login/WLoginUI.h"
#include "WRobotNavigation.h"
#include "base/GlobalSetting.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":QtTest/icon/icon.png"));

    if (!SETTING.init(QString(SETTING.getRootLocalPath() + "/system.ini"))) {
        return -1;
    }

    AiSound::GetInstance().Initialize();
    AiSound::GetInstance().ShowLoginFrame();

    auto r = a.exec();
    AiSound::GetInstance().Uninitialize();

    /*AiSound::GetInstance().PasswordLogin("test2", "qwert123456",
        [](int code, const QString& msg, const QString& token)
        {
            AiSound::GetInstance().ChatBotTest(token);

        });*/

    //AiSound::GetInstance().PasswordLogin("hhj", "qwert12345", [](int code, const QString& msg, const QString& token)
    //    {
    //        AiSound::GetInstance().AudioTest(token);
    //    });

  /*  AiSound::GetInstance().GetTranslationSrourceList(
        [](int code, const QString& msg, const QString& language, const QString& name, const QString& nameEn)
        {
            return;
        });*/

    return r;
}
