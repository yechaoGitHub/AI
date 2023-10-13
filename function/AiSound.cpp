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

const std::map<int, std::pair<QString, QString>> MP_ID_TO_LANG =
{
    {1, {QString::fromLocal8Bit("普通话"), QString::fromLocal8Bit("Mandarin")}},
    {2, {QString::fromLocal8Bit("英话"), QString::fromLocal8Bit("English")}},
    {3, {QString::fromLocal8Bit("日话"), QString::fromLocal8Bit("Japanese")}},
    {4, {QString::fromLocal8Bit("阿拉伯话"), QString::fromLocal8Bit("Arabic")}},
    {5, {QString::fromLocal8Bit("法话"), QString::fromLocal8Bit("French")}},
    {6, {QString::fromLocal8Bit("俄话"), QString::fromLocal8Bit("Russian")}},
    {7, {QString::fromLocal8Bit("西班牙话"), QString::fromLocal8Bit("Spanish")}},
    {8, {QString::fromLocal8Bit("越南话"), QString::fromLocal8Bit("Vietnamese")}},
    {9, {QString::fromLocal8Bit("泰话"), QString::fromLocal8Bit("Thai")}},
    {10, {QString::fromLocal8Bit("马来话"), QString::fromLocal8Bit("Malay")}},
    {11, {QString::fromLocal8Bit("德话"), QString::fromLocal8Bit("German")}},
    {12, {QString::fromLocal8Bit("土耳其话"), QString::fromLocal8Bit("Turkish")}},
    {13, {QString::fromLocal8Bit("韩语话"), QString::fromLocal8Bit("Korean")}},
    {14, {QString::fromLocal8Bit("葡萄牙话"), QString::fromLocal8Bit("Portuguese")}},
};

const std::map<int, std::pair<QString, QString>> MP_ID_TO_SEX =
{
    {0, {QString::fromLocal8Bit("其他"), QString::fromLocal8Bit("Other")}},
    {1, {QString::fromLocal8Bit("男"), QString::fromLocal8Bit("Man")}},
    {2, {QString::fromLocal8Bit("女"), QString::fromLocal8Bit("Woman")}},
};

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
    QString token = SETTING.getToken();
    if (token.isEmpty()) {
        return;
    }
    if (type == 1) {
        _wTranslationMain->show();
    }
    else if (type == 2) {
        _speech_ui->show();
    }
}

//加上Language
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

    _httpAsync.Post("http://47.106.253.9:9101/api/config/getTargetLanguaueList", {}, headers, userParam);
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

    _httpAsync.Post("http://47.106.253.9:9102/business/voiceGen/export", dataObj, headers, userParam);
}

void AiSound::ShowLoginFrame()
{
    _wLoginFrame->show();
}

void AiSound::ShowRobotNavigation()
{
    //_wTranslationSelect->show();
    _wLoginFrame->close();
    QDesktopWidget* deskWgt = QApplication::desktop();
    QRect availableRect = deskWgt->availableGeometry();
    _robotNaviga->move(availableRect.width()- _robotNaviga->width(), availableRect.height() - _robotNaviga->height());
    _robotNaviga->show();
}

