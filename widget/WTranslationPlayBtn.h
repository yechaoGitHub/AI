#pragma once

#include <QWidget>
#include <QImage>

class WTranslationPlayBtn : public QWidget
{
    Q_OBJECT
public:
    WTranslationPlayBtn(QWidget* parent = nullptr);
    ~WTranslationPlayBtn();

protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    QImage          _icon;
    QRect           _iconRect;
};
