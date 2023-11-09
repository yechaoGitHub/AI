#include "AiSound.h"

#include "WTranslationMain.h"
#include "WRobotNavigation.h"
#include "widget/chat/WRobotChatMainUI.h"
#include "widget/Speech/WSpeechGenerationUi.h"
#include "widget/Setting/WSettingMainUi.h"
#include "widget/login/WLoginUI.h"
#include "WConversationSuggestion.h"
#include "base/GlobalSetting.h"
#include "function/QAudioPlayer.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QAudioInput>
#include <QDesktopWidget>
#include <QApplication>
#include <QTranslator>
#include <QDir>
#include <QDebug>
#include <QTimerEvent>

#include <Windows.h>
#include <thread>
#include <set>

#include "base/HttpClient.h"

AiSound AiSound::INSTANCE;

AiSound::AiSound() :
    _sysLanguage{ LanguageType::EN }
{

}

AiSound::~AiSound()
{
}

void AiSound::Initialize()
{
    _enTs.load("en.qm");
    _chsTs.load("zh_Hans.qm");

    _httpAsync.Initialize();
    connect(&_httpAsync, &HttpAsync::httpRespond, this, &AiSound::HttpCallbackDispatch);

    _wLoginFrame = new WLoginUI{};
    _wTranslationMain = new WTranslationMain{};
    _robotNaviga = new WRobotNavigation(nullptr);
    _wConversationSuggestion = new WConversationSuggestion{};
    connect(_robotNaviga,&WRobotNavigation::sig_robot_clicked,this, &AiSound::slot_robot_nv_clicked);

    _robot_chat = new RobotChatMainUI();
    _speech_ui = new WSpeechGenerationUi();
    _set_main = new WSettingMainUi();

    _translation.Initialize();
    _voiceCompositor.Initialize();
    _chatBot.Initialize();

    GetPhoneRegionNumber([this](int code, const QString& msg, std::vector<PhoneRegionInfo> data)
        {
            if (code == 200)
            {
                _phoneRegionData = std::move(data);
            }
        });
    _audio_play = new QAudioPlayer(nullptr);
    GetInputDeviceList();

    hook.installHook();
    connect(&hook, SIGNAL(sig_sendKeyType(int)), this, SLOT(slot_keyType(int)));

    auto in =  SETTING.getCurLanguage();
    switch (in)
    {
        default:
        case 0:
            SwitchLanguage(LanguageType::EN);
        break;

        case 1:
            SwitchLanguage(LanguageType::CHS);
        break;
    }
}

void AiSound::Uninitialize()
{
    _translation.Uninitialize();
    _voiceCompositor.Uninitialize();
    _chatBot.Uninitialize();
}

AiSound& AiSound::GetInstance()
{
    return INSTANCE;
}

void AiSound::slot_keyType(int type)
{
    if (!_logined) {
        return;
    }
    if (type == 1) {
        _wTranslationMain->show();
        _wTranslationMain->Play();
    }
    else if (type == 2) {
        _speech_ui->show();
        _speech_ui->Play();
    }
}

//º”…œLanguage
void AiSound::PasswordLogin(const QString& userName, const QString& password, LoginCallback callback)
{
    QJsonObject dataObj;
    dataObj.insert("password", password);
    dataObj.insert("username", userName);

    auto packet = new HttpCallbackPacket<LoginCallbackType>();
    packet->type = httpPasswordLogin;
    packet->callback = callback;
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/json;charset=utf-8");
    headers.insert("client_type", "PC");
    headers.insert("language", GetLanguageToken());

    _httpAsync.Post("http://47.106.253.9:9101/api/user/loginByPwd", dataObj, headers, userParam);
}

