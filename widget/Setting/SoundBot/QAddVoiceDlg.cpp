#include "QAddVoiceDlg.h"
#include "function/Bussiness/SettingInterfaceBussiness.h"
#include <QPainter>
#include <qmath.h>


QAddVoiceDlg::QAddVoiceDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    this->setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint | Qt::SubWindow);
}

QAddVoiceDlg::~QAddVoiceDlg()
{}

void QAddVoiceDlg::Show(int voiceId, const QString& name)
{
    _voiceId = voiceId;
    ui.lineEdit->setText(name);

    exec();
}

void QAddVoiceDlg::on_btn_close_clicked()
{
    this->close();
}

void QAddVoiceDlg::on_pb_cancel_clicked()
{
    this->close();
}

void QAddVoiceDlg::on_pb_sure_clicked()
{
    QString voiceLibName = ui.lineEdit->text();
    if (voiceLibName.isEmpty()) {
        return;
    }
    SettingInterfaceBussiness::getInstance()->addMyVoice(_voiceId, voiceLibName);
    this->close();
}

void QAddVoiceDlg::on_toolButton_clicked()
{
    SettingInterfaceBussiness::getInstance()->getVoiceUrlReq(_voiceId, false);
}

void QAddVoiceDlg::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    if (isActiveWindow()) {
        p.save();
        p.setRenderHint(QPainter::Antialiasing, true);
        QColor color(0, 0, 0, 10);
        for (int i = 0; i < 2; i++)
        {
            //color.setAlpha(60 - qSqrt(i) * 20);
            p.setPen(color);
            p.drawRoundedRect(rect().adjusted(2 - i, 2 - i, i - 2, i - 2), 8, 8);
        }
        p.restore();
    }

    p.setRenderHint(QPainter::Antialiasing, true);
    p.setPen(QColor("#ffffff"));
    p.setBrush(QColor("#ffffff"));
    p.drawRoundedRect(8, 8, width() - 10, height() - 10, 8, 8);
}