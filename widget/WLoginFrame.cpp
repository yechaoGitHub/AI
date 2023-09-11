#include "WLoginFrame.h"
#include <QFile>
#include <qevent.h>
#include <Windows.h>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QNetworkReply>
#include "AiSound.h"

#include "WComboBox.h"
#include "WTranslationSelect.h"
#include "WTransaltionMain.h"
#include "base/GlobalSetting.h"


WLoginFrame::WLoginFrame(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    _login = new WLogin(this);

    _signUp = new WRegister(this);
    _signUp->hide();

    //connect(_login->registerLabel, &WClickedLabel::clicked, this, &WLoginFrame::RegisterClicked);
    //connect(_login->loginBtn, &WButton::clicked, this, &WLoginFrame::LoginBtnClicked);
    //connect(_signUp->signBtn, &WButton::clicked, this, &WLoginFrame::CommitRegisterBtnClicked);
}

void WLoginFrame::RegisterClicked()
{
    //_login->hide();

   /* _signUp->move(0, 24);
    _signUp->show();*/
}

void WLoginFrame::CommitRegisterBtnClicked()
{
    auto& ins = AiSound::GetInstance();

    auto userName = _signUp->UserName();
    auto password = _signUp->Password();
    QString dCode = "+86";
    auto phoneNumber = _signUp->PhoneNumber();
    auto verifyCode = _signUp->VerifyCode();


    auto callback = [](int code, const QString& msg)->void
    {
        qDebug() << code;
    };

    ins.Register(userName, password, dCode, phoneNumber, verifyCode, std::bind(callback, std::placeholders::_1, std::placeholders::_2));
}

void WLoginFrame::LoginBtnClicked()
{
    auto& ins = AiSound::GetInstance();

    auto userName = _login->UserName();
    auto password = _login->Password();
    if (_login->remberPwd()) {
        SETTING.setPWD(password);
        SETTING.setUserName(userName);
    }
    SETTING.setRememberPWD(_login->remberPwd());
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

void WLoginFrame::handleStateChanged(QAudio::State newState)
{
    switch (newState) {
    case QAudio::IdleState:
        // Finished playing (no more data)
      /*  audio->stop();
        sourceFile.close();
        delete audio;*/
        break;

    case QAudio::StoppedState:
        // Stopped for other reasons
        //if (audio->error() != QAudio::NoError) {
        //    // Error handling
        //}
        break;

    default:
        // ... other cases as appropriate
        break;
    }
}

void WLoginFrame::Connected()
{
}

void WLoginFrame::Disconnected()
{
}

void WLoginFrame::StateChanged(QAbstractSocket::SocketState state)
{

}

void WLoginFrame::TextMessageReceived(const QString& message)
{

}

WLoginFrame::~WLoginFrame()
{

}

void WLoginFrame::receiveGaodePostReply(QNetworkReply* reply)
{
    QString res = reply->readAll();

    return;
}
