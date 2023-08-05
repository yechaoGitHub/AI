#include "WLoginUI.h"


WLoginUI::WLoginUI(QWidget *parent)
	: FrameLessWidget(parent)
{
	ui.setupUi(this);
	this->setWidgetType(true, DragType::Drag_Null, false);
	setAttribute(Qt::WA_TranslucentBackground);

	connect(ui.pb_min, &QPushButton::clicked, this, [=] {
		this->showMinimized();
		});
	connect(ui.pb_close, &QPushButton::clicked, this, [=] {
		this->close();
		});
}

WLoginUI::~WLoginUI()
{}
