#pragma once

#include <QObject>
#include "function/Bussiness/Define.h"
#include <QAbstractTableModel>
#include <QSharedPointer>
#include <QList>


class soundHistoryModel  : public QAbstractTableModel
{
	Q_OBJECT

public:
	soundHistoryModel(QObject *parent=nullptr);
	~soundHistoryModel();

	void updateData(const QVector<struc_teamInfo>& team_list);

protected:
	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;

private:
	QStringList _headerList;
	QList<QSharedPointer<struc_teamInfo>> _list;
};
