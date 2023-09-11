#include "WHistoryDiaPage.h"
#include "Bussiness/soundHistoryModel.h"
#include "Bussiness/SettingInterfaceBussiness.h"
#include "widget/Setting/WChatHistoryDelegate.h"
#include "function/AiSound.h"
#include "../WConformWidget.h"


WHistoryDiaPage::WHistoryDiaPage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.le_search->setPlaceholderText(tr("Search anr record"));
    this->installEventFilter(this);
    _sound_model = new soundHistoryModel(this);

    ui.tableView->setModel(_sound_model);
    ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _history_delegate = new WHistoryDelegate(this);
    ui.tableView->setItemDelegateForColumn(3, _history_delegate);

    ui.tableView->setShowGrid(false);
    ui.tableView->setAlternatingRowColors(true);

    ui.tableView->verticalHeader()->setDefaultSectionSize(44);
    ui.tableView->setColumnWidth(0, 90);
    ui.tableView->setColumnWidth(1, 160);
    ui.tableView->setColumnWidth(2, 160);
    ui.tableView->setColumnWidth(3, 80);

    ui.tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui.tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui.tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui.tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);

    ui.tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui.tableView->setSelectionBehavior(QTableView::SelectRows);
    ui.tableView->setSelectionMode(QTableView::SingleSelection);

    ui.tableView->verticalHeader()->hide();

    ui.tableView->viewport()->installEventFilter(this);

    connect(_history_delegate, &WHistoryDelegate::sig_opeData, this, [=](const QModelIndex& index, int type) {
        int btn_index = index.row();
        opeHistoryItem(type,btn_index);
        });

    connect(ui.widget_page, &WPageCtlWidget::sig_changePage, this, &WHistoryDiaPage::slot_changePage);
    connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_common_replay, this, &WHistoryDiaPage::slot_commonReplay);
    connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_transHistoryReplay,this, &WHistoryDiaPage::slot_transHistoryReplay);
    connect(ui.le_search, &QLineEdit::returnPressed, this, &WHistoryDiaPage::on_pb_search_clicked);

    _conform_widget = new WConformWidget(nullptr);
    connect(_conform_widget, &WConformWidget::sig_conform, this, [=] {
        SettingInterfaceBussiness::getInstance()->delTransId(_select_id);
        });
}

WHistoryDiaPage::~WHistoryDiaPage()
{
    if (_conform_widget) {
        delete _conform_widget;
        _conform_widget = nullptr;
    }
}

void WHistoryDiaPage::on_pb_search_clicked()
{
    getTransHistory();
}

void WHistoryDiaPage::slot_commonReplay(int type, bool success, const QString& msg)
{
    if (type == httpReqType::DelTrans_Req) {
        if (success) {
            getTransHistory();
        }
        else {
            AiSound::GetInstance().ShowTip(this,msg);
        }
    }
}

void WHistoryDiaPage::opeHistoryItem(int type,int index)
{
    if (type == 0) {

    }
    else {
        _select_id = _trans_info_list.at(index).id;
        _conform_widget->ShowConform(tr("Are you sure you want to delete the Translation History?"));
        //SettingInterfaceBussiness::getInstance()->delTransId(_trans_info_list.at(index).id);
    }
}

void WHistoryDiaPage::getTransHistory()
{
    QString content = ui.le_search->text();
    SettingInterfaceBussiness::getInstance()->getTransHistory(_page_size,_cur_page, content);
}

void WHistoryDiaPage::slot_changePage(int index)
{
    QString content = ui.le_search->text();
    SettingInterfaceBussiness::getInstance()->getTransHistory(_page_size, index, content);
}

void WHistoryDiaPage::slot_transHistoryReplay(bool success, int type, const strc_PageInfo& page, const  QVector<strc_transHistory>& trans_list)
{
    _trans_info_list = trans_list;
    _sound_model->updateData(trans_list);
    ui.widget_page->initCtl(page.total_pages,page.total_size,page.cur_page);
}

//void WHistoryDiaPage::keyReleaseEvent(QKeyEvent* event)
//{
//    if (event->key() == Qt::Key_Return)
//    {
//        on_pb_search_clicked();
//    }
//}