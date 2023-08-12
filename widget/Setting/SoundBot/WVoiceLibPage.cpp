#include "WVoiceLibPage.h"
#include "WVoicelibWidget.h"

WVoiceLibPage::WVoiceLibPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			WVoicelibWidget* widget = new WVoicelibWidget(this);
			ui.gridLayout->addWidget(widget, i, j);
		}
	}

}

WVoiceLibPage::~WVoiceLibPage()
{}
