#pragma once

#include <QPushButton>
#include <QLabel>

class WNavbarButton  : public QPushButton
{
	Q_OBJECT

public:
	WNavbarButton(QWidget*parent);
	~WNavbarButton();

	enum BarType {
		Bar_Account = 0,
		Bar_Help,
		Bar_Team,
		Bar_Tools,
		Bar_Lib,
		Bar_History,
		Sound_Page1,
		Sound_Page2,
		Sound_Page3,
		Sound_Page4,
		Sound_Page5,
		Sound_Page6
	};

	void initBar(const QString& text, BarType bar_type);
	void setSelect(bool select);
protected:
	void paintEvent(QPaintEvent*) override;

private:


private:
	bool	_is_select = false;
	BarType  _button_type = BarType::Bar_Account;
	QPixmap m_iconPixmap;
	QPixmap m_linePixmap;
	QPixmap m_rightPixmap;

	QLabel* m_pLabel = nullptr;

	QString m_strText;
	int _width =0;
	int _height = 0;
};
