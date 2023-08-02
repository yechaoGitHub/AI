#include "WScrollBar.h"

WScrollBar::WScrollBar(QWidget* parent) :
    QScrollBar{ parent }
{
    setStyleSheet(
        "QScrollBar:vertical{ background-color:transparent; width:8px ;margin-top:0px;margin-bottom:0px }"
        "QScrollBar::handle:vertical{ width:8px;background:rgba(0,0,0,25%);border-radius:4px;min-height:20px;}"
        "QScrollBar::sub-line:vertical{height:0px;subcontrol-position:top;subcontrol-origin:margin;}"
        "QScrollBar::add-line:vertical{height:0px;subcontrol-position:bottom;subcontrol-origin:margin;}"
        "QScrollBar:horizontal{ background-color:transparent; height:8px ;margin-left:0px;margin-right:0px }"
        "QScrollBar::handle:horizontal{ width:8px;background:rgba(0,0,0,25%);border-radius:4px;min-height:20px;}"
        "QScrollBar::sub-line:horizontal{height:0px;subcontrol-position:top;subcontrol-origin:margin;}"
        "QScrollBar::add-line:horizontal{height:0px;subcontrol-position:bottom;subcontrol-origin:margin;}"
    );
}

WScrollBar::WScrollBar(Qt::Orientation o, QWidget* parent) :
    QScrollBar { o, parent }
{
    setStyleSheet(
        "QScrollBar:vertical{ background-color:transparent; width:8px ;margin-top:0px;margin-bottom:0px }"
        "QScrollBar::handle:vertical{ width:8px;background:rgba(0,0,0,25%);border-radius:4px;min-height:20px;}"
        "QScrollBar::sub-line:vertical{height:0px;subcontrol-position:top;subcontrol-origin:margin;}"
        "QScrollBar::add-line:vertical{height:0px;subcontrol-position:bottom;subcontrol-origin:margin;}"
        "QScrollBar:horizontal{ background-color:transparent; height:8px ;margin-left:0px;margin-right:0px }"
        "QScrollBar::handle:horizontal{ width:8px;background:rgba(0,0,0,25%);border-radius:4px;min-height:20px;}"
        "QScrollBar::sub-line:horizontal{height:0px;subcontrol-position:top;subcontrol-origin:margin;}"
        "QScrollBar::add-line:horizontal{height:0px;subcontrol-position:bottom;subcontrol-origin:margin;}"
    );
}