void AiSound::PhoneLogin(const QString& dialingCode, const QString& mobileNumber, const QString& verifyCode, LoginCallback callback)
{
    QJsonObject dataObj;
    dataObj.insert("dialingCode", dialingCode);
    dataObj.insert("mobileNumber", mobileNumber);
    dataObj.insert("verifyCode", verifyCode);

    auto packet = new HttpCallbackPacket<LoginCallbackType>();
    packet->type = httpPasswordLogin;
    packet->callback = callback;
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/json;charset=utf-8");
    headers.insert("client_type", "PC");
    headers.insert("language", GetLanguageToken());

    _httpAsync.Post("http://47.106.253.9:9101/api/user/loginByMobile", dataObj, headers, userParam);
}

void AiSound::EmailLogin(const QString& mailAddress, const QString& verifyCode, LoginCallback callback)
{
    QJsonObject dataObj;
    dataObj.insert("mailAddress", mailAddress);
    dataObj.insert("verifyCode", verifyCode);

    auto packet = new HttpCallbackPacket<LoginCallbackType>();
    packet->type = httpPasswordLogin;
    packet->callback = callback;
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/json;charset=utf-8");
    headers.insert("client_type", "PC");
    headers.insert("language", GetLanguageToken());

    _httpAsync.Post("http://47.106.253.9:9101/api/user/loginByMail", dataObj, headers, userParam);
}

void AiSound::GetVerifyCode(const QString& moduleType, GetVerifyCodeCallback callback)
{
    QJsonObject dataObj;
    dataObj.insert("moduleType", moduleType);

    auto packet = new HttpCallbackPacket<GetVerifyCodeCallbackType>();
    packet->type = httpGetVerifyCode;
    packet->callback = callback;
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/json;charset=utf-8");
    headers.insert("client_type", "PC");
    headers.insert("language", GetLanguageToken());

    _httpAsync.Post("http://47.106.253.9:9101/api/common/getImgVerifyCode", dataObj, headers, userParam);
}

void AiSound::SendVerifyCode(const QString& dCode, const QString& mobileNumber, const QString& verifyCode, const QString& uuid, const QString& moduleType, SendVerifyCodeCallback callback)
{
    QJsonObject dataObj;
    dataObj.insert("moduleType", moduleType);
    dataObj.insert("mobileNumber", mobileNumber);
    dataObj.insert("dialingCode", dCode);
    dataObj.insert("imgVerifyCode", verifyCode);
    dataObj.insert("uuid", uuid);

    auto packet = new HttpCallbackPacket<SendVerifyCodeCallbackType>();
    packet->type = httpSendVerifyCode;
    packet->callback = callback;
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/json;charset=utf-8");
    headers.insert("client_type", "PC");
    headers.insert("language", GetLanguageToken());

    _httpAsync.Post("http://47.106.253.9:9101/api/user/sendSmsVerifyCode", dataObj, headers, userParam);
}

void AiSound::SendMailVerfyCode(const QString& mailAddress, const QString& verifyCode, const QString& uuid, const QString& moduleType, SendVerifyCodeCallback callback)
{
    QJsonObject dataObj;
    dataObj.insert("moduleType", moduleType);
    dataObj.insert("mailAddress", mailAddress);
    dataObj.insert("imgVerifyCode", verifyCode);
    dataObj.insert("uuid", uuid);

    auto packet = new HttpCallbackPacket<SendVerifyCodeCallbackType>();
    packet->type = httpSendVerifyCode;
    packet->callback = callback;
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/json;charset=utf-8");
    headers.insert("client_type", "PC");
    headers.insert("language", GetLanguageToken());

    _httpAsync.Post("http://47.106.253.9:9101/api/user/sendMailVerifyCode", dataObj, headers, userParam);
}

void AiSound::GetTranslationSrourceList(GetTranslationSourceListCallback callback)
{
    auto packet = new HttpCallbackPacket<GetTranslationSourceListCallbackType>();
    packet->type = httpGetTranslationSource;
    packet->callback = callback;
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/json;charset=utf-8");
    headers.insert("access_token", _token.toUtf8());
    headers.insert("client_type", "PC");
    headers.insert("language", GetLanguageToken());

    _httpAsync.Post("http://47.106.253.9:9101/api/config/getSourceLanguaueList", {}, headers, userParam);
}

