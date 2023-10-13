#include "WInvitePage.h"
#include "function/Bussiness/SettingInterfaceBussiness.h"
#include <qscrollbar.h>
#include "function/Bussiness/teamListModel.h"
#include "WTeamDelegate.h"
#include <QTimer>
#include "model/WPageCtlWidget.h"
#include "function/AiSound.h"



WInvitePage::WInvitePage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.pb_invite->setText(tr("Invite member"));
    ui.le_invite->setPlaceholderText(tr("Enter user name to add"));
    ui.le_search->setPlaceholderText(tr("Search any record"));

    qRegisterMetaType< QVector<struc_teamInfo>>(" QVector<struc_teamInfo>");
    connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_common_replay,this, &WInvitePage::slot_inviteReplay);
    connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_searchTeam_replay, this, &WInvitePage::slot_searchTeam_replay);

    _team_model = new teamListModel(nullptr);

    ui.tableView->setModel(_team_model);
    ui.tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _team_delegate = new WTeamDelegate(this);
    ui.tableView->setItemDelegateForColumn(4, _team_delegate);

    ui.tableView->setShowGrid(false);
    ui.tableView->setAlternatingRowColors(true);

    ui.tableView->verticalHeader()->setDefaultSectionSize(44);
    ui.tableView->setColumnWidth(0, 120);
    ui.tableView->setColumnWidth(1, 100);
    ui.tableView->setColumnWidth(2, 100);
    ui.tableView->setColumnWidth(3, 200);
    ui.tableView->setColumnWidth(4, 70);

    ui.tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui.tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui.tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui.tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui.tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);

    ui.tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui.tableView->setSelectionBehavior(QTableView::SelectRows);
    ui.tableView->setSelectionMode(QTableView::SingleSelection);

    ui.tableView->verticalHeader()->hide();

    ui.tableView->viewport()->installEventFilter(this);

    connect(_team_delegate, &WTeamDelegate::sig_deleteData, this, [=](const QModelIndex& index) {
        int btn_index = index.row();
        if (btn_index < _teal_list.size()) {
            SettingInterfaceBussiness::getInstance()->removeTeamReq(_teal_list.at(btn_index).userId);
        }
        });

    connect(ui.widget,&WPageCtlWidget::sig_changePage,this, &WInvitePage::slot_changePage);
}

WInvitePage::~WInvitePage()
{
    if (_team_model) {
        delete _team_model;
        _team_model = nullptr;
    }
}

void WInvitePage::slot_changePage(int index)
{
    SettingInterfaceBussiness::getInstance()->getTeamRecordReq(index, _page_size, "");
}

void WInvitePage::reqTeamData()
{
    SettingInterfaceBussiness::getInstance()->getTeamRecordReq(_cur_page, _page_size, "");
}

void WInvitePage::slot_inviteReplay(int type, bool success, const QString& msg)
{
    if (type == httpReqType::InviteUser_Req) {
        AiSound::GetInstance().ShowTip(this, msg);
    }
    else if (type == httpReqType::Search_Team) {

    }
    else if (type == httpReqType::Remove_Team) {
        if (success) {
            QTimer::singleShot(200, this, [=]() {
                reqTeamData();
                });
        }
        else {
            AiSound::GetInstance().ShowTip(this, msg);
        }
    }
}

void WInvitePage::slot_searchTeam_replay(int pages, int total, int cur, const QVector<struc_teamInfo>& teams)
{
    if (_team_model) {
        _teal_list = teams;
        _team_model->updateData(teams);
    }
    _cur_page = cur;
    _total_pages = pages;
    _total_size = total;

    ui.widget->initCtl(_total_pages, _total_size, _cur_page);
}

void WInvitePage::on_pb_invite_clicked()
{
    QString invite_user = ui.le_invite->text();
    if (invite_user.isEmpty()) {
        return;
    }
    ui.le_invite->clear();
    SettingInterfaceBussiness::getInstance()->inviteUserJoinReq(invite_user);
}

void WInvitePage::on_pb_search_clicked()
{
    QString content = ui.le_search->text();

    SettingInterfaceBussiness::getInstance()->getTeamRecordReq(1, _page_size, content);
}