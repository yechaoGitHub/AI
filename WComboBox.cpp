#include "WComboBox.h"
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QMouseEvent>

WComboBox::WComboBox(QWidget* parent) :
    QWidget{ parent },
    _dropDownList{ nullptr },
    _status{ Status::FOLD }
{
    _downImg.load(":/QtTest/icon/triangle_down.png");
    _upImg.load(":/QtTest/icon/triangle_up.png");

    this->setFocusPolicy(Qt::ClickFocus);

    _dropDownList = new WDropDownList{ this };
    _dropDownList->hide();
    
    auto app = static_cast<QApplication*>(QCoreApplication::instance());
    connect(_dropDownList, &WDropDownList::itemClicked, this, &WComboBox::ItemClicked);
    connect(_dropDownList, &WDropDownList::windowHiden, this, &WComboBox::DropDownHiden);
}

WComboBox::~WComboBox()
{
}

void WComboBox::AddItem(const QString& text, void* data)
{
    _dropDownList->AddItem(text);
    _datas.push_back(data);

    _dropDownList->resize(width(), 44 * 3);
}

int WComboBox::SelectItem()
{
    return 0;
}

void WComboBox::paintEvent(QPaintEvent* event)
{
    auto rt = this->rect();

    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QColor lineColor{};
    if (hasFocus()) 
    {
        lineColor = QColor{ 17, 106, 204 };
    }
    else 
    {
        lineColor = QColor{ 156, 163, 175 };
    }

    QPen pen{ lineColor };
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRoundedRect(rt, 8, 8);

    if (!_text.isEmpty()) 
    {
        auto textRt = rt;
        textRt.setX(20);

        QTextOption opt;
        opt.setAlignment(Qt::AlignLeft |Qt::AlignVCenter);
        painter.drawText(textRt, _text, opt);
    }

    QImage triangleImg;
    if (_status == Status::FOLD) 
    {
        triangleImg = _downImg;
    }
    else 
    {
        triangleImg = _upImg;
    }

    auto imgX = rt.width() - 24 - 16;
    auto imgY = (rt.height() - 24) / 2;
    painter.drawImage(QRect{ imgX, imgY, 24, 24 }, triangleImg, triangleImg.rect());
}

void WComboBox::resizeEvent(QResizeEvent* event)
{
    _dropDownList->resize(this->width(), this->height() * 3);
}

void WComboBox::mouseReleaseEvent(QMouseEvent* event)
{
    if (_dropDownList->isHidden())
    {
        QPoint pt{};
        pt = this->mapToGlobal(QPoint{});
        pt.setY(pt.y() + this->height());
        _dropDownList->move(pt);
        _dropDownList->show();
        _status = Status::EXPLAND;
    }
}

void WComboBox::ItemClicked(int row, const QString& text)
{
    _selectItem = row;
    _text = text;
    _dropDownList->hide();
}

void WComboBox::DropDownHiden()
{
    _status = Status::FOLD;
}


