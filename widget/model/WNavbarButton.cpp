#include "WNavbarButton.h"
#include <QPainter>


WNavbarButton::WNavbarButton(QWidget*parent)
	: QPushButton(parent)
{
	m_linePixmap = QPixmap(":/QtTest/icon/Setting/line.png");
	m_rightPixmap = QPixmap(":/QtTest/icon/Setting/right.png");
	this->setStyleSheet("border:none;");
}

WNavbarButton::~WNavbarButton()
{}

void WNavbarButton::initBar(const QString& text, BarType bar_type)
{
	if (bar_type == BarType::Bar_Account) {
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/account.png");
	}
	else if (bar_type == BarType::Bar_Help) {
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/help.png");
	}
	else if (bar_type == BarType::Bar_Team) {
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/team.png");
	}
	else if (bar_type == BarType::Bar_Tools) {
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/tools.png");
	}
	else if (bar_type == BarType::Bar_Lib) {
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/chatbot/lib_icon.png");
	}
	else if (bar_type == BarType::Bar_History) {
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/chatbot/history_icon.png");
	}
	m_strText = text;

	QFontMetrics fm(this->font());
	_width = fm.width(m_strText);
	_height = fm.height();
}

void WNavbarButton::setSelect(bool select)
{
	_is_select = select;
	update();
}

void WNavbarButton::paintEvent(QPaintEvent*)
{
	auto btn_rect = rect();

	QPainter p(this);
	auto icon_size = m_iconPixmap.size();
	p.drawPixmap(QRect(0, (btn_rect.height()- icon_size.height())/2, icon_size.width(), icon_size.height()), m_iconPixmap);

	if (_is_select) {
		p.setBrush(QBrush(QColor("#BD00FF")));
		auto line_size = m_linePixmap.size();
		p.drawPixmap(QRect(icon_size.width() + 6, btn_rect.height()-8, line_size.width(), line_size.height()), m_linePixmap);

		auto right_size = m_rightPixmap.size();
		p.drawPixmap(QRect(btn_rect.width() - right_size.width() - 10, (btn_rect.height() - right_size.height())/2, right_size.width(), right_size.height()), m_rightPixmap);
		p.setPen(QColor("#BD00FF"));
	}
	else {
		p.setPen(QColor(0, 0, 0));
		p.setBrush(QBrush(QColor(0, 0, 0)));
	}
	p.drawText(QRectF(icon_size.width()+6, (btn_rect.height() - _height) / 2, _width, _height), m_strText);

}