void AiSound::GetTranslationDestList(GetTranslationDestListCallback callback)
{
    auto packet = new HttpCallbackPacket<GetTranslationSourceListCallbackType>();
    packet->type = httpGetTranslationDest;
    packet->callback = callback;
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/json;charset=utf-8");
    headers.insert("access_token", _token.toUtf8());
    headers.insert("client_type", "PC");
    headers.insert("language", GetLanguageToken());

    _httpAsync.Post("http://47.106.253.9:9101/api/config/getTargetLanguaueList", {}, headers, userParam);
}

void AiSound::GetTemplateMessage(int templateID, GetTmeplateMessageCallback callback)
{
    QJsonObject dataObj;
    dataObj.insert("templateId", templateID);

    auto packet = new HttpCallbackPacket<GetTmeplateMessageCallbackType>();
    packet->type = httpGetTemplateMessage;
    packet->callback = callback;
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/json;charset=utf-8");
    headers.insert("access_token", _token.toUtf8());
    headers.insert("client_type", "PC");
    headers.insert("language", GetLanguageToken());

    _httpAsync.Post("http://47.106.253.9:9101/api/config/getChatbotTemplate", dataObj, headers, userParam);
}

void AiSound::GetVoiceSpeaker(GetVoiceSpeakerCallback callback)
{
    auto packet = new HttpCallbackPacket<GetVoiceSpeakerCallbackType>();
    packet->type = httpGetVoiceSpeakerCallback;
    packet->callback = callback;
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/json;charset=utf-8");
    headers.insert("access_token", _token.toUtf8());
    headers.insert("client_type", "PC");
    headers.insert("language", GetLanguageToken());

    _httpAsync.Post("http://47.106.253.9:9101/api/config/getSpeakers", {}, headers, userParam);
}

void AiSound::GetPhoneRegionNumber(GetPhoneRegionNumberCallback callback)
{
    auto packet = new HttpCallbackPacket<GetPhoneRegionNumberCallbackType>();
    packet->type = httpGetPhoneRegionNumberCallback;
    packet->callback = callback;
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/json;charset=utf-8");
    headers.insert("access_token", _token.toUtf8());
    headers.insert("client_type", "PC");
    headers.insert("language", GetLanguageToken());

    _httpAsync.Post("http://47.106.253.9:9101/api/common/getMobileDialingList", {}, headers, userParam);
}

void AiSound::ForgetPassword(const QString& dialingCode, const QString& phoneEmail, const QString& password, const QString& rePassword, const QString& verifyCode, CommomCallback callback)
{
    QJsonObject dataObj;
    dataObj.insert("dialingCode", dialingCode);
    if (dialingCode == QString::fromLocal8Bit("+86"))
    {
        dataObj.insert("mobileNumber", phoneEmail);
    }
    else
    {
        dataObj.insert("mailAddress", phoneEmail);
    }
    dataObj.insert("password", password);
    dataObj.insert("rePassword", rePassword);
    dataObj.insert("rePassword", password);
    dataObj.insert("verifyCode", verifyCode);

    auto packet = new HttpCallbackPacket<CommomCallbackType>();
    packet->type = httpCommonCallback;
    packet->callback = callback;
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/json;charset=utf-8");
    headers.insert("access_token", _token.toUtf8());
    headers.insert("client_type", "PC");
    headers.insert("language", GetLanguageToken());

    _httpAsync.Post("http://47.106.253.9:9101/api/user/modify/password", dataObj, headers, userParam);
}

