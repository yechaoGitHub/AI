#include "WLoginFrame.h"
#include <QtWidgets/QApplication>
#include "AiSound.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AiSound::GetInstance().Initialize();
    AiSound::GetInstance().ShowLoginFrame();

    //AiSound::GetInstance().PasswordLogin("test", "qwert12345", 
    //    [](int code, const QString& msg, const QString& token)
    //    {
    //        AiSound::GetInstance().CompositorTest(token);
    //    });

    //AiSound::GetInstance().PasswordLogin("hhj", "qwert12345", [](int code, const QString& msg, const QString& token) 
    //    {
    //        AiSound::GetInstance().AudioTest(token);
    //    });

  /*  AiSound::GetInstance().GetTranslationSrourceList(
        [](int code, const QString& msg, const QString& language, const QString& name, const QString& nameEn) 
        {
            return;
        });*/


    //WLoginFrame w;
    //w.show();
    return a.exec();
}
