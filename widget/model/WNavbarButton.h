#pragma once

#include <QPushButton>

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
		Bar_History
	};

	void initBar(const QString& text, BarType bar_type);
	void setSelect(bool select);
protected:
	void paintEvent(QPaintEvent*) override;

private:


private:
	bool	_is_select = false;
	QPixmap m_iconPixmap;
	QPixmap m_linePixmap;
	QPixmap m_rightPixmap;

	QString m_strText;
	int _width =0;
	int _height = 0;
};
