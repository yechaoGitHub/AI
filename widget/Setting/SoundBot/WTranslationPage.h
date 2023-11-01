#pragma once

#include <QWidget>
#include "ui_WTranslationPage.h"
#include "model/WRadioBtn.h"


class WTranslationPage : public QWidget
{
	Q_OBJECT

public:
	WTranslationPage(QWidget *parent = nullptr);
	~WTranslationPage();

protected:
	void changeEvent(QEvent* event) override;

private slots:
	void slot_bk_clicked();
	void slot_tl_clicked();
	void slot_og_clicked();
private:
	Ui::WTranslationPageClass ui;

	QVector<WRadioBtn*>		_tl_ck_list_;
	QVector<WRadioBtn*>		_og_ck_list_;
};
