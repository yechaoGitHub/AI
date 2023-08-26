#pragma once
#include "AiSound.h"
#include "ui_TranslateMain.h"

#include <QWidget>
#include <QEvent>


class WTransaltionMain : public QWidget
{
    Q_OBJECT
public:
    WTransaltionMain(QWidget* parent = nullptr);
    ~WTransaltionMain();

    void SetLanguage(const TranslationLanguage& srcLan, const TranslationLanguage& destLan);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    void MinClicked();
    void CloseClicked();
    void LockClicked();
    void PlayInternal(bool play);
    void TranslationReceived(const QString& src, const QString& dst, int type);
    void StopBtnStateChanged(WTranslationPlayBtn::State state);

    TranslationLanguage     _srcLan;
    TranslationLanguage     _destLan;

    Ui::translateMain       ui;
    QPoint                  _clickPos;
    bool                    _mouseHold = false;
    bool                    _mouseIn = false;
    bool                    _newSubtitle = true;
};

