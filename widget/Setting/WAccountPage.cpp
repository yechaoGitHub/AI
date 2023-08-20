#include "WAccountPage.h"
#include <QDate>


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
	ui.lb_time->setText(tr("Premium(Expires on %1)").arg(cur_date));
}