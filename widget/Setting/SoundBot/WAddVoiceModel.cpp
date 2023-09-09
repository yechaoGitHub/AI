#include "WAddVoiceModel.h"
#include <QDesktopServices>
#include <qurl.h>


WAddVoiceModel::WAddVoiceModel(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

WAddVoiceModel::~WAddVoiceModel()
{}

void WAddVoiceModel::on_pb_add_clicked()
{
	QDesktopServices::openUrl(QUrl("https://aisounda.cn/#/custom"));
}

void WAddVoiceModel::setModelText(const QString& text)
{
	//ui.label_page->setText(text);
}