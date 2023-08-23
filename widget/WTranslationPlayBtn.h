#pragma once

#include <QWidget>
#include <QImage>

class WTranslationPlayBtn : public QWidget
{
    Q_OBJECT
public:
    enum State { PLAY, STOP };

public:
    WTranslationPlayBtn(QWidget* parent = nullptr);
    ~WTranslationPlayBtn();

    void SetState(State state);

signals:
    void stateChanged(State state);

protected:
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    State           _state = PLAY;
    QImage          _icon;
    QRect           _iconRect;
};
