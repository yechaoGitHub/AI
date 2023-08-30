#include "WLoginFrame.h"
#include <QtWidgets/QApplication>
#include "AiSound.h"
#include "login/WLoginUI.h"
#include "WRobotNavigation.h"
#include "base/GlobalSetting.h"
#include "AiDebug.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":QtTest/icon/icon.png"));
    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    if (!SETTING.init(QString(SETTING.getRootLocalPath() + "/system.ini"))) {
        return -1;
    }

    AiDebug::ShowDebugWindow(true);

    AiSound::GetInstance().Initialize();
    AiSound::GetInstance().ShowLoginFrame();

    auto r = a.exec();
    AiSound::GetInstance().Uninitialize();

    return r;
}
