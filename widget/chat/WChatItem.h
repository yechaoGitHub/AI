#pragma once

#include <QWidget>
#include <qpushbutton.h>

class QPaintEvent;
class QPainter;
class QLabel;
class QMovie;

class WChatItem  : public QWidget
{
    Q_OBJECT

public:
    explicit WChatItem(QWidget*parent);
    ~WChatItem();

    enum User_Type {
        User_System,//系统
        User_Robot,    //自己
        User_Self,   //用户
        User_Time,  //时间
    };
    void setTextSuccess();
    void setText(QString text, QString time, QSize allSize, User_Type userType);

    QSize getRealString(QString src);
    QSize fontRect(QString str);

    inline QString text() { return m_msg; }
    inline QString time() { return m_time; }
    inline User_Type userType() { return m_userType; }
protected:
    void paintEvent(QPaintEvent* event);
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void copyContent();

private:
    QString m_msg;
    QString m_time;
    QString m_curTime;

    QSize m_allSize;
    User_Type m_userType = User_System;

    int m_kuangWidth;
    int m_textWidth;
    int m_spaceWid;
    int m_lineHeight;

    QRect m_iconLeftRect;
    QRect m_iconRightRect;
    QRect m_sanjiaoLeftRect;
    QRect m_sanjiaoRightRect;
    QRect m_kuangLeftRect;
    QRect m_kuangRightRect;
    QRect m_textLeftRect;
    QRect m_textRightRect;
    QPixmap m_robotPixmap;
    QPixmap m_selfPixmap;
    QLabel* m_loading = nullptr;
    QMovie* m_loadingMovie = nullptr;
    bool m_isSending = false;

    QPushButton* m_pCopyBtn = nullptr;
};
