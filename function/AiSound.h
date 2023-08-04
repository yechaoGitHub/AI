#pragma once
#include <QString>
#include <QObject>
#include <QNetworkAccessManager>
#include <QAudioInput>
#include <QWebsocket>
#include <QThread>
#include <functional>
#include <list>

#include "VoiceCompositor.h"
#include "ChatBot.h"

#include "WLoginFrame.h"
#include "Translate.h"
#include "WTip.h"

class WTranslationSelect;
class WTransaltionMain;

struct TranslationLanguage 
{
    QString name;
    QString nameEn;
    QString language;
};

using PasswordLoginCallbackType = void(int code, const QString& msg, const QString& token);
using PasswordLoginCallback = std::function<PasswordLoginCallbackType>;

using GetVerifyCodeCallbackType = void(int code, const QString& msg, const QString& img, const QString& uuid);
using GetVerifyCodeCallback = std::function<GetVerifyCodeCallbackType>;

using RegisterCallbackType = void(int code, const QString& msg);
using RegisterCallback = std::function<RegisterCallbackType>;

using SendVerifyCodeCallbackType = void(int code, const QString& msg);
using SendVerifyCodeCallback = std::function<SendVerifyCodeCallbackType>;

using GetTranslationSourceListCallbackType = void(int code, const QString& msg, const std::vector<TranslationLanguage>& languageList);
using GetTranslationSourceListCallback = std::function<GetTranslationSourceListCallbackType>;

using GetTranslationDestListCallbackType = void(int code, const QString& msg, const std::vector<TranslationLanguage>& languageList);
using GetTranslationDestListCallback = std::function<GetTranslationDestListCallbackType>;


class AiSound : public QObject
{
    Q_OBJECT

public:
    static AiSound& GetInstance();
    ~AiSound();

    void Initialize();

#pragma region http调用
    void PasswordLogin(const QString& userName, const QString& password, PasswordLoginCallback callback);
    void GetVerifyCode(GetVerifyCodeCallback callback);
    void Register(const QString& userName, const QString& password, const QString& dialingCode, const QString& phoneNumber, const QString& verifyCode, RegisterCallback callback);
    void SendVerifyCode(const QString& dCode, const QString& mobileNumber, const QString& verifyCode, const QString& uuid, SendVerifyCodeCallback callback);
    void GetTranslationSrourceList(GetTranslationSourceListCallback callback);
    void GetTranslationDestList(GetTranslationDestListCallback callback);
    void ShowLoginFrame();
    void ShowTranslationWindow();
    void ShowTranslationMainWindow();
#pragma endregion

#pragma region 翻译
    void TranslateConnect();
#pragma endregion

#pragma region 功能
    void ShowTip(const QString& msg);
    Translate& Translation();
    const QString& Token();
#pragma endregion

#pragma region 测试
    void AudioTest(const QString& token);
    void CompositorTest(const QString& token);
    void ChatBotTest(const QString& token);
#pragma endregion
private:
    AiSound();
    void HttpCallbackDispatch(QNetworkReply* reply);

    void FillTranslationFillList();
    void UserLoginCallbackInternal(int code, const QString& msg, const QString& token);
    void NextMessage();

    enum HttpCallEnum
    {
        httpPasswordLogin = 1,
        httpGetVerifyCode = 2,
        httpRegister = 3,
        httpSendVerifyCode = 4,
        httpGetTranslationSource = 5,
        httpGetTranslationDest = 6
    };

    struct HttpCallbackPacketRaw : QObjectUserData
    {
        HttpCallEnum type;
    };

    template<typename T>
    struct HttpCallbackPacket : HttpCallbackPacketRaw
    {
        std::function<T> callback;
    };

    static AiSound          INSTANCE;

    std::list<QString>      _ltMsg;
    QString                 _token;

    QNetworkAccessManager   _networkAccess;
    QThread                 _translateThread;
    Translate               _translate;
    VoiceCompositor         _voiceCompositor;
    ChatBot                 _chatBot;
#pragma region 窗口
    WLoginFrame*            _wLoginFrame;
    WTranslationSelect*     _wTranslationSelect;
    WTransaltionMain*       _wTranslationMain;
    WTip*                   _wTip;
#pragma endregion
};

