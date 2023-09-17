#pragma once

#include "VoiceType.h"
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
    void closeEvent(QCloseEvent* event) override;

private:
    void LanguageIndexChanged(int index);
    void SexIndexChanged(int index);
    void NameIndexChanged(int index);

    void CloseClicked();
    void StartClicked();
    void SendClicked();
    void ExportClicked();
    void TranslationReceived(const QString& src, const QString& dst, int type);
    QString GetSelectSpeaker();

    Ui::WSpeechGenerationUiClass    ui;

    TranslationLanguage             _srcLan;
    TranslationLanguage             _destLan;

    QPoint                          _clickPos;
    bool                            _mouseHold = false;
};
