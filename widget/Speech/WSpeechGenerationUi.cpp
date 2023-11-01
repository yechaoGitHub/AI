#include "WSpeechGenerationUi.h"
#include "VoiceType.h"
#include "AiSound.h"
#include "base/GlobalSetting.h"

#include <set>

#include <QFileDialog>
#include <QPainter>
#include <QListView>
#include <QPainterPath>
#include <QStandardPaths>
#include <QListView>

WSpeechGenerationUi::WSpeechGenerationUi(QWidget* parent)
    : FrameLessWidget(parent)
{
    ui.setupUi(this);
    this->setWidgetType(true, DragType::Drag_Null, false);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);

    ui.comboBox_lang->setView(new QListView{});
    ui.comboBox_sex->setView(new QListView{});
    ui.cbFrom->setView(new QListView{});

    ui.comboBox_vector->setView(new QListView());

    auto& ins = AiSound::GetInstance();
    auto& voiceCompositor = ins.GetVoiceCompositor();

    connect(ui.pb_start, &QPushButton::clicked, this, &WSpeechGenerationUi::StartClicked);
    connect(ui.pb_min, &QPushButton::clicked, this, &WSpeechGenerationUi::MinClicked);
    connect(ui.pb_close, &QPushButton::clicked, this, &WSpeechGenerationUi::CloseClicked);
    connect(ui.pb_send, &QPushButton::clicked, this, &WSpeechGenerationUi::SendClicked);
    connect(ui.pb_export, &QPushButton::clicked, this, &WSpeechGenerationUi::ExportClicked);
    connect(ui.comboBox_lang, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WSpeechGenerationUi::LanguageIndexChanged);
    connect(ui.comboBox_sex, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WSpeechGenerationUi::SexIndexChanged);
    connect(ui.comboBox_vector, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WSpeechGenerationUi::NameIndexChanged);
    connect(&voiceCompositor, &VoiceCompositor::stateChanged, this, &WSpeechGenerationUi::VCStateChanged);
    connect(&voiceCompositor, &VoiceCompositor::translationReceived, this, &WSpeechGenerationUi::TranslationReceived);

    ui.pb_lock->setProperty("lock", false);
    ui.pb_lock->style()->unpolish(ui.pb_lock);

    ui.pb_send->setProperty("enable", true);
    ui.pb_send->style()->unpolish(ui.pb_send);

    SyncUI();
}

WSpeechGenerationUi::~WSpeechGenerationUi()
{}

void WSpeechGenerationUi::Clear()
{
    ui.comboBox_lang->clear();
    ui.comboBox_sex->clear();
    ui.comboBox_vector->clear();
    ui.cbFrom->clear();
    ui.textEdit->clear();
}

void WSpeechGenerationUi::Flush()
{
    auto& ins = AiSound::GetInstance();
    ui.comboBox_lang->clear();
    ui.comboBox_sex->clear();
    ui.comboBox_vector->clear();
    ui.cbFrom->clear();

    ui.vcEffectTimer->Clear();

    auto callback = [this](int code, const QString& msg, std::vector<VoiceData> vecVoiceData)
    {
        if (code != 200)
        {
            return;
        }

        auto& ins = AiSound::GetInstance();

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
    };

    ins.GetVoiceSpeaker(callback);

    auto srcCallback = [this](int code, const QString& msg, std::vector<TranslationLanguage> languageList)
    {
        if (code != 200)
        {
            return;
        }

        for (auto data : languageList)
        {
            ui.cbFrom->addItem(data.name);
        }
    };

    ins.GetTranslationSrourceList(srcCallback);

    SyncUI();
}

void WSpeechGenerationUi::on_pb_lock_clicked()
{
    _lock = !_lock;
    ui.pb_lock->setProperty("lock", _lock);
    ui.pb_lock->style()->unpolish(ui.pb_lock);
    Qt::WindowFlags flags = windowFlags();
    if (_lock) {
        this->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
    }
    else {
        this->setWindowFlags(flags ^ Qt::WindowStaysOnTopHint); // È¡ÏûÖÃ¶¥
    }
    show();
}

void WSpeechGenerationUi::paintEvent(QPaintEvent* event)
{
    auto bkColor = SETTING.getSpeechBkColor();

    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(this->rect(), 16, 16);
    painter.fillPath(path, QBrush{ bkColor });
}

