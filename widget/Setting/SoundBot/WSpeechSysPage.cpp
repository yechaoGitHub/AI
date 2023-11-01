#include "WSpeechSysPage.h"
#include "AiSound.h"
#include "base/GlobalSetting.h"
#include ".\Speech\WSpeechGenerationUi.h"

WSpeechSysPage::WSpeechSysPage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    if (SETTING.getSpeechBk() == 1) {
        ui.pb_bk_1->setSel(true);
    }
    else {
        ui.pb_bk_2->setSel(true);
    }

    ui.pb_bk_2->setColor(QColor("#C9C9C9"));
    ui.pb_og_2->setColor(QColor("#00D7F4"));
    ui.pb_og_3->setColor(QColor("#00A9FF"));
    ui.pb_og_4->setColor(QColor("#6BFBCE"));

    _orig_btn_list.push_back(ui.pb_og_1);
    _orig_btn_list.push_back(ui.pb_og_2);
    _orig_btn_list.push_back(ui.pb_og_3);
    _orig_btn_list.push_back(ui.pb_og_4);

    int value = SETTING.getSpeechOg();
    for (int i = 0; i < _orig_btn_list.size(); i++) {
        int index = i + 1;
        _orig_btn_list.at(i)->setSel(value==index);
    }

    /*connect(ui.pb_bk_1, &QCheckBox::stateChanged, [this](int nState) {
        if (nState == Qt::CheckState::Checked)
        {
            ui.pb_bk_2->blockSignals(true);
            ui.pb_bk_2->setChecked(false);
            ui.pb_bk_2->blockSignals(false);
        }
        });
    connect(ui.pb_bk_2, &QCheckBox::stateChanged, [this](int nState) {
        if (nState == Qt::CheckState::Checked)
        {
            ui.pb_bk_1->blockSignals(true);
            ui.pb_bk_1->setChecked(false);
            ui.pb_bk_1->blockSignals(false);
        }
        });

    connect(ui.pb_og_2, &QCheckBox::stateChanged, [this](int nState) {
        if (nState == Qt::CheckState::Checked)
        {
            ui.pb_og_1->blockSignals(true);
            ui.pb_og_1->setChecked(false);
            ui.pb_og_1->blockSignals(false);
        }
        });*/
}

WSpeechSysPage::~WSpeechSysPage()
{}


void WSpeechSysPage::slot_color_change()
{
    WRadioBtn* btn = static_cast<WRadioBtn*>(QObject::sender());
    if (btn) {
        btn->setSel(true);
        int index = 0;
        for (auto it: _orig_btn_list) {
            index++;
            if (it != btn) {
                it->setSel(false);
            }
            else {
                SETTING.setSpeechOg(index);
            }
        }
    }
}

void WSpeechSysPage::slot_bk_change()
{
    if (QObject::sender() == ui.pb_bk_1) {
        ui.pb_bk_1->setSel(true);
        ui.pb_bk_2->setSel(false);
        SETTING.setSpeechBk(1);
    }
    else {
        ui.pb_bk_1->setSel(false);
        ui.pb_bk_2->setSel(true);
        SETTING.setSpeechBk(2);
    }

    auto view = AiSound::GetInstance().GetSpeechGenerationView();
    view->update();
}

void WSpeechSysPage::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        ui.retranslateUi(this);
    }

    QWidget::changeEvent(event);
}