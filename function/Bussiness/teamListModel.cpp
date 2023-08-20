#include "teamListModel.h"


teamListModel::teamListModel(QObject* parent)
    :QAbstractTableModel(parent)
    , _headerList(QStringList() << tr(" Name") << tr(" Status") << tr(" Credits Used") << tr(" Date and Time") << tr(" Actions"))
{
}

void teamListModel::updateData(const QVector<struc_teamInfo>& team_list)
{
    this->beginResetModel();
    _list.clear();
    for (auto it : team_list) {
        QSharedPointer<struc_teamInfo> service = QSharedPointer<struc_teamInfo>(new struc_teamInfo);
        service->username = it.username;
        service->credits_used = it.credits_used;
        service->join_time = it.join_time;
        service->status = it.status;
        service->userId = it.userId;
        _list.push_back(std::move(service));
    }
    this->endResetModel();
}

teamListModel::~teamListModel()
{}

int teamListModel::rowCount(const QModelIndex& parent) const
{
    return _list.count();
}

int teamListModel::columnCount(const QModelIndex& parent) const
{
    return _headerList.count();
}

QVariant teamListModel::data(const QModelIndex& index, int role) const
{
    int r = index.row();
    int c = index.column();
    if (!index.isValid() || r >= _list.size() || c >= _headerList.count())
        return QVariant();
    if (role == Qt::DisplayRole) {
        if (c == 0) {
            return _list[r]->username;
        }
        else if (c == 1)
            return tr("In Team");
        else if (c == 2)
            return QString::number(_list[r]->credits_used,'f',2);
        else if (c == 3)
            return _list[r]->join_time;
    }
    return QVariant();
}

QVariant teamListModel::headerData(int section, Qt::Orientation orientation,
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