#include "WConformWidget.h"
#include <QPainter>


WConformWidget::WConformWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::SubWindow);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    ui.lb_content->setWordWrap(true);
}

WConformWidget::~WConformWidget()
{}

void WConformWidget::ShowConform(const QString& msg)
{
    ui.lb_content->setText(msg);
    show();
}

void WConformWidget::on_pb_sure_clicked()
{
    emit sig_conform();
    this->close();
}

void WConformWidget::on_pb_cancel_clicked()
{
    this->close();
}

void WConformWidget::paintEvent(QPaintEvent*)
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
    p.drawRoundedRect(2, 2, width() - 6, height() - 6, 8, 8);
}