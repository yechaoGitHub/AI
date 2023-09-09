#include "historyListModel.h"


historyListModel::historyListModel(QObject* parent)
    :QAbstractTableModel(parent)
    , _headerList(QStringList() << tr("     ID") << tr(" Chat Name") << tr(" Bot Template") << tr(" Date and Time") << tr(" Synopsis") << tr(" Actions"))
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
        QString syn = it.synopsis.toLocal8Bit();
        if (syn.length() > 10) {
            service->synopsis = QString("%1...").arg(it.synopsis.left(8));
        }
        //service->synopsis = it.synopsis;
        service->chatName = it.chatName;
        service->templateName = it.templateName;
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
            if (_list[r]->chatHistoryId < 10) {
                return QString("      %1").arg(_list[r]->chatHistoryId);
            }
            else {
                return QString("     %1").arg(_list[r]->chatHistoryId);
            }

        }
        else if (c == 1)
            return QString(" %1").arg(_list[r]->chatName);
        else if (c == 2)
            return QString(" %1").arg(_list[r]->templateName);
        else if (c == 3)
            return QString(" %1").arg(_list[r]->initTime);
        else if (c == 4)
            return QString(" %1").arg(_list[r]->synopsis);
    }
    return QVariant();
}

QVariant historyListModel::headerData(int section, Qt::Orientation orientation,
    int role) const
{//_headerList(QStringList() << tr("     ID") << tr(" Chat Name") << tr(" Bot Template") << tr(" Date and Time") << tr(" Synopsis") << tr(" Actions"))
    if (role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("     ID");
        }
        else if (section == 1) {
            return tr(" Chat Name");
        }
        else if (section == 2) {
            return tr(" Bot Template");
        }
        else if (section == 3) {
            return tr(" Date and Time");
        }
        else if (section == 3) {
            return tr(" Synopsis");
        }
        else if (section == 4) {
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