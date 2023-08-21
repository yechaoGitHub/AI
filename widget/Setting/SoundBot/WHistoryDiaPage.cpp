#include "WHistoryDiaPage.h"
#include "Bussiness/soundHistoryModel.h"

WHistoryDiaPage::WHistoryDiaPage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.le_search->setPlaceholderText(tr("Search anr record"));

    _sound_model = new soundHistoryModel(nullptr);

    ui.tableView->setModel(_sound_model);
    ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //_history_delegate = new WHistoryDelegate(this);
    //ui.tableView->setItemDelegateForColumn(4, _history_delegate);

    ui.tableView->setShowGrid(false);

    ui.tableView->verticalHeader()->setDefaultSectionSize(44);
    ui.tableView->setColumnWidth(0, 90);
    ui.tableView->setColumnWidth(1, 160);
    ui.tableView->setColumnWidth(2, 160);
    ui.tableView->setColumnWidth(3, 120);
    ui.tableView->setColumnWidth(4, 100);

    ui.tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui.tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui.tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui.tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui.tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

    ui.tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui.tableView->setSelectionBehavior(QTableView::SelectRows);
    ui.tableView->setSelectionMode(QTableView::SingleSelection);

    ui.tableView->verticalHeader()->hide();

    ui.tableView->viewport()->installEventFilter(this);

   /* connect(_history_delegate, &WHistoryDelegate::sig_opeData, this, [=](const QModelIndex& index, int type) {
        int btn_index = index.row();

        });*/

    connect(ui.widget_page, &WPageCtlWidget::sig_changePage, this, &WHistoryDiaPage::slot_changePage);
}

WHistoryDiaPage::~WHistoryDiaPage()
{}

void WHistoryDiaPage::slot_changePage(int index)
{

}