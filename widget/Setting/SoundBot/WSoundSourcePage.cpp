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

    _aoMic.Initialize();
    _aoMonitor.Initialize();
    _aoLoop.Initialize();

    _aoMic.EndMic();
    _aoMonitor.EndMic();
    _aoLoop.EndMic();

    connect(&_aoMic, &AudioInput::volumeLevel, this, &WSoundSourcePage::MicVolumeLevel);
    connect(&_aoMonitor, &AudioInput::volumeLevel, this, &WSoundSourcePage::MonitorVolumeLevel);
    connect(&_aoLoop, &AudioLoop::volumeLevel, this, &WSoundSourcePage::LoopVolumeLevel);

    connect(ui.pb_output, &QPushButton::clicked, this, &WSoundSourcePage::PbOutputClicked);
    connect(ui.pb_phyMic, &QPushButton::clicked, this, &WSoundSourcePage::PbPhyMicClicked);
    connect(ui.pb_virInput, &QPushButton::clicked, this, &WSoundSourcePage::PbVirInputClicked);

    ui.pb_output->setProperty("open", true);
    ui.pb_output->style()->unpolish(ui.pb_output);

    ui.pb_phyMic->setProperty("open", false);
    ui.pb_phyMic->style()->unpolish(ui.pb_phyMic);

    ui.pb_virInput->setProperty("open", false);
    ui.pb_virInput->style()->unpolish(ui.pb_virInput);
}

WSoundSourcePage::~WSoundSourcePage()
{
    _aoMic.Uninitialize();
    _aoMonitor.Uninitialize();
}

void WSoundSourcePage::showEvent(QShowEvent* event)
{
    _avoid = true;

    auto& ins = AiSound::GetInstance();
    _inList = ins.GetInputDeviceList();
    int32_t index{ 0 };
    for (auto& in : _inList)
    {
        auto name = in.deviceName();
        ui.cbMic->addItem(in.deviceName(), name);
        if (SETTING.getMicDeviceName() == name)
        {
            ui.cbMic->setCurrentIndex(index);
        }
        index++;
    }

    index = 0;
    for (auto& out : _inList)
    {
        auto name = out.deviceName();
        ui.cbSpeaker->addItem(out.deviceName(), name);
        if (SETTING.getMonitorDeviceName() == name)
        {
            ui.cbSpeaker->setCurrentIndex(index);
        }
        index++;
    }

    _outList = ins.GetOutputDeviceList();
    index = 0;
    for (auto& in : _outList)
    {
        auto name = in.deviceName();
        ui.cbMonitor->addItem(in.deviceName(), name);
        if (SETTING.getSpeakerDeviceName() == name)
        {
            ui.cbMonitor->setCurrentIndex(index);
        }
        index++;
    }

    _avoid = false;
}

void WSoundSourcePage::hideEvent(QHideEvent* event)
{
    _avoid = true;

    _aoMic.EndMic();
    _aoMonitor.EndMic();

    ui.cbMic->clear();
    ui.cbSpeaker->clear();
    ui.cbMonitor->clear();

    _avoid = false;
}

void WSoundSourcePage::MicIndexChanged(int index)
{
    if (_avoid)
    {
        return;
    }

    auto curIndex =  ui.cbMic->currentIndex();
    if (curIndex >= 0)
    {
        auto name = ui.cbMic->itemData(curIndex).toString();
        if (!name.isEmpty())
        {
            SETTING.setMicDeviceName(name);
        }
    }

    if (_aoMic.IsRunning())
    {
        _aoMic.EndMic();
        auto name = SETTING.getMicDeviceName();
        auto devInfo = AiSound::GetInstance().GetInputDeviceFormName(name, "default");
        _aoMic.StartMic(devInfo);
    }
}

void WSoundSourcePage::SpeakerIndexChanged(int index)
{
    if (_avoid)
    {
        return;
    }

    auto curIndex = ui.cbSpeaker->currentIndex();
    if (curIndex >= 0)
    {
        auto name = ui.cbSpeaker->itemData(curIndex).toString();
        if (!name.isEmpty())
        {
            SETTING.setMonitorDeviceName(name);
        }
    }

    if (_aoMonitor.IsRunning())
    {
        _aoMonitor.EndMic();
        auto name = SETTING.getMonitorDeviceName();
        auto devInfo = AiSound::GetInstance().GetInputDeviceFormName(name, "default");
        _aoMonitor.StartMic(devInfo);
    }
}

void WSoundSourcePage::MonitorIndexChanged(int index)
{
    if (_avoid)
    {
        return;
    }

    auto curIndex = ui.cbMonitor->currentIndex();
    if (curIndex >= 0)
    {
        auto name = ui.cbMonitor->itemData(curIndex).toString();
        if (!name.isEmpty())
        {
            SETTING.setSpeakerDeviceName(name);
        }
    }

    if (_aoLoop.IsRunning())
    {
        _aoLoop.EndMic();
        auto name = SETTING.getSpeakerDeviceName();
        auto devInfo = AiSound::GetInstance().GetOutputDeviceFormName(name, "default");
        _aoLoop.StartMic(devInfo);
    }
}

// (-15) - (-5)
void WSoundSourcePage::MicVolumeLevel(int level)
{
    int v = (std::max)(level + 15, 0);
    ui.slMic->startMovice(v);
}

void WSoundSourcePage::MonitorVolumeLevel(int level)
{
    int v = (std::max)(level + 15, 0);
    ui.slSpeaker->startMovice(v);
}

void WSoundSourcePage::LoopVolumeLevel(int level)
{
    int v = (std::max)(level + 15, 0);
    ui.slMonitor->startMovice(v);
}

void WSoundSourcePage::PbOutputClicked()
{
    auto b = ui.pb_output->property("open").toBool();
    if (b)
    {
        ui.slMic->stopMovice();
        _aoMonitor.EndMic();
    }
    else
    {
        auto name = SETTING.getMonitorDeviceName();
        auto devInfo = AiSound::GetInstance().GetInputDeviceFormName(name, "default");
        _aoMonitor.StartMic(devInfo);
    }

    ui.pb_output->setProperty("open", !b);
    ui.pb_output->style()->unpolish(ui.pb_output);
}

void WSoundSourcePage::PbPhyMicClicked()
{
    auto b = ui.pb_phyMic->property("open").toBool();
    if (b)
    {
        ui.slMic->stopMovice();
        _aoMic.EndMic();
    }
    else
    {
        auto name = SETTING.getMicDeviceName();
        auto devInfo = AiSound::GetInstance().GetInputDeviceFormName(name, "default");
        _aoMic.StartMic(devInfo);
    }

    ui.pb_phyMic->setProperty("open", !b);
    ui.pb_phyMic->style()->unpolish(ui.pb_phyMic);
}

void WSoundSourcePage::PbVirInputClicked()
{
    auto b = ui.pb_virInput->property("open").toBool();
    if (b)
    {
        ui.slMonitor->stopMovice();
        _aoLoop.EndMic();
    }
    else
    {
        auto name = SETTING.getSpeakerDeviceName();
        auto devInfo = AiSound::GetInstance().GetOutputDeviceFormName(name, "default");
        _aoLoop.StartMic(devInfo);
    }

    ui.pb_virInput->setProperty("open", !b);
    ui.pb_virInput->style()->unpolish(ui.pb_virInput);
}

void WSoundSourcePage::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui.retranslateUi(this);
    }

    QWidget::changeEvent(event);
}