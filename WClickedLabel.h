#pragma once

#include <QLabel>

class WClickedLabel : public QLabel
{
    Q_OBJECT
public:
    explicit WClickedLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    explicit WClickedLabel(const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

Q_SIGNALS:
    void clicked();

protected:
    void mouseReleaseEvent(QMouseEvent* ev) override;

};

