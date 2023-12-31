#pragma once

#include <QPushButton>
#include <QStyledItemDelegate>

class WHistoryDelegate  : public QStyledItemDelegate
{
	Q_OBJECT
signals:
	void sig_opeData(QModelIndex index,int type);
public:
	WHistoryDelegate(QWidget*parent,int type = 1);
	~WHistoryDelegate();

protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);

private:
	QPoint m_mousePoint;
	int _delegate_type = 1;
};
