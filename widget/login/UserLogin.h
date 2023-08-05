#pragma once

#include <QWidget>
#include "ui_UserLogin.h"

class UserLogin : public QWidget
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = nullptr);
	~UserLogin();

private:
	Ui::UserLoginClass ui;
};
