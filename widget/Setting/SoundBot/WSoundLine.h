#pragma once

#include <QObject>
#include <QPaintEvent>
#include <QWidget>
#include <QTimer>

class WSoundLine  : public QWidget{
    Q_OBJECT

public:
    WSoundLine(QWidget *parent);
    ~WSoundLine();

    void startMovice(int volume=6);
    void stopMovice();
protected:
    void paintEvent(QPaintEvent* event);

private:
    QTimer				_timer;
    int					_volume = 6;
    int					_cur_line = 0;
    bool				_movice = false;
};
