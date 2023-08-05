#include "WChatDesc.h"

WChatDesc::WChatDesc(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.pb_start, &QPushButton::clicked, this, [=] {
		emit sig_startClick();
		});
}

WChatDesc::~WChatDesc()
{}
