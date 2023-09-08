#include "WSpeechGenerationUi.h"
#include "AiSound.h"

#include <QPainter>
#include <QListView>
#include <QPainterPath>

WSpeechGenerationUi::WSpeechGenerationUi(QWidget *parent)
    : FrameLessWidget(parent)
{
    ui.setupUi(this);
    this->setWidgetType(true, DragType::Drag_Null, false);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint | Qt::SubWindow);

    _simTrans = new WSimTrans{ this };
    _simTrans->hide();

    ui.comboBox_lang->setView(new  QListView());
    ui.comboBox_lang->addItem("English");
    ui.comboBox_lang->addItem("Chinese");

    ui.comboBox_sex->setView(new  QListView());
    ui.comboBox_sex->addItem("Male");
    ui.comboBox_sex->addItem("Woman");

    ui.comboBox_vector->setView(new  QListView());

    auto& ins = AiSound::GetInstance();
    auto& voiceCompositor = ins.GetVoiceCompositor();

    //connect(ui.pb_simTrans, &QPushButton::clicked, this, &WSpeechGenerationUi::SimTransClicked);
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

    //ui.speechEffect->Play(true);
    //ui.speechEffect->StartTimer(true);
}

//void WSpeechGenerationUi::SimTransClicked()
//{
//    if (_simTrans->isHidden())
//    {
//        _simTrans->show();
//    }
//    else
//    {
//        _simTrans->hide();
//    }
//}

void WSpeechGenerationUi::CloseClicked()
{
    AiSound::GetInstance().GetVoiceCompositor().Disconnect();
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

    AiSound::GetInstance().GetVoiceCompositor().Connect(token, _srcLan.language, _destLan.language, name, true);
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
