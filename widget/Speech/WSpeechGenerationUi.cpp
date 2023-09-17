#include "WSpeechGenerationUi.h"
#include "AiSound.h"
#include "base/GlobalSetting.h"

#include <set>

#include <QFileDialog>
#include <QPainter>
#include <QListView>
#include <QPainterPath>
#include <QStandardPaths>
#include <QListView>

const QString playStyle = "background-origin:content;\
background-image:url(:/QtTest/icon/Speech/pause.png);\
background-position:left;\
background-repeat:no-repeat;\
padding-left:39px;\
padding-right:25px;\
text-align:center;\
font-size:14px;\
font-weight:700;\
color:#FFFFFF;\
border:1px solid;\
border-radius:8px;\
border-color:#FFFFFF;";

const QString stopStyle = "background-origin:content;\
background-image:url(:/QtTest/icon/stop.png);\
background-position:left;\
background-repeat:no-repeat;\
padding-left:39px;\
padding-right:25px;\
text-align:center;\
font-size:14px;\
font-weight:700;\
color:#FFFFFF;\
border:1px solid;\
border-radius:8px;\
border-color:#FFFFFF;";

WSpeechGenerationUi::WSpeechGenerationUi(QWidget* parent)
    : FrameLessWidget(parent)
{
    ui.setupUi(this);
    this->setWidgetType(true, DragType::Drag_Null, false);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);

    ui.comboBox_lang->setView(new QListView());
    ui.comboBox_lang->addItem("English");
    ui.comboBox_lang->addItem("Chinese");

    ui.comboBox_sex->setView(new QListView());
    ui.comboBox_sex->addItem("Male");
    ui.comboBox_sex->addItem("Woman");

    ui.comboBox_vector->setView(new QListView());

    auto& ins = AiSound::GetInstance();
    auto& voiceCompositor = ins.GetVoiceCompositor();

    connect(ui.pb_start, &QPushButton::clicked, this, &WSpeechGenerationUi::StartClicked);
    connect(ui.pb_close, &QPushButton::clicked, this, &WSpeechGenerationUi::CloseClicked);
    connect(ui.pb_send, &QPushButton::clicked, this, &WSpeechGenerationUi::SendClicked);
    connect(ui.pb_export, &QPushButton::clicked, this, &WSpeechGenerationUi::ExportClicked);
    connect(ui.comboBox_lang, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WSpeechGenerationUi::LanguageIndexChanged);
    connect(ui.comboBox_sex, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WSpeechGenerationUi::SexIndexChanged);
    connect(ui.comboBox_vector, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WSpeechGenerationUi::NameIndexChanged);

    connect(&voiceCompositor, &VoiceCompositor::translationReceived, this, &WSpeechGenerationUi::TranslationReceived);
}

WSpeechGenerationUi::~WSpeechGenerationUi()
{}

void WSpeechGenerationUi::SetLanguage(const TranslationLanguage &srcLan, const TranslationLanguage &destLan)
{
    _srcLan = srcLan;
    _destLan = destLan;
}

void WSpeechGenerationUi::paintEvent(QPaintEvent* event)
{
    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(this->rect(), 16, 16);
    painter.fillPath(path, QBrush(QColor(0, 0, 0, 204)));
}

void WSpeechGenerationUi::showEvent(QShowEvent* event)
{
    auto& ins = AiSound::GetInstance();
    ui.comboBox_lang->clear();
    ui.comboBox_sex->clear();
    ui.comboBox_vector->clear();

    auto&& lanData = ins.GetVoiceLanguage();
    for (auto& data : lanData)
    {
        auto name = ins.GetVoiceLanguageName(data);
        if (!name.isEmpty())
        {
            ui.comboBox_lang->addItem(name, data);
        }
    }

    auto&& sexData = ins.GetVoiceSex(-1);
    for (auto& data : sexData)
    {
        if (ui.comboBox_sex->findData(data) == -1)
        {
            auto name = ins.GetVoiceSexName(data);
            if (!name.isEmpty())
            {
                ui.comboBox_sex->addItem(name, data);
            }
        }
    }

    auto langId = ui.comboBox_lang->currentData().toInt();
    auto sexId = ui.comboBox_sex->currentData().toInt();
    if (langId == -1 && sexId == -1)
    {
        return;
    }
    auto&& userData = ins.GetVoiceName(langId, sexId);
    for (auto data : userData)
    {
        VoiceData voiceData;
        if (ins.GetVoiceData(data, voiceData))
        {
            if (ui.comboBox_vector->findData(voiceData.id) == -1)
            {
                ui.comboBox_vector->addItem(voiceData.name, voiceData.id);
            }
        }
    }

    ui.vcEffectTimer->Clear();
}

