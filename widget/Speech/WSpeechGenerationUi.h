#pragma once

#include "VoiceType.h"

#include <QWidget>
#include "ui_WSpeechGenerationUi.h"
#include "model/FrameLessWidget.h"

class WSpeechGenerationUi : public FrameLessWidget
{
    Q_OBJECT

public:
    WSpeechGenerationUi(QWidget *parent = nullptr);
    ~WSpeechGenerationUi();

    void SetLanguage(const TranslationLanguage& srcLan, const TranslationLanguage& destLan);

protected:
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;

private slots:
    void on_pb_close_clicked();

private:
    void StartClicked();
    void TranslationReceived(const QString& src, const QString& dst, int type);

    TranslationLanguage             _srcLan;
    TranslationLanguage             _destLan;
    std::vector<VoiceData>          _voiceData;

    Ui::WSpeechGenerationUiClass    ui;
};
