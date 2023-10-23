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

    void Clear();

protected:
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    void timerEvent(QTimerEvent* event) override;

private slots:
    void on_pb_lock_clicked();

private:
    void LanguageIndexChanged(int index);
    void SexIndexChanged(int index);
    void NameIndexChanged(int index);
    QString GetSelectSrcLanguage();
    void SyncUI();

    void CloseClicked();
    void StartClicked();
    void MinClicked();
    void SendClicked();
    void ExportClicked();
    void VCStateChanged(int state);
    void TranslationReceived(const QString& src, const QString& dst, int type);
    int GetSelectSpeaker();

    Ui::WSpeechGenerationUiClass    ui;
    QPoint                          _clickPos;
    int                             _time = 0;
    bool                            _mouseHold = false;
    bool                            _lock = false;
};