void WSpeechGenerationUi::closeEvent(QCloseEvent* event)
{
    event->setAccepted(false);

    auto& vc = AiSound::GetInstance().GetVoiceCompositor();
    if (vc.IsRunning())
    {
        vc.Disconnect();
    }

    hide();
}

void WSpeechGenerationUi::timerEvent(QTimerEvent* event)
{
    ui.pb_send->setEnabled(true);
    killTimer(_time);
    _time = 0;

    ui.pb_send->setProperty("enable", true);
    ui.pb_send->style()->unpolish(ui.pb_send);
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

QString WSpeechGenerationUi::GetSelectSrcLanguage()
{
    auto index = ui.cbFrom->currentIndex();
    if (index < 0)
    {
        return "";
    }

    auto& ins = AiSound::GetInstance();
    auto&& listData = ins.GetTranslationSrourceListData();

    if (index >= listData.size())
    {
        return "";
    }
    else
    {
        return listData[index].language;
    }
}

void WSpeechGenerationUi::SyncUI()
{
    auto& ins = AiSound::GetInstance();
    auto& vc = AiSound::GetInstance().GetVoiceCompositor();
    if (vc.IsRunning())
    {
        ui.vcEffectTimer->StartTimer(true);
        ui.pb_start->setProperty("play", true);
        ui.pb_start->style()->unpolish(ui.pb_start);
        ui.pb_start->setText(tr("Stop"));
    }
    else
    {
        ui.vcEffectTimer->StartTimer(false);
        ui.pb_start->setProperty("play", false);
        ui.pb_start->style()->unpolish(ui.pb_start);
        ui.pb_start->setText(tr("Start"));
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
    }
    else
    {
        auto id = GetSelectSpeaker();
        if (id == -1)
        {
            return;
        }

        auto srcLanguage = GetSelectSrcLanguage();
        if (srcLanguage.isEmpty())
        {
            return;
        }


        auto mic = SETTING.getMicDeviceName();
        auto micDevInfo = ins.GetInputDeviceFormName(mic, "default");
        auto speaker = SETTING.getSpeakerDeviceName();
        auto speakerDevInfo = ins.GetOutputDeviceFormName(speaker, "default");

        vc.Connect(token, srcLanguage, id, isSend, micDevInfo, speakerDevInfo);
    }
}

void WSpeechGenerationUi::MinClicked()
{
    showMinimized();
}

void WSpeechGenerationUi::SendClicked()
{
    if (AiSound::GetInstance().GetVoiceCompositor().IsRunning())
    {
        auto&& text = ui.textEdit->toPlainText();
        AiSound::GetInstance().GetVoiceCompositor().SendChatMessage(text);

        _time = startTimer(3000);

        ui.pb_send->setEnabled(false);

        ui.pb_send->setProperty("enable", false);
        ui.pb_send->style()->unpolish(ui.pb_send);
    }
}

void WSpeechGenerationUi::ExportClicked()
{
    auto& ins = AiSound::GetInstance();
    int id = GetSelectSpeaker();
    auto&& text = ui.textEdit->toPlainText();
    if (text.isEmpty())
    {
        return;
    }

    QString homeLocation = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    auto savePath = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("Save Mp3 File"), homeLocation, "mp3(*.mp3)");

    auto callback = [savePath](const QByteArray& content)
    {
        QFile file{ savePath, };
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            file.write(content);
            file.close();
        }
    };

    ins.ExportSound(text, id, callback);
}

void WSpeechGenerationUi::VCStateChanged(int state)
{
    switch (state)
    {
        case VC_RUNING:
            ui.vcEffectTimer->StartTimer(true);
            ui.pb_start->setProperty("play", true);
            ui.pb_start->style()->unpolish(ui.pb_start);
            ui.pb_start->setText(tr("Stop"));
        break;

        case VC_STOP:
            ui.vcEffectTimer->StartTimer(false);
            ui.pb_start->setProperty("play", false);
            ui.pb_start->style()->unpolish(ui.pb_start);
            ui.pb_start->setText(tr("Start"));
            ui.textEdit->clear();
        break;
    }
}

void WSpeechGenerationUi::TranslationReceived(const QString& src, const QString& dst, int type)
{
    if (type == FIN)
    {
        ui.textEdit->setText(src);
    }
}

int WSpeechGenerationUi::GetSelectSpeaker()
{
    auto id = ui.comboBox_vector->currentData().toInt();
    if (id != -1)
    {
        auto& ins = AiSound::GetInstance();
        VoiceData data;
        if (ins.GetVoiceData(id, data))
        {
            return data.voiceCode;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

