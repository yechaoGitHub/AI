#pragma once

#include <QPushButton>


class WRadioBtn  : public QPushButton
{
	Q_OBJECT

public:
	WRadioBtn(QWidget *parent,QColor color = QColor("#000000"));
	~WRadioBtn();

	void	setColor(QColor color);
	void	setSel(bool sel);
protected:
	void paintEvent(QPaintEvent* event) override;

	QColor	_color;
	bool	_select = false;
};
