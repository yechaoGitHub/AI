#include "WCommonEdit.h"
#include <QPainter>
#include <QApplication>

WCommonEdit::WCommonEdit(QWidget* parent) :
    WCommonWidget{ parent }
{
    textEdit = new QLineEdit{"", this};
    textEdit->setFrame(false);

    auto app = static_cast<QApplication*>(QCoreApplication::instance());
    connect(app, &QApplication::focusChanged, this, &WCommonEdit::focusChanged);


    //_textEdit->setStyleSheet("background:#FFFF00");

    textEdit->setFont(this->font());
}

WCommonEdit::~WCommonEdit()
{

}

void WCommonEdit::SetImage(const QString& path)
{
    _icon.load(path);
}

void WCommonEdit::paintEvent(QPaintEvent* event)
{
    WCommonWidget::paintEvent(event);

    if (!_icon.isNull()) 
    {
        QPainter painter{ this };
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.drawImage(QRect{16, 20, 16, 16 }, _icon, _icon.rect());
    }
}

void WCommonEdit::resizeEvent(QResizeEvent* event)
{
    textEdit->setGeometry(40, 2, this->rect().width() - 45, this->rect().height() - 4);
}

void WCommonEdit::focusChanged(QWidget* old, QWidget* now)
{
    bool newSelect{ false };
    if (now == textEdit) 
    {
        newSelect = true;
    }
    else 
    {
        newSelect = false;
    }

    if (_selected != newSelect) 
    {
        _selected = newSelect;
        this->SetSelect(_selected);
        repaint();
    }
}
