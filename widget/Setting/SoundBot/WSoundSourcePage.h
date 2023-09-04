#pragma once

#include <QWidget>
#include <QAudioDeviceInfo>
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

private:
    void MicIndexChanged(int index);
    void SpeakerIndexChanged(int index);

    Ui::WSoundSourcePageClass           ui;
    std::vector<QAudioDeviceInfo>       _inList;
    std::vector<QAudioDeviceInfo>       _outList;
    std::vector<QAudioDeviceInfo>       _monitorList;
};
