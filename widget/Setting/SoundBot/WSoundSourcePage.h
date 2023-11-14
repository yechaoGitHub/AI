#pragma once

#include <QWidget>
#include <QAudioDeviceInfo>
#include "AudioInput.h"
#include "AudioLoop.h"

#include "ui_WSoundSourcePage.h"


class WSoundSourcePage : public QWidget
{
    Q_OBJECT

public:
    WSoundSourcePage(QWidget *parent = nullptr);
    ~WSoundSourcePage();

protected:
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    void changeEvent(QEvent* event) override;
private:
    void MicIndexChanged(int index);
    void SpeakerIndexChanged(int index);
    void MonitorIndexChanged(int index);

    void MicVolumeLevel(int level);
    void MonitorVolumeLevel(int level);
    void LoopVolumeLevel(int level);

    void PbOutputClicked();
    void PbPhyMicClicked();
    void PbVirInputClicked();

    Ui::WSoundSourcePageClass           ui;
    bool                                _avoid = false;
    AudioInput                          _aoMic;
    AudioInput                          _aoMonitor;
    AudioLoop                           _aoLoop;

    std::vector<QAudioDeviceInfo>       _inList;
    std::vector<QAudioDeviceInfo>       _outList;
    std::vector<QAudioDeviceInfo>       _monitorList;
};
