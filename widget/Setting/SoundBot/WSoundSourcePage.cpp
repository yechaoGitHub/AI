#include "WSoundSourcePage.h"
#include "AiSound.h"
#include "base\GlobalSetting.h"
#include <QListView>


WSoundSourcePage::WSoundSourcePage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.cbMic->setView(new QListView);
    ui.cbSpeaker->setView(new QListView);
    ui.cbMonitor->setView(new QListView);
    connect(ui.cbMic, static_cast<void(QComboBox::*)(int)>(& QComboBox::currentIndexChanged), this, &WSoundSourcePage::MicIndexChanged);
    connect(ui.cbSpeaker, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WSoundSourcePage::SpeakerIndexChanged);
    connect(ui.cbMonitor, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WSoundSourcePage::WSoundSourcePage::MonitorIndexChanged);
    connect(ui.pb_speak, &QPushButton::clicked, this, [=] {
        ui.widget->startMovice(8);
        });
    connect(ui.pb_speak, &QPushButton::clicked, this, [=] {
        ui.widget->startMovice(8);
        });
    connect(ui.pb_mic, &QPushButton::clicked, this, [=] {
        ui.widget_2->startMovice(6);
        });
    connect(ui.pb_monitor, &QPushButton::clicked, this, [=] {
        ui.widget_3->startMovice(88);
        });
}

WSoundSourcePage::~WSoundSourcePage()
{

}

void WSoundSourcePage::showEvent(QShowEvent* event)
{
    auto& ins = AiSound::GetInstance();
    _inList = ins.GetInputDeviceList();
    int32_t index{ 0 };
    for (auto& in : _inList)
    {
        ui.cbMic->addItem(in.deviceName(), QVariant::fromValue(index));
        if (SETTING.MicDeviceInfo() == in)
        {
            ui.cbMic->setCurrentIndex(index);
        }
        index++;
    }

    index = 0;
    _outList = ins.GetOutputDeviceList();
    for (auto& out : _outList)
    {
        ui.cbSpeaker->addItem(out.deviceName(), QVariant::fromValue(index));
        if (SETTING.SpeakerDeviceInfo() == out)
        {
            ui.cbSpeaker->setCurrentIndex(index);
        }
        index++;
    }

    index = 0;
    for (auto& in : _inList)
    {
        ui.cbMonitor->addItem(in.deviceName(), QVariant::fromValue(index));
        if (SETTING.MonitorDeviceInfo() == in)
        {
            ui.cbMonitor->setCurrentIndex(index);
        }
        index++;
    }
}

void WSoundSourcePage::hideEvent(QHideEvent* event)
{
    ui.cbMic->clear();
    ui.cbSpeaker->clear();
}

void WSoundSourcePage::MicIndexChanged(int index)
{
    auto curIndex =  ui.cbMic->currentIndex();
    if (curIndex > 0 && curIndex < _inList.size())
    {
        SETTING.MicDeviceInfo() = _inList[curIndex];
    }
}

void WSoundSourcePage::SpeakerIndexChanged(int index)
{
    auto curIndex = ui.cbSpeaker->currentIndex();
    if (curIndex > 0 && curIndex < _outList.size())
    {
        SETTING.SpeakerDeviceInfo() = _outList[curIndex];
    }
}

void WSoundSourcePage::MonitorIndexChanged(int index)
{
    auto curIndex = ui.cbMonitor->currentIndex();
    if (curIndex > 0 && curIndex < _inList.size())
    {
        SETTING.MonitorDeviceInfo() = _inList[curIndex];
    }
}
