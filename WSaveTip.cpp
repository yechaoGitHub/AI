#include "WSaveTip.h"

#include <QPainter>
#include <QPainterPath>

WSaveTip::WSaveTip(QWidget* parent) :
    QDialog{ parent }
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    ui.setupUi(this);

    connect(ui.pbClose, &QPushButton::clicked, this, &WSaveTip::CloseClicked);
    connect(ui.pbConfirm, &QPushButton::clicked, this, &WSaveTip::ConfirmClicked);
    connect(ui.pbCancel, &QPushButton::clicked, this, &WSaveTip::ConfirmClicked);
}

WSaveTip::~WSaveTip()
{
}

bool WSaveTip::Sure()
{
    return _sure;
}

void WSaveTip::paintEvent(QPaintEvent* event)
{
    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(this->rect(), 16, 16);
    painter.fillPath(path, QBrush(QColor(0, 0, 0, 204)));
}

void WSaveTip::CloseClicked()
{
    close();
}

void WSaveTip::ConfirmClicked()
{
    _sure = true;
    close();
}

void WSaveTip::CancelClicked()
{
    close();
}
