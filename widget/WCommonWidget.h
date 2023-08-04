#pragma once

#include <QWidget>

class WCommonWidget : public QWidget
{
    Q_OBJECT

public:
    WCommonWidget(QWidget *parent = nullptr);
    ~WCommonWidget();

    void SetSelect(bool select);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool _select;
};
