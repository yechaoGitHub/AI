#include "WLibarary.h"
#include "AiSound.h"
#include "Bussiness/SettingInterfaceBussiness.h"
#include "base/GlobalSetting.h"
#include "widget/chat/WRobotChatMainUI.h"

WLibarary::WLibarary(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    _conform_dlg = new WConformDlg(nullptr);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            WLibModelWidget* lib = new WLibModelWidget(this);
            QSizePolicy sp_retain = lib->sizePolicy();
            sp_retain.setRetainSizeWhenHidden(true);
            lib->setSizePolicy(sp_retain);
            connect(lib, &WLibModelWidget::sig_libModel_click,this,&WLibarary::slot_model_clicked);
            _lib_model_list.push_back(lib);
            ui.gridLayout->addWidget(lib, i,j);
        }
    }
    _select_model = _lib_model_list.at(0);
    connect(ui.widget,&WPageCtlWidget::sig_changePage,this, &WLibarary::slot_page_change);

    qRegisterMetaType<QVector<strc_ChatbotInfo>>("QVector<strc_ChatbotInfo>");
    qRegisterMetaType<strc_PageInfo>("strc_PageInfo");
    connect(SettingInterfaceBussiness::getInstance(), &SettingInterfaceBussiness::sig_getChatBotListReplay, this, &WLibarary::slot_getChatBotListReplay);
}

WLibarary::~WLibarary()
{
    if (_conform_dlg) {
        delete _conform_dlg;
        _conform_dlg = nullptr;
    }
}

void WLibarary::getChatBotTemplate()
{
    SettingInterfaceBussiness::getInstance()->getCharBotListReq(_cur_page, _page_size, _cur_type);
}

void WLibarary::updateLibBySelType(int type)
{
    _cur_type = type;
    getChatBotTemplate();
}

void WLibarary::slot_model_clicked()
{
    int ret = _conform_dlg->exec();
    if (ret == QDialog::Accepted) {
        WLibModelWidget* btn = static_cast<WLibModelWidget*>(QObject::sender());
        if (btn == _select_model && _select_model) {
            _select_model->setSel(true);
            SETTING.setRebotModel(_select_model->getModelId());
            return;
        }
        if (_select_model) {
            _select_model->setSel(false);
        }
        _select_model = btn;
        _select_model->setSel(true);
        SETTING.setRebotModel(_select_model->getModelId());


        auto& bot = AiSound::GetInstance().GetChatBot();
        auto view = AiSound::GetInstance().GetRobotChatView();
        view->Clear();

        if (bot.IsRunning())
        {
            bot.Disconnect();
            auto& token = AiSound::GetInstance().Token();
            auto modelID = SETTING.getRebotModel();
            bot.Connect(token, modelID, "");
        }
    }
}

void WLibarary::slot_page_change(int index)
{
    SettingInterfaceBussiness::getInstance()->getCharBotListReq(index, _page_size, _cur_type);
}

void WLibarary::opeModelswitch(bool open,int id)
{
    QString strList;
    bool exist = false;
    for (auto it : _model_list) {
        if (it == QString::number(id)) {
            exist = true;
            if (!open) {
                continue;
            }
        }
        strList = strList.append(it).append(";");
    }

    if (open && !exist) {
        strList = strList.append(QString::number(id));
        _model_list.push_back(QString::number(id));
    }

    if (!open && exist) {
        _model_list.removeAll(QString::number(id));
    }
}

void WLibarary::slot_getChatBotListReplay(bool success, int, const strc_PageInfo& page_info, const QVector<strc_ChatbotInfo>& chatbot_list)
{
    if (success) {
        int cur_model = SETTING.getRebotModel();
        int list_index = 0;
        for (auto it : chatbot_list) {
            _lib_model_list.at(list_index)->setTitle(/*tr("Chatbot(%1)").arg(*/it.name, it.desc,it.id);
            _lib_model_list.at(list_index)->show();

            if ((cur_model == 0 && list_index == 0)) {
                _lib_model_list.at(list_index)->setSel(true);
                SETTING.setRebotModel(it.id);
                _select_model = _lib_model_list.at(list_index);
            }
            else if (cur_model == it.id && cur_model != 0) {
                _lib_model_list.at(list_index)->setSel(true);
                _select_model = _lib_model_list.at(list_index);
            }
            else {
                _lib_model_list.at(list_index)->setSel(false);
            }

            list_index++;
        }

        for (auto i = list_index; i < _page_size; i++) {
            _lib_model_list.at(i)->hide();
        }

        _total_size = page_info.total_size;
        _pages = page_info.total_pages;
        _cur_page = page_info.cur_page;
        ui.widget->initCtl(_pages, _total_size, _cur_page);
    }
}

void WLibarary::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui.retranslateUi(this);
    }

    QWidget::changeEvent(event);
}