#pragma once

#include "VoiceType.h"
#include "VoiceCompositor.h"
#include "ChatBot.h"

#include "WLoginFrame.h"
#include "Translation.h"
#include "widget/login/WLoginUI.h"

#include "WTip.h"
#include "WRobotNavigation.h"
#include "widget/chat/WRobotChatMainUI.h"
#include "widget/Speech/WSpeechGenerationUi.h"
#include "widget/Setting/WSettingMainUi.h"

#include <QString>
#include <QObject>
#include <QNetworkAccessManager>
#include <QAudioInput>
#include <QWebsocket>
#include <QThread>
#include <functional>
#include <list>

class WTranslationSelect;
class WTransaltionMain;

using PasswordLoginCallbackType = void(int code, const QString& msg, const QString& token);
using PasswordLoginCallback = std::function<PasswordLoginCallbackType>;

using GetVerifyCodeCallbackType = void(int code, const QString& msg, const QString& img, const QString& uuid);
using GetVerifyCodeCallback = std::function<GetVerifyCodeCallbackType>;

using RegisterCallbackType = void(int code, const QString& msg);
using RegisterCallback = std::function<RegisterCallbackType>;

using SendVerifyCodeCallbackType = void(int code, const QString& msg);
using SendVerifyCodeCallback = std::function<SendVerifyCodeCallbackType>;

using GetTranslationSourceListCallbackType = void(int code, const QString& msg, std::vector<TranslationLanguage> languageList);
using GetTranslationSourceListCallback = std::function<GetTranslationSourceListCallbackType>;

using GetTranslationDestListCallbackType = void(int code, const QString& msg, std::vector<TranslationLanguage> languageList);
using GetTranslationDestListCallback = std::function<GetTranslationDestListCallbackType>;

using GetVoiceSpeakerCallbackType = void(int code, const QString& msg, std::vector<VoiceData> vecVoiceData);
using GetVoiceSpeakerCallback = std::function<GetVoiceSpeakerCallbackType>;

class AiSound : public QObject
{
    Q_OBJECT

public:
    static AiSound& GetInstance();
    AiSound(const AiSound&) = delete;
    AiSound& operator=(const AiSound&) = delete;
    ~AiSound();

    void Initialize();
    void Uninitialize();

#pragma region http调用
    void PasswordLogin(const QString& userName, const QString& password, PasswordLoginCallback callback);
    void GetVerifyCode(GetVerifyCodeCallback callback);
    void Register(const QString& userName, const QString& password, const QString& dialingCode, const QString& phoneNumber, const QString& verifyCode, RegisterCallback callback);
    void SendVerifyCode(const QString& dCode, const QString& mobileNumber, const QString& verifyCode, const QString& uuid, SendVerifyCodeCallback callback);
    void GetTranslationSrourceList(GetTranslationSourceListCallback callback);
    void GetTranslationDestList(GetTranslationDestListCallback callback);
    void GetVoiceSpeaker(GetVoiceSpeakerCallback callback);
#pragma endregion

#pragma region 窗口
    void ShowLoginFrame();
    void ShowTranslationWindow();
    void ShowTranslationMainWindow(const TranslationLanguage& srcLan, const TranslationLanguage& destLan);
    void ShowVoiceCompositorMainWindow(const TranslationLanguage& srcLan, const TranslationLanguage& destLan);
#pragma endregion

#pragma region 功能
    void ShowTip(const QString& msg);
    Translation& GetTranslation();
    VoiceCompositor& GetVoiceCompositor();
    ChatBot& GetChatBot();
    const QString& Token();
    const std::vector<TranslationLanguage>& GetTranslationSrourceListData();
    const std::vector<TranslationLanguage>& GetTranslationDestListData();
    const std::vector<VoiceData>& GetVoiceData();
#pragma endregion

#pragma region 测试
    //void CompositorTest(const QString& token);
    void ChatBotTest(const QString& token);
#pragma endregion

private slots:
    void slot_robot_nv_clicked(Navig_Type type);


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
        httpGetTranslationDest = 6,
        httpGetVoiceSpeakerCallback = 7
    };

    struct HttpCallbackPacketRaw : public QObjectUserData
    {
        HttpCallEnum type;
    };

    template<typename T>
    struct HttpCallbackPacket : HttpCallbackPacketRaw
    {
        std::function<T> callback;
    };

    static AiSound                      INSTANCE;

    std::list<QString>                  _ltMsg;
    QString                             _token;

    QThread                             _netThread;
    QNetworkAccessManager               _networkAccess;

#pragma region 功能
    Translation                         _translation;
    VoiceCompositor                     _voiceCompositor;
    ChatBot                             _chatBot;
#pragma endregion

#pragma region 数据
    std::vector<TranslationLanguage>    _srcTranslationLanguage;
    std::vector<TranslationLanguage>    _destTranslationLanguage;
    std::vector<VoiceData>              _voiceData;
#pragma endregion

#pragma region 窗口
    WLoginUI*                           _wLoginFrame;
    WTranslationSelect*                 _wTranslationSelect;
    WTransaltionMain*                   _wTranslationMain;
    WTip*                               _wTip;

    WRobotNavigation*                   _robotNaviga = nullptr;
    RobotChatMainUI*                    _robot_chat = nullptr;
    WSpeechGenerationUi*                _speech_ui = nullptr;
    WSettingMainUi*                     _set_main = nullptr;
#pragma endregion
};

