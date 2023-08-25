#include "WMyVoicePage.h"

WMyVoicePage::WMyVoicePage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);


	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			WMyVoiceModel* voice = new WMyVoiceModel(this);
			ui.gridLayout->addWidget(voice,i,j);
		}
	}

}

WMyVoicePage::~WMyVoicePage()
{}