void WSpeechGenerationUi::closeEvent(QCloseEvent* event)
{
    auto& vc = AiSound::GetInstance().GetVoiceCompositor();
    if (vc.IsRunning())
    {
        vc.Disconnect();
    }
}

void WSpeechGenerationUi::LanguageIndexChanged(int index)
{
    auto& ins = AiSound::GetInstance();

    ui.comboBox_sex->clear();
    ui.comboBox_vector->clear();

    auto id = ui.comboBox_lang->currentData().toInt();
    if (id == -1)
    {
        return;
    }

    auto&& sexData = ins.GetVoiceSex(id);
    for (auto& data : sexData)
    {
        if (ui.comboBox_sex->findData(data) == -1)
        {
            auto name = ins.GetVoiceSexName(data);
            if (!name.isEmpty())
            {
                ui.comboBox_sex->addItem(name, data);
            }
        }
    }

    auto&& userData = ins.GetVoiceName(id, -1);
    for (auto data : userData)
    {
        VoiceData voiceData;
        if (ins.GetVoiceData(data, voiceData))
        {
            if (ui.comboBox_vector->findData(voiceData.id) == -1)
            {
                ui.comboBox_vector->addItem(voiceData.name, voiceData.id);
            }
        }
    }
}

void WSpeechGenerationUi::SexIndexChanged(int index)
{
    auto& ins = AiSound::GetInstance();

    ui.comboBox_vector->clear();

    auto langId = ui.comboBox_lang->currentData().toInt();
    auto sexId = ui.comboBox_sex->currentData().toInt();
    if (langId == -1 && sexId == -1)
    {
        return;
    }

    auto&& userData = ins.GetVoiceName(langId, sexId);
    for (auto data : userData)
    {
        VoiceData voiceData;
        if (ins.GetVoiceData(data, voiceData))
        {
            if (ui.comboBox_vector->findData(voiceData.id) == -1)
            {
                ui.comboBox_vector->addItem(voiceData.name, voiceData.id);
            }
        }
    }
}

void WSpeechGenerationUi::NameIndexChanged(int index)
{
    auto& ins = AiSound::GetInstance();

    auto id = ui.comboBox_vector->currentData().toInt();
    VoiceData data;
    if (!ins.GetVoiceData(id, data))
    {
        return;
    }

    auto in = ui.comboBox_sex->findData(data.gender);
    if (in != -1)
    {
        ui.comboBox_sex->setCurrentIndex(in);
    }

    in = ui.comboBox_lang->findData(data.language);
    if (in != -1)
    {
        ui.comboBox_lang->setCurrentIndex(in);
    }
}

void WSpeechGenerationUi::CloseClicked()
{
    close();
}

void WSpeechGenerationUi::StartClicked()
{
    auto& ins = AiSound::GetInstance();
    auto& vc = AiSound::GetInstance().GetVoiceCompositor();
    auto isSend = ui.cbAutoSend->isChecked();
    auto& token = ins.Token();

    if (vc.IsRunning())
    {
        vc.Disconnect();
        ui.vcEffectTimer->StartTimer(false);
        ui.pb_start->setStyleSheet(playStyle);
        ui.pb_start->setText(QString::fromLocal8Bit("Start"));
    }
    else
    {
        auto name = GetSelectSpeaker();
        if (name.isEmpty())
        {
            return;
        }

        vc.Connect(token, _srcLan.language, _destLan.language, name, isSend, SETTING.MicDeviceInfo(), SETTING.SpeakerDeviceInfo());
        ui.vcEffectTimer->StartTimer(true);
        ui.pb_start->setStyleSheet(stopStyle);
        ui.pb_start->setText(QString::fromLocal8Bit("Stop"));
    }
}

void WSpeechGenerationUi::SendClicked()
{
    auto&& text = ui.textEdit->toPlainText();
    AiSound::GetInstance().GetVoiceCompositor().SendMessage(text);
}

void WSpeechGenerationUi::ExportClicked()
{
    QString homeLocation = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    auto savePath = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("Save Mp3 File"), homeLocation, "mp3(*.mp3)");
    auto& vc = AiSound::GetInstance().GetVoiceCompositor();
    vc.SaveMp3(savePath);
}

void WSpeechGenerationUi::TranslationReceived(const QString& src, const QString& dst, int type)
{
    if (type == FIN)
    {
        ui.textEdit->setText(dst);
    }
}

QString WSpeechGenerationUi::GetSelectSpeaker()
{
    auto id = ui.comboBox_vector->currentData().toInt();
    if (id != -1)
    {
        auto& ins = AiSound::GetInstance();
        VoiceData data;
        if (ins.GetVoiceData(id, data))
        {
            return data.name;
        }
        else
        {
            return "";
        }
    }
    else
    {
        return "";
    }
}

