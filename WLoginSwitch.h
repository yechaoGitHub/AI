#pragma once

#include <QWidget>

class WLoginSwitch : public QWidget
{
    Q_OBJECT

public:
    enum class ETitle { userName, mobile };

public:
    WLoginSwitch(QWidget* parent = nullptr);
    ~WLoginSwitch();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    QRect GetTextRect(ETitle title);

private:
    ETitle _title;

Q_SIGNALS:
    void TitleChanged(ETitle title);
};

