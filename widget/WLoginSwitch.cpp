#include "WLoginSwitch.h"
#include <qevent.h>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QEvent>

WLoginSwitch::WLoginSwitch(QWidget* parent) :
    QWidget{ parent },
    _title{ ETitle::userName }
{
    setMouseTracking(true);

    _passwordLogin = tr("Password Login");
    _verificationCodeLogin = tr("Verification Code Login");
}

WLoginSwitch::~WLoginSwitch()
{
}

void WLoginSwitch::paintEvent(QPaintEvent* event)
{
    auto rt = this->rect();
    rt = rt.marginsRemoved(QMargins{1, 1, 1, 1});

    QRect rtUser{ 0, 0, rt.width() / 2, rt.height() };
    QRect rtMobile{ rt.width() / 2, 0, rt.width() / 2, rt.height() };

    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QBrush brush;
    brush.setColor(QColor{ 228, 239, 255 });
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    if (_title == ETitle::userName)
    {
        path.addRoundedRect(rtUser, 8, 8);
        QRect temp_rect(rtUser.left(), rtUser.top()+8, rtUser.width(), rtUser.height());
        path.addRect(temp_rect);
        painter.fillPath(path, brush);
    }
    else
    {
        path.addRoundedRect(rtMobile, 8, 8);
        QRect temp_rect(rtMobile.left(), rtMobile.top() + 8, rtMobile.width(), rtMobile.height());
        path.addRect(temp_rect);
        painter.fillPath(path, brush);
    }

    QLinearGradient g{ 0.0, 0.0, 1.0, 1.0 };
    g.setSpread(QGradient::ReflectSpread);
    g.setColorAt(0, QColor{ 0, 102, 255 });
    g.setColorAt(1, QColor{ 189, 0, 255 });

    QPen pen;
    painter.setFont(this->font());

    QFontMetrics fm = painter.fontMetrics();
    auto tW = fm.width(_passwordLogin);
    auto tH = fm.height();
    auto outTextRt = GetTextRect(ETitle::userName);
    QRect inTextRt;
    inTextRt.setX((outTextRt.width() - tW) / 2);
    inTextRt.setWidth(tW);
    inTextRt.setY((outTextRt.height() - tH) / 2);
    inTextRt.setHeight(tH);

    QTextOption opt;
    opt.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    if (_title == ETitle::userName)
    {
        g.setFinalStop((float)tW, (float)tH);
        pen.setBrush(g);
        painter.setPen(pen);
    }
    else
    {
        painter.setPen(QColor{ 19, 19, 19 });
    }
    painter.drawText(inTextRt, _passwordLogin, opt);


    if (_title != ETitle::userName)
    {
        g.setFinalStop((float)tW, (float)tH);
        pen.setBrush(g);
        painter.setPen(pen);
    }
    else
    {
        painter.setPen(QColor{ 19, 19, 19 });
    }
    painter.drawText(rtMobile, _verificationCodeLogin, opt);

    g.setFinalStop((float)rt.width(), (float)rt.height());
    pen.setBrush(g);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(QLine{0, rt.height(), rt.width(), rt.height()});
}

void WLoginSwitch::mousePressEvent(QMouseEvent* event)
{

}

void WLoginSwitch::mouseReleaseEvent(QMouseEvent* event)
{
    if (GetTextRect(ETitle::userName).contains(event->pos()))
    {
        _title = ETitle::userName;
        emit TitleChanged(ETitle::userName);
    }
    else if (GetTextRect(ETitle::mobile).contains(event->pos()))
    {
        _title = ETitle::mobile;
        emit TitleChanged(ETitle::mobile);
    }

    repaint();
}

void WLoginSwitch::mouseMoveEvent(QMouseEvent* event)
{
    if (GetTextRect(ETitle::userName).contains(event->pos()))
    {
        setCursor(Qt::PointingHandCursor);
    }
    else if (GetTextRect(ETitle::mobile).contains(event->pos()))
    {
        setCursor(Qt::PointingHandCursor);
    }
    else
    {
        unsetCursor();
    }
}

QRect WLoginSwitch::GetTextRect(ETitle title)
{
    auto rt = this->rect();
    if (title == ETitle::userName)
    {
        return QRect{ 0, 0, rt.width() / 2, rt.height() };
    }
    else
    {
        return QRect{ rt.width() / 2, 0, rt.width() / 2, rt.height() };
    }
}

void WLoginSwitch::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        _passwordLogin = tr("Password Login");
        _verificationCodeLogin = tr("Verification Code Login");
        update();
    }

    QWidget::changeEvent(event);
}