void AiSound::Register(const QString& dialingCode, const QString& phoneEmail, const QString& password, const QString& rePassword, const QString& recommendCode, const QString& verifyCode, CommomCallback callback)
{
    QJsonObject dataObj;
    dataObj.insert("dialingCode", dialingCode);
    if (dialingCode == QString::fromLocal8Bit("+86"))
    {
        dataObj.insert("mobileNumber", phoneEmail);
    }
    else
    {
        dataObj.insert("mailAddress", phoneEmail);
    }
    dataObj.insert("password", password);
    dataObj.insert("rePassword", rePassword);
    dataObj.insert("rePassword", password);
    if (!recommendCode.isEmpty())
    {
        dataObj.insert("recommendCode", recommendCode);
    }
    dataObj.insert("verifyCode", verifyCode);

    auto packet = new HttpCallbackPacket<CommomCallbackType>();
    packet->type = httpCommonCallback;
    packet->callback = callback;
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/json;charset=utf-8");
    headers.insert("access_token", _token.toUtf8());
    headers.insert("client_type", "PC");
    headers.insert("language", GetLanguageToken());

    _httpAsync.Post("http://47.106.253.9:9101/api/user/register", dataObj, headers, userParam);
}

void AiSound::SaveChat(const QString& name, SaveChatCallback callback)
{
    QJsonObject dataObj;
    dataObj.insert("chatName", name);
    dataObj.insert("conversationId", _chatBot.ConversationID());
    dataObj.insert("templateId", _chatBot.TemplateID());

    auto packet = new HttpCallbackPacket<SaveChatCallbackType>();
    packet->type = httpSaveChat;
    packet->callback = callback;
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/json;charset=utf-8");
    headers.insert("access_token", _token.toUtf8());
    headers.insert("client_type", "PC");
    headers.insert("language", GetLanguageToken());

    _httpAsync.Post("http://47.106.253.9:9101/api/chatbot/saveConversation", dataObj, headers, userParam);
}

void AiSound::ExportSound(const QString& msg, int ttsSpeaker, ExportSoundCallback callback)
{
    QJsonObject dataObj;
    dataObj.insert("message", msg);
    dataObj.insert("ttsSpeaker", ttsSpeaker);

    auto packet = new HttpCallbackPacket<ExportSoundCallbackType>();
    packet->type = httpExportSound;
    packet->callback = callback;
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/octet-stream");
    headers.insert("access_token", _token.toUtf8());
    headers.insert("client_type", "PC");
    headers.insert("language", GetLanguageToken());

    _httpAsync.Post("http://47.106.253.9:9102/business/voiceGen/export", dataObj, headers, userParam);
}

void AiSound::ShowLoginFrame()
{
    _wLoginFrame->show();
}

void AiSound::ShowRobotNavigation()
{
    if (!_logined) {
        return;
    }
    //_wTranslationSelect->show();
    _wLoginFrame->close();
    QDesktopWidget* deskWgt = QApplication::desktop();
    QRect availableRect = deskWgt->availableGeometry();
    _robotNaviga->move(availableRect.width()- _robotNaviga->width(), availableRect.height() - _robotNaviga->height());
    _robotNaviga->show();
}

void AiSound::slot_robot_nv_clicked(Navig_Type type)
{
    if (type == Navig_Type::Chat) {
        if (_robot_chat->isHidden())
        {
            _robot_chat->Flush();
            _robot_chat->Show();
        }
    }
    else if (type == Navig_Type::Voice) {
        if (_wTranslationMain->isHidden())
        {
            _wTranslationMain->Flush();
            _wTranslationMain->show();
        }
    }
    else if (type == Navig_Type::Speech) {
        if (_speech_ui->isHidden())
        {
            _speech_ui->Flush();
            _speech_ui->show();
        }
    }
    else if (type == Navig_Type::System_Set) {
        _set_main->Show();
    }
    else if (type == Navig_Type::Chat_Set) {
        _set_main->Show(1);
    }
    else if (type == Navig_Type::Quite) {

        if (_wLoginFrame)
        {
            delete _wLoginFrame;
            _wLoginFrame = nullptr;
        }

        if (_wTranslationMain)
        {
            delete _wTranslationMain;
            _wTranslationMain = nullptr;
        }

        if (_wConversationSuggestion)
        {
            delete _wConversationSuggestion;
            _wConversationSuggestion = nullptr;
        }

        if (_robot_chat) {
            delete _robot_chat;
            _robot_chat = nullptr;
        }
        if (_speech_ui) {
            delete _speech_ui;
            _speech_ui = nullptr;
        }
        if (_set_main) {
            delete _set_main;
            _set_main = nullptr;
        }

        Uninitialize();
        qApp->exit();
    }
}

