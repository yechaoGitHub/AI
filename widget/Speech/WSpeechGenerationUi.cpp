#include "WSpeechGenerationUi.h"
#include "AiSound.h"
#include <QPainter>
#include <QListView>

WSpeechGenerationUi::WSpeechGenerationUi(QWidget *parent)
    : FrameLessWidget(parent)
{
    ui.setupUi(this);
    this->setWidgetType(true, DragType::Drag_Null, false);
    setAttribute(Qt::WA_TranslucentBackground);

    ui.comboBox_lang->setView(new  QListView());
    ui.comboBox_lang->addItem("English");
    ui.comboBox_lang->addItem("Chinese");

    ui.comboBox_sex->setView(new  QListView());
    ui.comboBox_sex->addItem("Male");
    ui.comboBox_sex->addItem("Woman");

    ui.comboBox_vector->setView(new  QListView());

    auto& ins = AiSound::GetInstance();
    auto& voiceCompositor = ins.GetVoiceCompositor();

    connect(ui.pb_start, &QPushButton::clicked, this, &WSpeechGenerationUi::StartClicked);
    connect(ui.pb_close, &QPushButton::clicked, this, &WSpeechGenerationUi::CloseClicked);
    connect(ui.pb_send, &QPushButton::clicked, this, &WSpeechGenerationUi::SendClicked);
    connect(&voiceCompositor, &VoiceCompositor::translationReceived, this, &WSpeechGenerationUi::TranslationReceived);
}

WSpeechGenerationUi::~WSpeechGenerationUi()
{}

void WSpeechGenerationUi::SetLanguage(const TranslationLanguage &srcLan, const TranslationLanguage &destLan)
{
    _srcLan = srcLan;
    _destLan = destLan;
}

void WSpeechGenerationUi::on_pb_close_clicked()
{
    close();
}

void WSpeechGenerationUi::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    QRect frameRect = rect();
    p.setPen(QColor(0, 0, 0, 204));
    p.setBrush(QColor(0, 0, 0, 204));
    p.drawRoundRect(frameRect, 6, 6);
}

void WSpeechGenerationUi::showEvent(QShowEvent* event)
{
    auto& ins = AiSound::GetInstance();
    for (auto data : ins.GetVoiceData())
    {
        ui.comboBox_vector->addItem(data.name);
    }
}

void WSpeechGenerationUi::CloseClicked()
{
    AiSound::GetInstance().GetVoiceCompositor().Disconnect();
}

void WSpeechGenerationUi::StartClicked()
{
    auto it = ui.checkBox->checkState();
    auto index = ui.comboBox_vector->currentIndex();
    if (index == -1)
    {
        return;
    }

    auto& ins = AiSound::GetInstance();
    auto& token = AiSound::GetInstance().Token();
    auto& name = ins.GetVoiceData()[index].voiceCode;
    auto isAutoSend = ui.checkBox->isChecked();

    AiSound::GetInstance().GetVoiceCompositor().Connect(token, _srcLan.language, _destLan.language, name, isAutoSend);
}

void WSpeechGenerationUi::SendClicked()
{
    auto&& text = ui.textEdit->toPlainText();
    AiSound::GetInstance().GetVoiceCompositor().SendMessage(text);
}

void WSpeechGenerationUi::TranslationReceived(const QString& src, const QString& dst, int type)
{
    if (type == FIN)
    {
        ui.textEdit->setText(dst);
    }
}
