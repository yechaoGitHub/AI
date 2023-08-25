#include "WHistoryPage.h"
#include "function/Bussiness/SettingInterfaceBussiness.h"
#include "function/Bussiness/historyListModel.h"
#include "widget/Setting/WChatHistoryDelegate.h"


WHistoryPage::WHistoryPage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.lineEdit->setPlaceholderText(tr("Search any record"));
    connect(SettingInterfaceBussiness::getInstance(),&SettingInterfaceBussiness::sig_chatHistoryReplay,this, &WHistoryPage::slot_chatHistoryReplay);

    _history_model = new historyListModel(nullptr);

    ui.tableView->setModel(_history_model);
    ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _history_delegate = new WHistoryDelegate(this);
    ui.tableView->setItemDelegateForColumn(5, _history_delegate);

    ui.tableView->setShowGrid(false);

    ui.tableView->verticalHeader()->setDefaultSectionSize(44);
    ui.tableView->setColumnWidth(0, 90);
    ui.tableView->setColumnWidth(1, 120);
    ui.tableView->setColumnWidth(2, 100);
    ui.tableView->setColumnWidth(3, 180);
    ui.tableView->setColumnWidth(4, 120);
    ui.tableView->setColumnWidth(5, 100);

    ui.tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui.tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui.tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui.tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui.tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui.tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);

    ui.tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui.tableView->setSelectionBehavior(QTableView::SelectRows);
    ui.tableView->setSelectionMode(QTableView::SingleSelection);

    ui.tableView->verticalHeader()->hide();

    ui.tableView->viewport()->installEventFilter(this);

    connect(_history_delegate, &WHistoryDelegate::sig_opeData, this, [=](const QModelIndex& index,int type) {
        int btn_index = index.row();
        userOpe(btn_index);
        });

    connect(ui.widget, &WPageCtlWidget::sig_changePage, this, &WHistoryPage::slot_changePage);
}

WHistoryPage::~WHistoryPage()
{
    if (_history_model) {
        delete _history_model;
        _history_model = nullptr;
    }
}

void WHistoryPage::userOpe(int type)
{
    if (type == 0) {

    }
    else if (type == 1) {

    }
    else {

    }
}

void WHistoryPage::reqChatHistory()
{
    SettingInterfaceBussiness::getInstance()->getCharHistoryReq(1, _cur_page,"",10);
}

void WHistoryPage::slot_chatHistoryReplay(bool success, int, const QString& msg, const  QVector<strc_ChatHistory>& chat_info)
{
    if (success) {
        _history_model->updateData(chat_info);
    }
}

void WHistoryPage::slot_changePage(int index)
{

}

void WHistoryPage::on_pb_search_clicked()
{
    QString search = ui.lineEdit->text();
    if (search.isEmpty()) {
        return;
    }
    SettingInterfaceBussiness::getInstance()->getCharHistoryReq(1, _cur_page, search, 10);
}