void AiSound::ShowTranslationMainWindow()
{
    _wTranslationMain->show();
}

void AiSound::ShowVoiceCompositorMainWindow()
{
    //_speech_ui->SetLanguage(srcLan, destLan);
    _speech_ui->show();
}

bool AiSound::IsConversationSuggestionShow()
{
    return !_wConversationSuggestion->isHidden();
}

void AiSound::ShowRobotChat(const QString& chatId)
{
    _robot_chat->ShowRecord(chatId);
}

void AiSound::ShowConversationSuggestion(bool show)
{
    if (show)
    {
        _wConversationSuggestion->show();
    }
    else
    {
        _wConversationSuggestion->hide();
    }
}

WTranslationMain* AiSound::GetTranslationMainView()
{
    return _wTranslationMain;
}

WSpeechGenerationUi* AiSound::GetSpeechGenerationView()
{
    return _speech_ui;
}

RobotChatMainUI* AiSound::GetRobotChatView()
{
    return _robot_chat;
}

void AiSound::ShowTip(QWidget* parent, const QString& msg)
{
    WTip* tip = new WTip{ parent };
    tip->SetMessage(msg);
    tip->show();
}

Translation& AiSound::GetTranslation()
{
    return _translation;
}

VoiceCompositor& AiSound::GetVoiceCompositor()
{
    return _voiceCompositor;
}

ChatBot& AiSound::GetChatBot()
{
    return _chatBot;
}

const QString& AiSound::Token()
{
    return _token;
}

const std::vector<TranslationLanguage>& AiSound::GetTranslationSrourceListData()
{
    return _srcTranslationLanguage;
}

const std::vector<TranslationLanguage>& AiSound::GetTranslationDestListData()
{
    return _destTranslationLanguage;
}

const std::vector<VoiceData>& AiSound::GetVoiceData()
{
    return _voiceData;
}

const std::vector<PhoneRegionInfo>& AiSound::GetPhoneRegionInfo()
{
    return _phoneRegionData;
}

std::vector<QAudioDeviceInfo> AiSound::GetInputDeviceList()
{
    auto list = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

    auto it = list.begin();
    while (it != list.end())
    {
        auto& devInfo = *it;
        if (devInfo.realm() != "default")
        {
            it = list.erase(it);
        }
        else
        {
            it++;
        }
    }

    return list.toVector().toStdVector();
}

std::vector<QAudioDeviceInfo> AiSound::GetOutputDeviceList()
{
    auto list = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);

    auto it = list.begin();
    while (it != list.end())
    {
        auto& devInfo = *it;
        if (devInfo.realm() != "default")
        {
            it = list.erase(it);
        }
        else
        {
            it++;
        }
    }

    return list.toVector().toStdVector();
}

QAudioDeviceInfo AiSound::GetInputDeviceFormName(const QString& name, const QString& realm)
{
    auto list = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    auto it_find = std::find_if(list.begin(), list.end(), [&realm, &name](const QAudioDeviceInfo& info)
        {
            if (info.deviceName() == name &&
                info.realm() == realm)
            {
                return true;
            }
            else
            {
                return false;
            }
        });

    if (it_find != list.end())
    {
        return *it_find;
    }
    else
    {
        return QAudioDeviceInfo::defaultInputDevice();
    }
}

QAudioDeviceInfo AiSound::GetOutputDeviceFormName(const QString& name, const QString& realm)
{
    auto list = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    auto it_find = std::find_if(list.begin(), list.end(), [&realm, &name](const QAudioDeviceInfo& info)
        {
            if (info.deviceName() == name &&
                info.realm() == realm)
            {
                return true;
            }
            else
            {
                return false;
            }
        });

    if (it_find != list.end())
    {
        return *it_find;
    }
    else
    {
        return QAudioDeviceInfo::defaultOutputDevice();
    }
}

