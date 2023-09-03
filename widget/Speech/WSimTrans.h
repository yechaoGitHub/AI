#pragma once
#include <QWidget.h>
#include "ui_SimultaneousInterpretation.h"

class WSimTrans : public QWidget
{
    Q_OBJECT
public:
    WSimTrans(QWidget* parent = nullptr);
    ~WSimTrans();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    Ui::simTrans                        ui;

    QPoint                              _clickPos;
    bool                                _mouseHold;
};
