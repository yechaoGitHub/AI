#include "WSoundLine.h"
#include <qpainter.h>


WSoundLine::WSoundLine(QWidget *parent)
	: QWidget(parent)
{}

WSoundLine::~WSoundLine()
{}

void WSoundLine::paintEvent(QPaintEvent* event)
{
	auto frame_rect = rect();
	QPainter p(this);
	QPen pen;
	pen.setWidth(4);
	pen.setColor("#EDEDED");
	//pen.setColor("qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #0066FF, stop:1 #BD00FF)");
	p.setPen(pen);

	int range = 10;
	int total = frame_rect.width() / range;

	for (int i = 0; i < total; i++) {
		p.drawLine(QPoint(i*range+2,1), QPoint(i * range + 2, frame_rect.height()-2));
	}
}