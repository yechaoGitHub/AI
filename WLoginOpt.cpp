#include "WLoginOpt.h"
#include <QFontMetrics>

WLoginOpt::WLoginOpt(QWidget* parent) :
    QWidget{ parent },
    registerLabel{ nullptr },
    loginBtn{ nullptr }
{
    ui.setupUi(this);
    ui.loginBtn->SetText("Login");

    ui.facebookBtn->setIconSize(QSize{ 24, 24 });
    ui.facebookBtn->setIcon(QIcon{":/QtTest/icon/facebook.png"});
    ui.facebookBtn->setCursor(Qt::PointingHandCursor);
    ui.facebookBtn->setStyleSheet("border-style:solid;border-width:1px;border-color:rgba(238, 238, 238, 1);border-radius:16px;");

    ui.googleBtn->setIconSize(QSize{ 24, 24 });
    ui.googleBtn->setIcon(QIcon{ ":QtTest/icon/google.png" });
    ui.googleBtn->setCursor(Qt::PointingHandCursor);
    ui.googleBtn->setStyleSheet("border-style:solid;border-width:1px;border-color:rgba(238, 238, 238, 1);border-radius:16px;");

    ui.wechatBtn->setIconSize(QSize{ 24, 24 });
    ui.wechatBtn->setIcon(QIcon{ ":QtTest/icon/wechat.png" });
    ui.wechatBtn->setCursor(Qt::PointingHandCursor);
    ui.wechatBtn->setStyleSheet("border-style:solid;border-width:1px;border-color:rgba(238, 238, 238, 1);border-radius:16px;");

    ui.label->setStyleSheet("color:rgba(155, 155, 155, 1)");
    ui.notLabel->setStyleSheet("color:rgba(155, 155, 155, 1)");
    ui.registerLabel->setStyleSheet("color:qlineargradient(spread:pad, x1:0, y1:0, x2:111, y2:20, stop:0 #0066FF, stop:1 #BD00FF)");

    registerLabel = ui.registerLabel;
    loginBtn = ui.loginBtn;
}

WLoginOpt::~WLoginOpt()
{
}

void WLoginOpt::resizeEvent(QResizeEvent* event)
{
    QFontMetrics fontMetrics{ ui.notLabel->font() };
    auto str = ui.notLabel->text() + ui.registerLabel->text();

    auto w = fontMetrics.width(str);
    auto x = (this->width() - w) / 2;

    auto pos = ui.notLabel->pos();
    ui.notLabel->move(x, pos.y());

    w = fontMetrics.width(ui.notLabel->text());
    auto h = ui.notLabel->rect().height();
    ui.notLabel->resize(w, h);

    ui.registerLabel->move(x + w + 2, pos.y());
}
