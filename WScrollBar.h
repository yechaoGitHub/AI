#pragma once
#include <QWidget>
#include <QScrollBar>

class WScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    explicit WScrollBar(QWidget* parent = nullptr);
    explicit WScrollBar(Qt::Orientation, QWidget* parent = nullptr);
};

