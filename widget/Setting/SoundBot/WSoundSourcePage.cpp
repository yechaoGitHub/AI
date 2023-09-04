#include "WSoundSourcePage.h"
#include "AiSound.h"
#include "base\GlobalSetting.h"

WSoundSourcePage::WSoundSourcePage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.cbMic, static_cast<void(QComboBox::*)(int)>(& QComboBox::currentIndexChanged), this, &WSoundSourcePage::MicIndexChanged);
    connect(ui.cbSpeaker, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &WSoundSourcePage::SpeakerIndexChanged);
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
