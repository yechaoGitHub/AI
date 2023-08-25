#include "WGerenalPage.h"
#include <QListView>
#include "function/AiSound.h"
#include "Bussiness/SettingInterfaceBussiness.h"


WGerenalPage::WGerenalPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.comboBox->setView(new  QListView());
	ui.comboBox->addItem(tr("English"));
	ui.comboBox->addItem(tr("Chinese"));

	connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboxIndexChange(int)));
	connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_common_replay,this,&WGerenalPage::slot_common_replay);
}

WGerenalPage::~WGerenalPage()
{}

void WGerenalPage::slot_common_replay(int type, bool success, const QString& msg)
{
	if (type == httpReqType::Feedback) {
		if (success) {
			ui.textEdit->clear();
			AiSound::GetInstance().ShowTip(this, tr("Send Ok"));
		}
		else
		AiSound::GetInstance().ShowTip(this,msg);
	}
}

void WGerenalPage::slot_comboxIndexChange(int index)
{

}

void WGerenalPage::on_pb_send_clicked()
{
	QString content = ui.textEdit->toPlainText();
	if (content.isEmpty()) {
		return;
	}

	SettingInterfaceBussiness::getInstance()->feedBackReq(content);
}