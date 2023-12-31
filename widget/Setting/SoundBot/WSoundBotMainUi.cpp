#include "WSoundBotMainUi.h"
#include "base/GlobalSetting.h"


WSoundBotMainUi::WSoundBotMainUi(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.pb_sound_page1->initBar(tr("Sound Source"), WNavbarButton::BarType::Sound_Page1);
    ui.pb_sound_page2->initBar(tr("Translation"), WNavbarButton::BarType::Sound_Page2);
    ui.pb_sound_page3->initBar(tr("Voice Library"), WNavbarButton::BarType::Sound_Page3);
    ui.pb_sound_page4->initBar(tr("My Voice"), WNavbarButton::BarType::Sound_Page4);
    ui.pb_sound_page5->initBar(tr("Speech Synthesis"), WNavbarButton::BarType::Sound_Page5);
    ui.pb_sound_page6->initBar(tr("Translation History"), WNavbarButton::BarType::Sound_Page6);
    changeSelectBtn(WNavbarButton::BarType::Sound_Page1);
    ui.pb_sound_page5->hide();
    connect(ui.checkBox, &QCheckBox::clicked, this, [=] {
        if (ui.checkBox->checkState() == Qt::CheckState::Checked) {
            SETTING.setSoundBot(true);
        }
        else {
            SETTING.setSoundBot(false);
        }
        });
    ui.checkBox->setVisible(false);
}

WSoundBotMainUi::~WSoundBotMainUi()
{}

void WSoundBotMainUi::initCheck()
{
    bool ret = SETTING.getSoundBot();
    ui.checkBox->setChecked(ret);
    ui.pb_sound_page1->click();
}

void WSoundBotMainUi::slot_sound_back()
{
    emit sig_soundBack();
}

void WSoundBotMainUi::on_pb_sound_page1_clicked()
{
    changeSelectBtn(WNavbarButton::BarType::Sound_Page1);
    ui.stackedWidget->setCurrentIndex(0);
}

void WSoundBotMainUi::on_pb_sound_page2_clicked()
{
    changeSelectBtn(WNavbarButton::BarType::Sound_Page2);
    ui.stackedWidget->setCurrentIndex(1);
}

void WSoundBotMainUi::on_pb_sound_page3_clicked()
{
    changeSelectBtn(WNavbarButton::BarType::Sound_Page3);
    ui.stackedWidget->setCurrentIndex(2);
    ui.voice_lib_page->setSel();
}

void WSoundBotMainUi::on_pb_sound_page4_clicked()
{
    changeSelectBtn(WNavbarButton::BarType::Sound_Page4);
    ui.stackedWidget->setCurrentIndex(3);
    ui.myVoice_page->initMyVoice();
}

void WSoundBotMainUi::on_pb_sound_page5_clicked()
{
    changeSelectBtn(WNavbarButton::BarType::Sound_Page5);
    ui.stackedWidget->setCurrentIndex(4);
}

void WSoundBotMainUi::on_pb_sound_page6_clicked()
{
    changeSelectBtn(WNavbarButton::BarType::Sound_Page6);
    ui.stackedWidget->setCurrentIndex(5);

    ui.history_dia_page->getTransHistory();
}

void WSoundBotMainUi::changeSelectBtn(WNavbarButton::BarType type)
{
    ui.pb_sound_page1->setSelect(type == WNavbarButton::BarType::Sound_Page1);
    ui.pb_sound_page2->setSelect(type == WNavbarButton::BarType::Sound_Page2);
    ui.pb_sound_page3->setSelect(type == WNavbarButton::BarType::Sound_Page3);
    ui.pb_sound_page4->setSelect(type == WNavbarButton::BarType::Sound_Page4);
    ui.pb_sound_page5->setSelect(type == WNavbarButton::BarType::Sound_Page5);
    ui.pb_sound_page6->setSelect(type == WNavbarButton::BarType::Sound_Page6);
}

void WSoundBotMainUi::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui.retranslateUi(this);
        ui.pb_sound_page1->initBar(tr("Sound Source"), WNavbarButton::BarType::Sound_Page1);
        ui.pb_sound_page2->initBar(tr("Translation"), WNavbarButton::BarType::Sound_Page2);
        ui.pb_sound_page3->initBar(tr("Voice Library"), WNavbarButton::BarType::Sound_Page3);
        ui.pb_sound_page4->initBar(tr("My Voice"), WNavbarButton::BarType::Sound_Page4);
        ui.pb_sound_page5->initBar(tr("Speech Synthesis"), WNavbarButton::BarType::Sound_Page5);
        ui.pb_sound_page6->initBar(tr("Translation History"), WNavbarButton::BarType::Sound_Page6);
        update();
    }

    QWidget::changeEvent(event);
}