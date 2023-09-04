#include "soundHistoryModel.h"


soundHistoryModel::soundHistoryModel(QObject* parent)
    :QAbstractTableModel(parent)
    , _headerList(QStringList() << tr(" Id") << tr(" Name") << tr(" Translation Type") << tr(" Actions"))
{
}

void soundHistoryModel::updateData(const QVector<strc_transHistory>& team_list)
{
    this->beginResetModel();
    _list.clear();
    for (auto it : team_list) {
        QSharedPointer<strc_transHistory> service = QSharedPointer<strc_transHistory>(new strc_transHistory);
        service->id = it.id;
        service->transName = it.transName;
        service->transTypeName = it.transTypeName;
        service->updateTime = it.updateTime;
        _list.push_back(std::move(service));
    }
    this->endResetModel();
}

soundHistoryModel::~soundHistoryModel()
{}

int soundHistoryModel::rowCount(const QModelIndex& parent) const
{
    return _list.count();
}

int soundHistoryModel::columnCount(const QModelIndex& parent) const
{
    return _headerList.count();
}

QVariant soundHistoryModel::data(const QModelIndex& index, int role) const
{
    int r = index.row();
    int c = index.column();
    if (!index.isValid() || r >= _list.size() || c >= _headerList.count())
        return QVariant();
    if (role == Qt::DisplayRole) {
        if (c == 0) {
            return QString::number(_list[r]->id);
        }
        else if (c == 1)
            return _list[r]->transName;
        else if (c == 2)
            return _list[r]->transTypeName;
    }
    return QVariant();
}

QVariant soundHistoryModel::headerData(int section, Qt::Orientation orientation,
    int role) const
{
    if (role == Qt::DisplayRole) {
        if (section < _headerList.count())
            return _headerList[section];
        else
            return QVariant();
    }
    else
        return QAbstractTableModel::headerData(section, orientation, role);
    return QVariant();
}