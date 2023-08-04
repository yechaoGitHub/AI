#include "WSpeechGenerationUi.h"
#include <QPainter>


WSpeechGenerationUi::WSpeechGenerationUi(QWidget *parent)
	: FrameLessWidget(parent)
{
	ui.setupUi(this);
	this->setWidgetType(true, DragType::Drag_Null, false);
	setAttribute(Qt::WA_TranslucentBackground);
}

WSpeechGenerationUi::~WSpeechGenerationUi()
{}

void WSpeechGenerationUi::paintEvent(QPaintEvent* event)
{
	QPainter p(this);
	QRect frameRect = rect();
	p.setPen(QColor(0, 0, 0, 204));
	p.setBrush(QColor(0, 0, 0, 204));
	p.drawRoundRect(frameRect, 6, 6);
}