bool AiSound::IsDeviceVaild(bool input, const QString& name, const QString& realm)
{
    if (name.isEmpty())
    {
        return false;
    }

    QList<QAudioDeviceInfo> list;

    if (input)
    {
        list = std::move(QAudioDeviceInfo::availableDevices(QAudio::AudioInput));
    }
    else
    {
        list = std::move(QAudioDeviceInfo::availableDevices(QAudio::AudioOutput));
    }

    auto it_find = std::find_if(list.begin(), list.end(), [&realm, &name](const QAudioDeviceInfo& info)
        {
            if (info.deviceName() == name &&
                info.realm() == realm)
            {
                return true;
            }
            else
            {
                return false;
            }
        });

    return it_find != list.end();
}

QString AiSound::GetVoiceLanguageName(int id)
{
    auto it = std::find_if(_voiceData.begin(), _voiceData.end(), [id](const VoiceData& data)
        {
            if (data.language == id)
            {
                return true;
            }
            else
            {
                return false;
            }
        });

    if (it != _voiceData.end())
    {
        return it->languageName;
    }
    else
    {
        return "";
    }
}

QString AiSound::GetVoiceSexName(int id)
{
    auto it = std::find_if(_voiceData.begin(), _voiceData.end(), [id](const VoiceData& data)
        {
            if (data.gender == id)
            {
                return true;
            }
            else
            {
                return false;
            }
        });

    if (it != _voiceData.end())
    {
        return it->genderName;
    }
    else
    {
        return "";
    }
}

std::vector<int> AiSound::GetVoiceLanguage()
{
    std::vector<int> ret;
    std::set<int> st;
    for (auto& data : _voiceData)
    {
        st.insert(data.language);
    }

    for (auto& elem : st)
    {
        ret.push_back(elem);
    }

    return ret;
}

std::vector<int> AiSound::GetVoiceSex(int language)
{
    std::vector<int> ret;
    std::set<int> st;
    for (auto& data : _voiceData)
    {
        if (language != -1)
        {
            if (data.language == language)
            {
                st.insert(data.gender);
            }
        }
        else
        {
            st.insert(data.gender);
        }
    }

    for (auto& elem : st)
    {
        ret.push_back(elem);
    }

    return ret;
}

std::vector<int> AiSound::GetVoiceName(int language, int sex)
{
    std::vector<int> ret;
    std::set<int> st;
    for (auto& data : _voiceData)
    {
        if (language != -1)
        {
            if (data.language == language)
            {
                if (sex == -1)
                {
                    st.insert(data.id);
                }
                else
                {
                    if (data.gender == sex)
                    {
                        st.insert(data.id);
                    }
                }
            }
        }
        else
        {
            if (sex == -1)
            {
                st.insert(data.id);
            }
            else
            {
                if (data.gender == sex)
                {
                    st.insert(data.id);
                }
            }
        }
    }

    for (auto& elem : st)
    {
        ret.push_back(elem);
    }

    return ret;
}

bool AiSound::GetVoiceData(int id, VoiceData& findData)
{
    for (auto& data : _voiceData)
    {
        if (data.id == id)
        {
            findData = data;
            return true;
        }
    }

    return false;
}

void AiSound::SwitchLanguage(LanguageType type)
{
    _sysLanguage = type;

    auto app = static_cast<QApplication*>(QCoreApplication::instance());
    switch (type)
    {
        case LanguageType::EN:
            app->installTranslator(&_enTs);
            SETTING.setCurLanguage(0);
        break;

        case LanguageType::CHS:
            app->installTranslator(&_chsTs);
            SETTING.setCurLanguage(1);
        break;
    }
}

QString AiSound::GetLanguageToken()
{
    switch (_sysLanguage)
    {
    case LanguageType::EN:
        return "en_us";

    case LanguageType::CHS:
        return "zh_cn";
    }

    return "en_us";
}

LanguageType AiSound::GetSystemLanguage()
{
    return _sysLanguage;
}

void AiSound::playVoiceMp3(const QString& url)
{
    _audio_play->playUrl(url);
}

