#include "WSettingMainUi.h"
#include <QPainter>
#include <qmath.h>


WSettingMainUi::WSettingMainUi(QWidget *parent)
    : FrameLessWidget(parent)
{
    ui.setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setWidgetType(true, DragType::Drag_Null, false);

    connect(ui.pb_min, &QPushButton::clicked, this, [this] {
        this->showMinimized();
        });
    connect(ui.pb_close, &QPushButton::clicked, this, [this] {
        this->close();
        });

    //this->setFixedSize(664, 490);
}

WSettingMainUi::~WSettingMainUi()
{}

void WSettingMainUi::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    if (isActiveWindow()) {
        p.save();
        p.setRenderHint(QPainter::Antialiasing, true);
        QColor color(0, 0, 0);
        for (int i = 0; i < 4; i++)
        {
            color.setAlpha(60 - qSqrt(i) * 20);
            p.setPen(color);
            p.drawRoundedRect(rect().adjusted(4 - i, 4 - i, i - 4, i - 4), 4, 4);
        }
        p.restore();
    }

    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(QColor("#ffffff"));
    p.setBrush(QColor("#ffffff"));
    p.drawRoundedRect(4, 4, width() - 8, height() - 8, 8, 8);
}