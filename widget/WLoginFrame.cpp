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

#include "WTip.h"
#include "WComboBox.h"
#include "WTranslationSelect.h"
#include "WTransaltionMain.h"

WLoginFrame::WLoginFrame(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    _login = new WLogin(this);
    _login->move(0, 24);

    _signUp = new WRegister(this);
    _signUp->hide();

    connect(_login->registerLabel, &WClickedLabel::clicked, this, &WLoginFrame::RegisterClicked);
    connect(_login->loginBtn, &WButton::clicked, this, &WLoginFrame::LoginBtnClicked);
    connect(_signUp->signBtn, &WButton::clicked, this, &WLoginFrame::CommitRegisterBtnClicked);

    //WTransaltionMain* t = new WTransaltionMain{};

    //WComboBox* box = new WComboBox{};
    //t->move(100, 100);
    //t->show();
    //WTip* tip = new WTip{};
    //tip->move(50, 50);
    //tip->show();


    //ui.centralWidget->setStyleSheet("color:rgba(255,255,255,1)");
    //ui.centralWidget->setAutoFillBackground(true);
    
    //QFile f1;
    //f1.setFileName("./main.css");
    //f1.open(QIODevice::ReadOnly);
    //QString strQss = f1.readAll();
    //ui.centralWidget->setStyleSheet(strQss);

    //QFile sourceFile;   // class member.
    //QAudioOutput* audio; // class member.
    //{
    //    sourceFile.setFileName("C:/Users/86788/Desktop/test.pcm");
    //    sourceFile.open(QIODevice::ReadOnly);

    //    QAudioFormat format;
    //    // Set up the format, eg.
    //    format.setSampleRate(8000);
    //    format.setChannelCount(1);
    //    format.setSampleSize(8);
    //    format.setCodec("audio/pcm");
    //    format.setByteOrder(QAudioFormat::LittleEndian);
    //    format.setSampleType(QAudioFormat::SignedInt);

    //    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    //    if (!info.isFormatSupported(format)) {
    //        //qWarning() << "Raw audio format not supported by backend, cannot play audio.";
    //        return;
    //    }

    //    audio = new QAudioOutput(format, this);
    //    //connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));
    //    audio->start(&sourceFile);


    //    //int size = audio->periodSize();
    //    //char* buf = new char[size]; //
    //    //FILE* fp = fopen("C:/Users/86788/Desktop/test.pcm", "rb");
    //    //while (!feof(fp))
    //    //{
    //    //    if (audio->bytesFree() < size)
    //    //    {
    //    //        ::Sleep(1);
    //    //        continue;   //读的速度很快，等待播放完成
    //    //    }
    //    //    int len = fread(buf, 1, size, fp);
    //    //    if (len <= 0) break;
    //    //    io->write(buf, len);  // 播放
    //    //}
    //    //fclose(fp);
    //    //delete[] buf;
    //
    //}

    //_client = new QWebSocket{};
    //_client->setParent(this);

    //connect(_client, &QWebSocket::connected, this, &QtTest::Connected);
    //connect(_client, &QWebSocket::disconnected, this, &QtTest::Disconnected);
    //connect(_client, &QWebSocket::textMessageReceived, this, &QtTest::TextMessageReceived);
    //connect(_client, &QWebSocket::stateChanged, this, &QtTest::StateChanged);

    //QNetworkRequest request{ QUrl{"ws://47.106.253.9:9101/user/loginByPwd"} };
    //

    //_client->open(request);
    //_client->sendTextMessage("{\"password\":\"qwert12345\", \"username\":\"cxm_test\" }");
    
    //QJsonObject dataobj;          

    //dataobj.insert("password", "qwert12345");
    //dataobj.insert("username", "cxm_test");

    //QJsonDocument document;
    //document.setObject(dataobj);
    //QByteArray byte_array = document.toJson(QJsonDocument::Compact);    //类型转换

    //_accessManager = new QNetworkAccessManager{};

    //connect(_accessManager, &QNetworkAccessManager::finished, this, &QtTest::receiveGaodePostReply);

    //QNetworkRequest request;
    //request.setUrl(QUrl{ "http://47.106.253.9:9101/user/loginByPwd" });
    //request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));

    //_accessManager->post(request, byte_array);
    //_client->req
}

void WLoginFrame::RegisterClicked()
{
    _login->hide();

    _signUp->move(0, 24);
    _signUp->show();
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

    auto callback = [](int code, const QString& msg, const QString& token)->void
    {
        auto& ins = AiSound::GetInstance();

        if (code == 200) 
        {
            ins.ShowTranslationWindow();
        }
        else 
        {
            ins.ShowTip(msg);
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