void AiSound::HttpCallbackDispatch(HttpAsync::HttpResult result, int code, const QByteArray& content, QVariant userParam)
{
    HttpCallbackPacketRaw* packetRaw = userParam.value<HttpCallbackPacketRaw*>();

    QJsonParseError err_rpt;
    auto document = QJsonDocument::fromJson(content, &err_rpt);

    switch (packetRaw->type)
    {
        case httpPasswordLogin:
        {
            int code = document["code"].toInt();
            QString token = document["data"]["accessToken"].toString();
            QString msg = document["msg"].toString();

            if (code == 200)
            {
                _logined = true;
            }

            auto packet = dynamic_cast<HttpCallbackPacket<LoginCallbackType>*>(packetRaw);
            UserLoginCallbackInternal(code, msg, token);
            packet->callback(code, msg, token);
        }
        break;

        case httpGetVerifyCode:
        {
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();
            QString img = document["data"]["img"].toString();
            QString uuid = document["data"]["uuid"].toString();

            auto packet = dynamic_cast<HttpCallbackPacket<GetVerifyCodeCallbackType>*>(packetRaw);
            packet->callback(code, msg, img, uuid);
        }
        break;

        case httpRegister:
        {
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();

            auto packet = dynamic_cast<HttpCallbackPacket<RegisterCallbackType>*>(packetRaw);
            packet->callback(code, msg);
        }
        break;

        case httpSendVerifyCode:
        {
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();

            auto packet = dynamic_cast<HttpCallbackPacket<SendVerifyCodeCallbackType>*>(packetRaw);
            packet->callback(code, msg);
        }
        break;

        case httpGetTranslationSource:
        {
            _srcTranslationLanguage.clear();

            int code = document["code"].toInt();
            QString msg = document["msg"].toString();
            auto data = document["data"].toArray();

            for (const auto& it : data)
            {
                TranslationLanguage lan;
                auto obj = it.toObject();
                lan.language = obj["language"].toString();
                lan.name = obj["name"].toString();
                lan.nameEn = obj["nameEn"].toString();
                _srcTranslationLanguage.push_back(lan);
            }

            auto packet = dynamic_cast<HttpCallbackPacket<GetTranslationSourceListCallbackType>*>(packetRaw);
            packet->callback(code, msg, _srcTranslationLanguage);
        }
        break;

        case httpGetTranslationDest:
        {
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();
            auto data = document["data"].toArray();

            _destTranslationLanguage.clear();
            for (const auto& it : data)
            {
                TranslationLanguage lan;
                auto obj = it.toObject();
                lan.language = obj["language"].toString();
                lan.name = obj["name"].toString();
                lan.nameEn = obj["nameEn"].toString();
                _destTranslationLanguage.push_back(lan);
            }

            auto packet = dynamic_cast<HttpCallbackPacket<GetTranslationDestListCallbackType>*>(packetRaw);
            packet->callback(code, msg, _destTranslationLanguage);
        }
        break;

        case httpGetVoiceSpeakerCallback:
        {
            _voiceData.clear();

            int code = document["code"].toInt();
            QString msg = document["msg"].toString();

            auto data = document["data"].toArray();
            for (const auto& it : data)
            {
                VoiceData data;
                auto obj = it.toObject();
                data.id = obj["id"].toInt();
                data.voiceCode = obj["voiceCode"].toInt();
                data.name = obj["name"].toString();
                data.language = obj["language"].toInt();
                data.languageName = obj["languageName"].toString();
                data.gender = obj["gender"].toInt();
                data.genderName = obj["genderName"].toString();

                _voiceData.push_back(data);
            }

            auto packet = dynamic_cast<HttpCallbackPacket<GetVoiceSpeakerCallbackType>*>(packetRaw);
            packet->callback(code, msg, _voiceData);
        }
        break;

        case httpGetPhoneRegionNumberCallback:
        {
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();

            std::vector<PhoneRegionInfo> regionData;
            auto data = document["data"].toArray();
            for (const auto& it : data)
            {
                PhoneRegionInfo data;
                auto obj = it.toObject();
                data.name = obj["name"].toString();
                data.dialingCode = obj["dialingCode"].toString();
                data.abb = obj["abb"].toString();
                regionData.push_back(std::move(data));
            }

            auto packet = dynamic_cast<HttpCallbackPacket<GetPhoneRegionNumberCallbackType>*>(packetRaw);
            packet->callback(code, msg, std::move(regionData));
        }
        break;

        case httpCommonCallback:
        {
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();

            auto packet = dynamic_cast<HttpCallbackPacket<CommomCallbackType>*>(packetRaw);
            packet->callback(code, msg);
        }
        break;

        case httpSaveChat:
        {
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();

            auto current = document["data"]["current"].toInt();
            auto pages = document["data"]["pages"].toInt();
            auto records = document["data"]["records"].toInt();
            auto size = document["data"]["size"].toInt();
            auto total = document["data"]["total"].toInt();

            auto packet = dynamic_cast<HttpCallbackPacket<SaveChatCallbackType>*>(packetRaw);
            packet->callback(code, msg, current, pages, records, size, total);
        }
        break;

        case httpExportSound:
        {
            auto packet = dynamic_cast<HttpCallbackPacket<ExportSoundCallbackType>*>(packetRaw);
            packet->callback(content);
        }
        break;

        case httpGetTemplateMessage:
        {
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();

            auto description = document["data"]["description"].toString();
            auto id = document["data"]["id"].toInt();
            auto initialMessage = document["data"]["initialMessage"].toString();
            auto isRecommend = document["data"]["isRecommend"].toInt() == 1;
            auto name = document["data"]["name"].toString();
            auto type = document["data"]["type"].toInt();

            auto packet = dynamic_cast<HttpCallbackPacket<GetTmeplateMessageCallbackType>*>(packetRaw);
            packet->callback(code, msg, description, id, initialMessage, isRecommend, name, type);
        }
        break;

        default:
        break;
    }

    delete packetRaw;
}

