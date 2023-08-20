#pragma once

#include <QPushButton>
#include <QStyledItemDelegate>

class WTeamDelegate  : public QStyledItemDelegate
{
	Q_OBJECT
signals:
	void sig_deleteData(QModelIndex);
public:
	WTeamDelegate(QWidget*parent);
	~WTeamDelegate();

protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);
};
