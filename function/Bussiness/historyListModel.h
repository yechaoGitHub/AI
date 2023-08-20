#pragma once

#include <QObject>
#include "function/Bussiness/Define.h"
#include <QAbstractTableModel>
#include <QSharedPointer>
#include <QList>


class historyListModel  : public QAbstractTableModel
{
	Q_OBJECT

public:
	historyListModel(QObject *parent=nullptr);
	~historyListModel();

	void updateData(const QVector<strc_ChatHistory>& team_list);

protected:
	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;

private:
	QStringList _headerList;
	QList<QSharedPointer<strc_ChatHistory>> _list;
};
