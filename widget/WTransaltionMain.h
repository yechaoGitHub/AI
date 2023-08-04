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

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void paintEvent(QPaintEvent* event) override;

private:
    void TranslationReceived(const QString& src, const QString& dst, int type);

    Ui::translateMain   ui;
    QPoint              _clickPos;
    bool                _mouseHold;
};

