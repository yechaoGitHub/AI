#pragma once

#include <QWidget>
#include "ui_WInvitePage.h"

class WInvitePage : public QWidget
{
	Q_OBJECT

public:
	WInvitePage(QWidget *parent = nullptr);
	~WInvitePage();

private:
	Ui::WInvitePageClass ui;
};
