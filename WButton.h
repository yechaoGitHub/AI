#pragma once
#include <QWidget>

class WButton : public QWidget
{
    Q_OBJECT
public:
    WButton(QWidget* parent = nullptr);
    ~WButton();

    void SetText(const QString& text);

Q_SIGNALS:
    void clicked();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QString _text;
};

