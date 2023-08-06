#include "WGerenalPage.h"
#include <QListView>


WGerenalPage::WGerenalPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.comboBox->setView(new  QListView());
	ui.comboBox->addItem("English");
	ui.comboBox->addItem("Chinese");
}

WGerenalPage::~WGerenalPage()
{}
