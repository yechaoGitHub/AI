#pragma once
#include <QWidget>

class WCheckBox : public QWidget
{
    Q_OBJECT

public:
    WCheckBox(QWidget *parent = nullptr);
    ~WCheckBox();

    void SetText(const QString& text);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString     _text;
};