void AiSound::slot_robot_nv_clicked(Navig_Type type)
{
    if (AiFunctionRunning())
    {
        return;
    }

    if (type == Navig_Type::Chat) {
        if (_robot_chat->isHidden())
        {
            _robot_chat->Show();
        }
    }
    else if (type == Navig_Type::Voice) {
        if (_wTranslationMain->isHidden())
        {
            _wTranslationMain->SetLanguage(GetTranslationSrourceListData(), GetTranslationDestListData());
            _wTranslationMain->show();
        }
    }
    else if (type == Navig_Type::Speech) {
        if (_speech_ui->isHidden())
        {
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
        if (_robotNaviga) {
            delete _robotNaviga;
            _robotNaviga = nullptr;
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
    return list.toVector().toStdVector();
}

std::vector<QAudioDeviceInfo> AiSound::GetOutputDeviceList()
{
    auto list = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    return list.toVector().toStdVector();
}

const std::map<int, std::pair<QString, QString>>& AiSound::GetIDLanguageMap()
{
    return MP_ID_TO_LANG;
}

const std::map<int, std::pair<QString, QString>>& AiSound::GetIDSexMap()
{
    return MP_ID_TO_SEX;
}

QString AiSound::GetVoiceLanguageName(int id)
{
    auto it = MP_ID_TO_LANG.find(id);
    if (it != MP_ID_TO_LANG.end())
    {
        return it->second.first;
    }
    else
    {
        return "";
    }
}

QString AiSound::GetVoiceSexName(int id)
{
    auto it = MP_ID_TO_SEX.find(id);
    if (it != MP_ID_TO_SEX.end())
    {
        return it->second.first;
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
    if (type == _sysLanguage)
    {
        return;
    }

    _sysLanguage = type;

    auto app = static_cast<QApplication*>(QCoreApplication::instance());
    switch (type)
    {
        case LanguageType::EN:
            app->installTranslator(&_enTs);
        break;

        case LanguageType::CHS:
            app->installTranslator(&_chsTs);
        break;
    }
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
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();
            auto data = document["data"].toArray();

            std::vector<TranslationLanguage> vecLan;
            for (const auto& it : data)
            {
                TranslationLanguage lan;
                auto obj = it.toObject();
                lan.language = obj["language"].toString();
                lan.name = obj["name"].toString();
                lan.nameEn = obj["nameEn"].toString();
                vecLan.push_back(lan);
            }

            auto packet = dynamic_cast<HttpCallbackPacket<GetTranslationSourceListCallbackType>*>(packetRaw);
            packet->callback(code, msg, std::move(vecLan));
        }
        break;

        case httpGetTranslationDest:
        {
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();
            auto data = document["data"].toArray();

            std::vector<TranslationLanguage> vecLan;
            for (const auto& it : data)
            {
                TranslationLanguage lan;
                auto obj = it.toObject();
                lan.language = obj["languaue"].toString();
                lan.name = obj["name"].toString();
                lan.nameEn = obj["nameEn"].toString();
                vecLan.push_back(lan);
            }

            auto packet = dynamic_cast<HttpCallbackPacket<GetTranslationDestListCallbackType>*>(packetRaw);
            packet->callback(code, msg, std::move(vecLan));
        }
        break;

        case httpGetVoiceSpeakerCallback:
        {
            int code = document["code"].toInt();
            QString msg = document["msg"].toString();

            std::vector<VoiceData> vecVoiceData;
            auto data = document["data"].toArray();
            for (const auto& it : data)
            {
                VoiceData data;
                auto obj = it.toObject();
                data.id = obj["id"].toInt();
                data.voiceCode = obj["voiceCode"].toInt();
                data.name = obj["name"].toString();
                data.language = obj["language"].toInt();
                data.gender = obj["gender"].toInt();
                vecVoiceData.push_back(data);
            }

            auto packet = dynamic_cast<HttpCallbackPacket<GetVoiceSpeakerCallbackType>*>(packetRaw);
            packet->callback(code, msg, std::move(vecVoiceData));
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

        default:
        break;
    }

    delete packetRaw;
}

void AiSound::FetchAppData()
{
    GetTranslationSrourceList([this](int code, const QString& msg, std::vector<TranslationLanguage> languageList)
        {
            if (code == 200)
            {
                _srcTranslationLanguage = std::move(languageList);
            }
        });

    GetTranslationDestList([this](int code, const QString& msg, std::vector<TranslationLanguage> languageList)
        {
            if (code == 200)
            {
                _destTranslationLanguage = std::move(languageList);
            }
        });

    GetVoiceSpeaker([this](int code, const QString& msg, std::vector<VoiceData> vecVoiceData)
        {
            if (code == 200)
            {
                _voiceData = std::move(vecVoiceData);
            }
        });
}

void AiSound::UserLoginCallbackInternal(int code, const QString& msg, const QString& token)
{
    if (code == 200)
    {
        _token = token;
        SETTING.setToken(token);
        FetchAppData();
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
}
