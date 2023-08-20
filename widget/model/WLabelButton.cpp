#include "WLabelButton.h"

WLabelButton::WLabelButton(QWidget *parent)
	: QPushButton(parent)
{
	this->setStyleSheet("border:none;color:#000000;");
	this->setMinimumSize(90,18);
	this->setMaximumSize(180, 18);
}

WLabelButton::~WLabelButton()
{}

void WLabelButton::setType(int type, const QString& typeName)
{
	_type = type;
	_typeName = typeName;
	this->setText(typeName);
}

void WLabelButton::setSelected(bool sel)
{
	if (sel) {
		this->setStyleSheet("border:none;color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #0066FF, stop:1 #BD00FF);");
	}
	else {
		this->setStyleSheet("border:none;color:#000000;");
	}
}