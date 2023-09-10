#pragma once

#include "VoiceType.h"
#include "WSimTrans.h"
#include "model/FrameLessWidget.h"

#include <QWidget>
#include "ui_WSpeechGenerationUi.h"

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
    //void SimTransClicked();
    void CloseClicked();
    void StartClicked();
    void SendClicked();
    void ExportClicked();
    void TranslationReceived(const QString& src, const QString& dst, int type);

    Ui::WSpeechGenerationUiClass    ui;

    WSimTrans*                      _simTrans = nullptr;
    TranslationLanguage             _srcLan;
    TranslationLanguage             _destLan;
    std::vector<VoiceData>          _voiceData;

    QPoint                          _clickPos;
    bool                            _mouseHold = false;
};
