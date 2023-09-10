#include "WSpeechGenerationUi.h"
#include "AiSound.h"
#include "base/GlobalSetting.h"

#include <QFileDialog>
#include <QPainter>
#include <QListView>
#include <QPainterPath>
#include <QStandardPaths>

WSpeechGenerationUi::WSpeechGenerationUi(QWidget* parent)
    : FrameLessWidget(parent)
{
    ui.setupUi(this);
    this->setWidgetType(true, DragType::Drag_Null, false);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint | Qt::SubWindow);

    ui.comboBox_lang->setView(new  QListView());
    ui.comboBox_lang->addItem("English");
    ui.comboBox_lang->addItem("Chinese");

    ui.comboBox_sex->setView(new  QListView());
    ui.comboBox_sex->addItem("Male");
    ui.comboBox_sex->addItem("Woman");

    ui.comboBox_vector->setView(new  QListView());

    auto& ins = AiSound::GetInstance();
    auto& voiceCompositor = ins.GetVoiceCompositor();

    ui.pb_play->setIcon(QIcon{":/QtTest/icon/Speech/pause.png"});
    ui.pb_play->setIconSize(QSize{16, 16});

    connect(ui.pb_start, &QPushButton::clicked, this, &WSpeechGenerationUi::StartClicked);
    connect(ui.pb_close, &QPushButton::clicked, this, &WSpeechGenerationUi::CloseClicked);
    connect(ui.pb_send, &QPushButton::clicked, this, &WSpeechGenerationUi::SendClicked);
    connect(ui.pb_export, &QPushButton::clicked, this, &WSpeechGenerationUi::ExportClicked);
    connect(ui.pb_play, &QPushButton::clicked, this, &WSpeechGenerationUi::PlayClicked);
    connect(&voiceCompositor, &VoiceCompositor::translationReceived, this, &WSpeechGenerationUi::TranslationReceived);
    connect(&voiceCompositor, &VoiceCompositor::stateChanged, this, &WSpeechGenerationUi::VcStateChanged);
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
    for (auto data : ins.GetVoiceData())
    {
        ui.comboBox_vector->addItem(data.name);
    }

    ui.vcEffectTimer->Clear();
}

void WSpeechGenerationUi::closeEvent(QCloseEvent* event)
{
    AiSound::GetInstance().GetVoiceCompositor().Disconnect();
}

void WSpeechGenerationUi::CloseClicked()
{
    close();
}

void WSpeechGenerationUi::StartClicked()
{
    auto index = ui.comboBox_vector->currentIndex();
    if (index == -1)
    {
        return;
    }

    auto& ins = AiSound::GetInstance();
    auto& token = AiSound::GetInstance().Token();
    auto& name = ins.GetVoiceData()[index].voiceCode;
    auto isSend = ui.cbAutoSend->isChecked();

    AiSound::GetInstance().GetVoiceCompositor().Connect(token, _srcLan.language, _destLan.language, name, isSend, SETTING.MicDeviceInfo(), SETTING.SpeakerDeviceInfo());
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

void WSpeechGenerationUi::PlayClicked()
{
    auto& vc = AiSound::GetInstance().GetVoiceCompositor();
    if (vc.IsRunning())
    {
        if (vc.IsMicWorking())
        {
            vc.StopMic();
            ui.vcEffectTimer->StartTimer(false);
            ui.pb_play->setIcon(QIcon{":/QtTest/icon/Speech/pause.png"});
        }
        else
        {
            vc.StartMic();
            ui.vcEffectTimer->StartTimer(true);
            ui.pb_play->setIcon(QIcon{":/QtTest/icon/stop.png"});
        }
    }
}

void WSpeechGenerationUi::TranslationReceived(const QString& src, const QString& dst, int type)
{
    if (type == FIN)
    {
        ui.textEdit->setText(dst);
    }
}

void WSpeechGenerationUi::VcStateChanged()
{
    auto& vc = AiSound::GetInstance().GetVoiceCompositor();
    if (vc.IsRunning())
    {
        if (vc.IsMicWorking())
        {
            ui.vcEffectTimer->StartTimer(true);
            ui.pb_play->setIcon(QIcon{ ":/QtTest/icon/stop.png" });
        }
        else
        {
            ui.vcEffectTimer->StartTimer(false);
            ui.pb_play->setIcon(QIcon{ ":/QtTest/icon/Speech/pause.png" });
        }
    }
}
