#include "WTranslationPage.h"
#include <qpushbutton.h>
#include "base/GlobalSetting.h"
#include "AiSound.h"
#include "WTranslationMain.h"

WTranslationPage::WTranslationPage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.ck_bk_1,&QCheckBox::clicked,this,&WTranslationPage::slot_bk_clicked);
    connect(ui.ck_bk_2, &QCheckBox::clicked, this, &WTranslationPage::slot_bk_clicked);
    ui.ck_bk_2->setColor(QColor("#C9C9C9"));

    if (SETTING.getTransBk()) {
        ui.ck_bk_1->setSel(true);
    }
    else {
        ui.ck_bk_2->setSel(true);
    }

    connect(ui.ck_tl_1, &QCheckBox::clicked, this, &WTranslationPage::slot_tl_clicked);
    connect(ui.ck_tl_2, &QCheckBox::clicked, this, &WTranslationPage::slot_tl_clicked);
    connect(ui.ck_tl_3, &QCheckBox::clicked, this, &WTranslationPage::slot_tl_clicked);
    connect(ui.ck_tl_4, &QCheckBox::clicked, this, &WTranslationPage::slot_tl_clicked);
    _tl_ck_list_.push_back(ui.ck_tl_1);
    _tl_ck_list_.push_back(ui.ck_tl_2);
    _tl_ck_list_.push_back(ui.ck_tl_3);
    _tl_ck_list_.push_back(ui.ck_tl_4);
    int value = SETTING.getTransTl();
    for (int i = 0; i < _tl_ck_list_.size(); i++) {
        int index = i + 1;
        _tl_ck_list_.at(i)->setSel(value == index);
    }


    ui.ck_tl_2->setColor(QColor("#00D7F4"));
    ui.ck_tl_3->setColor(QColor("#00A9FF"));
    ui.ck_tl_4->setColor(QColor("#6BFBCE"));

    connect(ui.ck_og_1, &QCheckBox::clicked, this, &WTranslationPage::slot_og_clicked);
    connect(ui.ck_og_2, &QCheckBox::clicked, this, &WTranslationPage::slot_og_clicked);
    connect(ui.ck_og_3, &QCheckBox::clicked, this, &WTranslationPage::slot_og_clicked);
    connect(ui.ck_og_4, &QCheckBox::clicked, this, &WTranslationPage::slot_og_clicked);
    _og_ck_list_.push_back(ui.ck_og_1);
    _og_ck_list_.push_back(ui.ck_og_2);
    _og_ck_list_.push_back(ui.ck_og_3);
    _og_ck_list_.push_back(ui.ck_og_4);

    int value_1 = SETTING.getTransOg();
    for (int i = 0; i < _og_ck_list_.size(); i++) {
        int index = i + 1;
        _og_ck_list_.at(i)->setSel(value_1 ==index);
    }

    ui.ck_og_2->setColor(QColor("#00D7F4"));
    ui.ck_og_3->setColor(QColor("#00A9FF"));
    ui.ck_og_4->setColor(QColor("#6BFBCE"));

    if (SETTING.getTransTT() == 1) {
        ui.ck_tr_1->setChecked(true);
    }
    else {
        ui.ck_tr_2->setChecked(true);
    }
    if (SETTING.getTransSD() == 1) {
        ui.ck_sd_1->setChecked(true);
    }
    else {
        ui.ck_sd_2->setChecked(true);
    }

    connect(ui.ck_tr_1, &QCheckBox::stateChanged, [this](int nState) {
        if (nState == Qt::CheckState::Checked)
        {
            ui.ck_tr_2->blockSignals(true);
            ui.ck_tr_2->setChecked(false);
            ui.ck_tr_2->blockSignals(false);
            SETTING.setTransTT(1);
        }
        });
    connect(ui.ck_tr_2, &QCheckBox::stateChanged, [this](int nState) {
        if (nState == Qt::CheckState::Checked)
        {
            ui.ck_tr_1->blockSignals(true);
            ui.ck_tr_1->setChecked(false);
            ui.ck_tr_1->blockSignals(false);
            SETTING.setTransTT(2);
        }
        });

    connect(ui.ck_sd_1, &QCheckBox::stateChanged, [this](int nState) {
        if (nState == Qt::CheckState::Checked)
        {
            ui.ck_sd_2->blockSignals(true);
            ui.ck_sd_2->setChecked(false);
            ui.ck_sd_2->blockSignals(false);
            SETTING.setTransSD(1);
        }
        });
    connect(ui.ck_sd_2, &QCheckBox::stateChanged, [this](int nState) {
        if (nState == Qt::CheckState::Checked)
        {
            ui.ck_sd_1->blockSignals(true);
            ui.ck_sd_1->setChecked(false);
            ui.ck_sd_1->blockSignals(false);
            SETTING.setTransSD(2);
        }
        });
}

WTranslationPage::~WTranslationPage()
{}

void WTranslationPage::slot_bk_clicked()
{
    if (QObject::sender() == ui.ck_bk_1) {
        ui.ck_bk_1->setSel(true);
        ui.ck_bk_2->setSel(false);
        SETTING.setTransBk(1);
    }
    else {
        ui.ck_bk_1->setSel(false);
        ui.ck_bk_2->setSel(true);
        SETTING.setTransBk(2);
    }

    auto view = AiSound::GetInstance().GetTranslationMainView();
    view->update();
}

void WTranslationPage::slot_tl_clicked()
{
    WRadioBtn* btn = static_cast<WRadioBtn*>(QObject::sender());
    if (btn) {
        btn->setSel(true);
    }
    int index = 0;
    for (auto it : _tl_ck_list_) {
        index++;
        if (it != btn) {
            it->setSel(false);
        }
        else {
            SETTING.setTransTl(index);
        }
    }

    auto view = AiSound::GetInstance().GetTranslationMainView();
    view->update();
}

void WTranslationPage::slot_og_clicked()
{
    WRadioBtn* btn = static_cast<WRadioBtn*>(QObject::sender());
    if (btn) {
        btn->setSel(true);
    }
    int index = 0;
    for (auto it : _og_ck_list_) {
        index++;
        if (it != btn) {
            it->setSel(false);
        }
        else {
            SETTING.setTransOg(index);
        }
    }

    auto view = AiSound::GetInstance().GetTranslationMainView();
    view->update();
}