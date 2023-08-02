#include "WClickedLabel.h"

WClickedLabel::WClickedLabel(QWidget* parent, Qt::WindowFlags f) :
    QLabel{ parent, f }
{
    this->setCursor(Qt::PointingHandCursor);
}

WClickedLabel::WClickedLabel(const QString& text, QWidget* parent, Qt::WindowFlags f) :
    QLabel{ text, parent, f }
{
    this->setCursor(Qt::PointingHandCursor);
}

void WClickedLabel::mouseReleaseEvent(QMouseEvent* ev)
{
    emit clicked();
}
