#include "WConformDlg.h"
#include <QPainter>
#include <qmath.h>


WConformDlg::WConformDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    this->setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint | Qt::SubWindow);
}

WConformDlg::~WConformDlg()
{}

int WConformDlg::Show(const QString& title, const QString& name)
{
    ui.lb_title->setText(title);
    ui.lb_content->setText(name);

    return exec();
}

void WConformDlg::on_btn_close_clicked()
{
    done(QDialog::Rejected);
}

void WConformDlg::on_pb_cancel_clicked()
{
    done(QDialog::Rejected);
}

void WConformDlg::on_pb_sure_clicked()
{
    done(QDialog::Accepted);
}

void WConformDlg::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    if (isActiveWindow()) {
        p.save();
        p.setRenderHint(QPainter::Antialiasing, true);
        QColor color(0, 0, 0, 10);
        for (int i = 0; i < 2; i++)
        {
            p.setPen(color);
            p.drawRoundedRect(rect().adjusted(2 - i, 2 - i, i - 2, i - 2), 8, 8);
        }
        p.restore();
    }

    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(QColor("#ffffff"));
    p.setBrush(QColor("#ffffff"));
    p.drawRoundedRect(8, 8, width() - 10, height() - 10, 8, 8);
}

void WConformDlg::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui.retranslateUi(this);
    }

    QWidget::changeEvent(event);
}