#include "soundHistoryModel.h"


soundHistoryModel::soundHistoryModel(QObject* parent)
    :QAbstractTableModel(parent)
    , _headerList(QStringList() << tr("    Id") << tr(" Name") << tr(" Translation Type") << tr(" Actions"))
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
            if (_list[r]->id < 10) {
                return QString("     %1").arg(_list[r]->id);
            }
            else {
                return QString("    %1").arg(_list[r]->id);
            }
        }
        else if (c == 1)
            return QString(" %1").arg(_list[r]->transName);
        else if (c == 2)
            return QString(" %1").arg(_list[r]->transTypeName);
    }
    return QVariant();
}

QVariant soundHistoryModel::headerData(int section, Qt::Orientation orientation,
    int role) const
{//_headerList(QStringList() << tr("     Id") << tr(" Name") << tr(" Translation Type") << tr(" Actions"))
    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("     Id");
        }
        else if (section == 1) {
            return tr(" Name");
        }
        else if (section == 2) {
            return tr(" Translation Type");
        }
        else if (section == 3) {
            return tr(" Actions");
        }
        else {
            if (section < _headerList.count())
                return _headerList[section];
            else
                return QVariant();
        }
    }
    else
        return QAbstractTableModel::headerData(section, orientation, role);
    return QVariant();
}