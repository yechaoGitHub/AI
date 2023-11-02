#include "WNavbarButton.h"
#include <QPainter>
#include <QEvent>


WNavbarButton::WNavbarButton(QWidget*parent)
	: QPushButton(parent)
{
	m_linePixmap = QPixmap(":/QtTest/icon/Setting/line.png");
	m_rightPixmap = QPixmap(":/QtTest/icon/Setting/right.png");
	this->setStyleSheet("border:none;font: 16px;color:qlineargradient(spread:pad, x1:0, y1:0, x2:111, y2:20, stop:0 #0066FF, stop:1 #BD00FF)");
	m_pLabel = new QLabel(this);
}

WNavbarButton::~WNavbarButton()
{}

void WNavbarButton::initBar(const QString& text, BarType bar_type)
{
	QFont font = this->font();
	font.setPixelSize(22);
	QFontMetrics fm(font);
	m_pLabel->setFont(font);

	_button_type = bar_type;
	switch (bar_type)
	{
	case WNavbarButton::Bar_Account:
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/account.png");
		m_pLabel->setText(tr("Account"));
		_width = fm.width(tr("Account"));
		break;
	case WNavbarButton::Bar_Help:
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/help.png");
		m_pLabel->setText(tr("Robot"));
		_width = fm.width(tr("Robot"));
		break;
	case WNavbarButton::Bar_Team:
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/team.png");
		m_pLabel->setText(tr("Team"));
		_width = fm.width(tr("Team"));
		break;
	case WNavbarButton::Bar_Tools:
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/tools.png");
		m_pLabel->setText(tr("General Settings"));
		_width = fm.width(tr("General Settings"));
		break;
	case WNavbarButton::Bar_Lib:
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/chatbot/lib_icon.png");
		m_pLabel->setText(tr("Library"));
		_width = fm.width(tr("Library"));
		break;
	case WNavbarButton::Bar_History:
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/chatbot/history_icon.png");
		m_pLabel->setText(tr("History"));
		_width = fm.width(tr("History"));
		break;
	case WNavbarButton::Sound_Page1:
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/soundbot/source_icon.png");
		m_pLabel->setText(tr("Sound Source"));
		_width = fm.width(tr("Sound Source"));
		break;
	case WNavbarButton::Sound_Page2:
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/soundbot/transl_icon.png");
		m_pLabel->setText(tr("Translation"));
		_width = fm.width(tr("Translation"));
		break;
	case WNavbarButton::Sound_Page3:
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/soundbot/voice_icon.png");
		m_pLabel->setText(tr("Voice Library"));
		_width = fm.width(tr("Voice Library"));
		break;
	case WNavbarButton::Sound_Page4:
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/soundbot/my_voice.png");
		m_pLabel->setText(tr("My Voice"));
		_width = fm.width(tr("My Voice"));
		break;
	case WNavbarButton::Sound_Page5:
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/soundbot/speech_icon.png");
		m_pLabel->setText(tr("Speech Synthesis"));
		_width = fm.width(tr("Speech Synthesis"));
		break;
	case WNavbarButton::Sound_Page6:
		m_iconPixmap = QPixmap(":/QtTest/icon/Setting/soundbot/historical.png");
		m_pLabel->setText(tr("Translation History"));
		_width = fm.width(tr("Translation History"));
		break;
	default:
		break;
	}

	m_strText = text;
	_height = fm.height();
}

void WNavbarButton::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::LanguageChange)
	{
		//ui.retranslateUi(this);
		initBar(m_strText, _button_type);
		update();
	}

	QWidget::changeEvent(event);
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
	p.drawPixmap(QRect(0, (btn_rect.height() - icon_size.height()) / 2 - 2, icon_size.width(), icon_size.height()), m_iconPixmap);

	if (_is_select) {
		//p.setBrush(QBrush(QColor("#BD00FF")));
		auto line_size = m_linePixmap.size();
		if (_button_type == BarType::Bar_Lib || _button_type == Bar_History) {
			p.drawPixmap(QRect(icon_size.width() + 6, btn_rect.height() - 8, line_size.width()-16, line_size.height()), m_linePixmap);
		}
		else {
			p.drawPixmap(QRect(icon_size.width() + 6, btn_rect.height() - 8, line_size.width(), line_size.height()), m_linePixmap);
		}

		auto right_size = m_rightPixmap.size();

		if (_button_type == BarType::Bar_Lib || _button_type == Bar_History || _button_type == Sound_Page1 || _button_type == Sound_Page2 || _button_type == Sound_Page3 || _button_type == Sound_Page4) {
			p.drawPixmap(QRect(btn_rect.width() - right_size.width()-16, (btn_rect.height() - right_size.height()) / 2, right_size.width(), right_size.height()), m_rightPixmap);
		}
		else if (_button_type == Sound_Page5) {
			p.drawPixmap(QRect(btn_rect.width() - right_size.width() - 10, (btn_rect.height() - right_size.height()) / 2, right_size.width(), right_size.height()), m_rightPixmap);
		}
		else {
			p.drawPixmap(QRect(btn_rect.width() - right_size.width(), (btn_rect.height() - right_size.height()) / 2, right_size.width(), right_size.height()), m_rightPixmap);
		}

		////线性渐变
		//QLinearGradient linearGradient(QPointF(icon_size.width() + 6, (btn_rect.height() - _height) / 2 - 2), QPointF(icon_size.width() + 6+ _width, (btn_rect.height() - _height) / 2 - 2));
		////插入颜色
		///*linearGradient.setColorAt(0, QColor("#0066FF"));
		//linearGradient.setColorAt(0.5, QColor("#BD00FF"));
		//linearGradient.setColorAt(1, QColor("#BD00FF"));*/

		//double s = 6;
		//linearGradient.setColorAt(0 / s, Qt::yellow);
		//linearGradient.setColorAt(1 / s, Qt::green);
		//linearGradient.setColorAt(2 / s, Qt::blue);
		//linearGradient.setColorAt(3 / s, Qt::red);
		//linearGradient.setColorAt(4 / s, Qt::magenta);
		//linearGradient.setColorAt(5 / s, Qt::cyan);
		//linearGradient.setColorAt(6 / s, Qt::white);
		///*linearGradient.setColorAt(0, QColor("#0066FF"));
		//linearGradient.setColorAt(0.5, QColor("#845725"));
		//linearGradient.setColorAt(1, QColor("#BD00FF"));*/

		////指定渐变区域以外的区域的扩散方式
		//linearGradient.setSpread(QGradient::RepeatSpread);
		////使用渐变作为画刷
		//p.setBrush(linearGradient);
		//p.setPen(QColor("#BD00FF"));

		m_pLabel->move(icon_size.width() + 6, (btn_rect.height() - _height) / 2 - 2);
		m_pLabel->show();
	}
	else {
		p.setPen(QColor(0, 0, 0));
		p.setBrush(QBrush(QColor(0, 0, 0)));
		m_pLabel->hide();
		p.drawText(QRectF(icon_size.width()+6, (btn_rect.height() - _height) / 2 - 2, _width, _height), m_strText);
	}
}
