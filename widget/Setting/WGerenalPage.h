#pragma once

#include <QWidget>
#include "ui_WGerenalPage.h"

class WGerenalPage : public QWidget
{
	Q_OBJECT

public:
	WGerenalPage(QWidget *parent = nullptr);
	~WGerenalPage();

private slots:
	void	on_pb_send_clicked();
	void	slot_comboxIndexChange(int index);
	void	slot_common_replay(int, bool, const QString& msg);

protected:
	void changeEvent(QEvent* event) override;
private:
	Ui::WGerenalPageClass ui;
};
