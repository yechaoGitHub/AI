#include "historyListModel.h"


historyListModel::historyListModel(QObject* parent)
    :QAbstractTableModel(parent)
    , _headerList(QStringList() << tr(" ID") << tr(" Chat Name") << tr(" Bot Type") << tr(" Date and Time") << tr(" Chat") << tr(" Actions"))
{
}

void historyListModel::updateData(const QVector<strc_ChatHistory>& team_list)
{
    this->beginResetModel();
    _list.clear();
    for (auto it : team_list) {
        QSharedPointer<strc_ChatHistory> service = QSharedPointer<strc_ChatHistory>(new strc_ChatHistory);
        service->chatHistoryId = it.chatHistoryId;
        service->chatType = it.chatType;
        service->content = it.content;
        service->initTime = it.initTime;
        service->receiverId = it.receiverId;
        service->senderId = it.senderId;
        _list.push_back(std::move(service));
    }
    this->endResetModel();
}

historyListModel::~historyListModel()
{}

int historyListModel::rowCount(const QModelIndex& parent) const
{
    return _list.count();
}

int historyListModel::columnCount(const QModelIndex& parent) const
{
    return _headerList.count();
}

QVariant historyListModel::data(const QModelIndex& index, int role) const
{
    int r = index.row();
    int c = index.column();
    if (!index.isValid() || r >= _list.size() || c >= _headerList.count())
        return QVariant();
    if (role == Qt::DisplayRole) {
        if (c == 0) {
            return QString::number(_list[r]->chatHistoryId);
        }
        else if (c == 1)
            return _list[r]->senderId;
        else if (c == 2)
            return QString::number(_list[r]->chatType);
        else if (c == 3)
            return _list[r]->initTime;
        else if (c == 4)
            return _list[r]->receiverId;
    }
    return QVariant();
}

QVariant historyListModel::headerData(int section, Qt::Orientation orientation,
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