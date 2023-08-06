#include "WSpeechGenerationUi.h"
#include <QPainter>
#include <QListView>


WSpeechGenerationUi::WSpeechGenerationUi(QWidget *parent)
	: FrameLessWidget(parent)
{
	ui.setupUi(this);
	this->setWidgetType(true, DragType::Drag_Null, false);
	setAttribute(Qt::WA_TranslucentBackground);

	ui.comboBox_lang->setView(new  QListView());
	ui.comboBox_lang->addItem("English");
	ui.comboBox_lang->addItem("Chinese");

	ui.comboBox_sex->setView(new  QListView());
	ui.comboBox_sex->addItem("Male");
	ui.comboBox_sex->addItem("Woman");

	ui.comboBox_vector->setView(new  QListView());
	ui.comboBox_vector->addItem("David John");
	ui.comboBox_vector->addItem("Joke");
}

WSpeechGenerationUi::~WSpeechGenerationUi()
{}

void WSpeechGenerationUi::on_pb_close_clicked()
{
	close();
}

void WSpeechGenerationUi::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	QRect frameRect = rect();
	p.setPen(QColor(0, 0, 0, 204));
	p.setBrush(QColor(0, 0, 0, 204));
	p.drawRoundRect(frameRect, 6, 6);
}