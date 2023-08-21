#pragma once

#include <QObject>
#include <QPaintEvent>
#include <qwidget.h>


class WSoundLine  : public QWidget{
	Q_OBJECT

public:
	WSoundLine(QWidget *parent);
	~WSoundLine();

protected:
	void paintEvent(QPaintEvent* event);
};
