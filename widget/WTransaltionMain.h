#pragma once
#include <QWidget>
#include "AiSound.h"
#include "ui_TranslateMain.h"


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

private:
    void CloseClicked();
    void TranslationReceived(const QString& src, const QString& dst, int type);

    TranslationLanguage     _srcLan;
    TranslationLanguage     _destLan;

    Ui::translateMain       ui;
    QPoint                  _clickPos;
    bool                    _mouseHold;
};

