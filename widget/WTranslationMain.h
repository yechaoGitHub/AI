#pragma once
#include "AiSound.h"
#include "ui_TranslateMain.h"

#include <QWidget>
#include <QEvent>
#include <vector>

class WTranslationMain : public QWidget
{
    Q_OBJECT
public:
    WTranslationMain(QWidget* parent = nullptr);
    ~WTranslationMain();

    void Clear();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event);
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;

private:
    void MinClicked();
    void CloseClicked();
    void LockClicked();
    void StopClicked();
    void PlayInternal(bool play);
    void TransStateChanged(int state);
    void TranslationReceived(const QString& src, const QString& dst, int type);
    bool GetSelectSrcLanguage(TranslationLanguage& language);
    bool GetSelectDestLanguage(TranslationLanguage& language);
    void SyncUI();

    Ui::translateMain                   ui;

    QPoint                              _clickPos;
    bool                                _mouseHold = false;
    bool                                _mouseIn = false;
    bool                                _newSubtitle = true;
};

