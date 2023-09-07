#include "WSoundLine.h"
#include <qpainter.h>
#include <QPainterPath>


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
	p.setRenderHint(QPainter::Antialiasing);
	//pen.setColor("qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #0066FF, stop:1 #BD00FF)");
	p.setPen(pen);

	int range = 14;
	int start = 10;
	int total = frame_rect.width() / range;

	QPainterPath path;
	for (int i = 0; i < total; i++) {
		if (i * range + 2 + start + 50 > frame_rect.width()) {
			break;
		}
		path.addRoundedRect(QRectF(i * range + 2 + start,1,3, frame_rect.height() - 2),4, 4);
		//p.drawLine(QPoint(i*range+2+ start,1), QPoint(i * range + 2+10, frame_rect.height()-2));
		p.drawPath(path);

	}
}