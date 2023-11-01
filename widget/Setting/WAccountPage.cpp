#include "WAccountPage.h"
#include "AiSound.h"
#include <QDate>
#include <QDesktopServices>
#include <QUrl>
#include "base/GlobalSetting.h"



WAccountPage::WAccountPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

WAccountPage::~WAccountPage()
{}

void WAccountPage::initAccount(const stru_UserInfo& user_info)
{
	ui.lb_phone->setText(user_info.phoneId);
	ui.lb_account->setText(QString::number(user_info.userId));
	ui.lb_name->setText(user_info.userName);
	ui.lb_point->setText(QString::number(user_info.balance, 'f', 2));

	QDate qdate = QDate::currentDate();
	QString cur_date = qdate.toString("dd/MM/yyyy");
	ui.lb_time->setText(tr("(Expires on %1)").arg(cur_date));
}

void WAccountPage::on_pb_charge_clicked()
{
	QString url = SETTING.getRechargeUrl();
	if (url.isEmpty()) {
		url = "https://aisounda.cn/#/account/package";
	}
	url.append("?").append("access_token=").append(AiSound::GetInstance().Token());
	QDesktopServices::openUrl(QUrl(url));
}

void WAccountPage::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::LanguageChange)
	{
		ui.retranslateUi(this);
	}

	QWidget::changeEvent(event);
}