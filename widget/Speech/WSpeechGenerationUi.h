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

protected:
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private slots:
    void on_pb_lock_clicked();

private:
    void LanguageIndexChanged(int index);
    void SexIndexChanged(int index);
    void NameIndexChanged(int index);
    QString GetSelectSrcLanguage();

    void CloseClicked();
    void StartClicked();
    void SendClicked();
    void ExportClicked();
    void TranslationReceived(const QString& src, const QString& dst, int type);
    QString GetSelectSpeaker();

    Ui::WSpeechGenerationUiClass    ui;
    QPoint                          _clickPos;
    bool                            _mouseHold = false;
    bool                            _lock = false;
};