void AiSound::UserLoginCallbackInternal(int code, const QString& msg, const QString& token)
{
    if (code == 200)
    {
        _token = token;
        _heartBeatId = startTimer(5000);
    }
}

bool AiSound::AiFunctionRunning()
{
    return _translation.IsRunning() | _voiceCompositor.IsRunning() | _chatBot.IsRunning();
}

void AiSound::timerEvent(QTimerEvent* event)
{
    auto id = event->timerId();
    if (id == _heartBeatId)
    {
        HeartBeat();
    }
}

void AiSound::HeartBeat()
{
    auto packet = new HttpCallbackPacket<CommomCallbackType>();
    packet->type = httpCommonCallback;
    packet->callback = std::bind(&AiSound::HeartBeatCallback, this, std::placeholders::_1, std::placeholders::_2);
    QVariant userParam = QVariant::fromValue(static_cast<HttpCallbackPacketRaw*>(packet));

    QMap<QString, QString> headers;
    headers.insert("Content-Type", "application/json;charset=utf-8");
    headers.insert("access_token", _token.toUtf8());
    headers.insert("client_type", "PC");

    _httpAsync.Post("http://47.106.253.9:9101/api/user/heartbeat", {}, headers, userParam);
}

void AiSound::HeartBeatCallback(int code, const QString& msg)
{
    if (code == 20011)
    {
        QuitApp();
    }
}

void AiSound::QuitApp()
{
    _logined = false;

    _token.clear();
    killTimer(_heartBeatId);
    _heartBeatId = -1;
    _wLoginFrame->show();
    _robotNaviga->hide();

    _wTranslationMain->close();
    _wTranslationMain->Clear();
    _robot_chat->close();
    _robot_chat->Clear();
    _speech_ui->close();
    _speech_ui->Clear();

    _wConversationSuggestion->hide();
    _wConversationSuggestion->Clear();
    _robotNaviga->hide();
    _set_main->hide();

    ShowTip(_wLoginFrame, tr("Your account has already been logged in elsewhere."));
}
