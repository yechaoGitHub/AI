#include "WGerenalPage.h"
#include <QListView>
#include "Bussiness/SettingInterfaceBussiness.h"


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

void WGerenalPage::on_pb_send_clicked()
{
	QString content = ui.textEdit->toPlainText();
	if (content.isEmpty()) {
		return;
	}

	SettingInterfaceBussiness::getInstance()->feedBackReq(content);
}