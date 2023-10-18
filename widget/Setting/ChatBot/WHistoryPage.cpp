#include "WHistoryPage.h"
#include "function/Bussiness/SettingInterfaceBussiness.h"
#include "function/Bussiness/historyListModel.h"
#include "widget/Setting/WChatHistoryDelegate.h"
#include "function/AiSound.h"
#include "../WConformWidget.h"


WHistoryPage::WHistoryPage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.lineEdit->setPlaceholderText(tr("Search any record"));
    connect(SettingInterfaceBussiness::getInstance(),&SettingInterfaceBussiness::sig_chatHistoryReplay,this, &WHistoryPage::slot_chatHistoryReplay);
    connect(SettingInterfaceBussiness::getInstance(),&SettingInterfaceBussiness::sig_common_replay,this,&WHistoryPage::slot_commonReplay);

    _history_model = new historyListModel(nullptr);

    ui.tableView->setModel(_history_model);
    ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _history_delegate = new WHistoryDelegate(this);
    ui.tableView->setItemDelegateForColumn(5, _history_delegate);

    ui.tableView->setShowGrid(false);
    ui.tableView->setAlternatingRowColors(true);
    ui.tableView->verticalHeader()->setDefaultSectionSize(44);
    ui.tableView->setColumnWidth(0, 70);
    ui.tableView->setColumnWidth(1, 120);
    ui.tableView->setColumnWidth(2, 120);
    ui.tableView->setColumnWidth(3, 160);
    ui.tableView->setColumnWidth(4, 140);
    ui.tableView->setColumnWidth(5, 80);

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
        userOpe(type,btn_index);
        });

    connect(ui.widget, &WPageCtlWidget::sig_changePage, this, &WHistoryPage::slot_changePage);
    connect(ui.lineEdit, &QLineEdit::returnPressed, this, &WHistoryPage::on_pb_search_clicked);
    _conform_widget = new WConformWidget(nullptr);
    connect(_conform_widget, &WConformWidget::sig_conform, this, [=] {
        SettingInterfaceBussiness::getInstance()->delChatHsitory(_select_id);
        });
}

WHistoryPage::~WHistoryPage()
{
    if (_history_model) {
        delete _history_model;
        _history_model = nullptr;
    }
    if (_conform_widget) {
        delete _conform_widget;
        _conform_widget = nullptr;
    }

    ui.tableView->viewport()->removeEventFilter(this);
}

void WHistoryPage::slot_commonReplay(int type, bool success , const QString& msg)
{
    if (type == httpReqType::ChatHistory_Req) {
        if (!success) {
            AiSound::GetInstance().ShowTip(this,msg);
        }
    }
    else if (type == Del_ChatHistory) {
        if (!success) {
            AiSound::GetInstance().ShowTip(this, msg);
        }
        else {
            reqChatHistory();
        }
    }
}

void WHistoryPage::userOpe(int type,int index)
{
    if (index < chat_list_.size()) {
        strc_ChatHistory chat_history = chat_list_.at(index);
        if (type == 0) {
            AiSound::GetInstance().ShowRobotChat(chat_history.conversationId);
        }
        else {
            _select_id.clear();
            _select_id << QString::number(chat_history.chatHistoryId);

            _conform_widget->ShowConform(tr("Are you sure you want to delete the historical synopsis?"));
            //SettingInterfaceBussiness::getInstance()->delChatHsitory(list);
        }
    }
}

void WHistoryPage::reqChatHistory()
{
    QString search = ui.lineEdit->text();
    SettingInterfaceBussiness::getInstance()->getChatHistoryReq(_cur_page, search, _page_size);
}

void WHistoryPage::slot_chatHistoryReplay(bool success, int, const strc_PageInfo& page, const  QVector<strc_ChatHistory>& chat_info)
{
    if (success) {
        chat_list_ = chat_info;
        _history_model->updateData(chat_info);

        _total_pages = page.total_pages;
        _total_size = page.total_size;
        _cur_page = page.cur_page;
        ui.widget->initCtl(_total_pages, _total_size, _cur_page);
    }
}

void WHistoryPage::slot_changePage(int index)
{
    QString search = ui.lineEdit->text();
    SettingInterfaceBussiness::getInstance()->getChatHistoryReq(index, search, _page_size);
}

void WHistoryPage::on_pb_search_clicked()
{
    reqChatHistory();
}