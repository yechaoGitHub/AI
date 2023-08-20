#include "WTeamDelegate.h"
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>


WTeamDelegate::WTeamDelegate(QWidget*parent)
{
}

WTeamDelegate::~WTeamDelegate()
{}

void WTeamDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    QStyledItemDelegate::paint(painter, viewOption, index);

    QStyleOptionButton button;
    button.rect = option.rect;
    button.state |= QStyle::State_Enabled;
    button.state |= QStyle::State_MouseOver;

    QPushButton pushBtn;
    pushBtn.setStyleSheet("QPushButton{border:none;background-position:center;background-repeat:no-repeat;background-image: url(:/QtTest/icon/Setting/del.png);}");

    pushBtn.style()->drawControl(QStyle::CE_PushButton, &button, painter, &pushBtn);
}


bool WTeamDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    bool bRepaint = false;
    QMouseEvent* pEvent = static_cast<QMouseEvent*> (event);

    QApplication::restoreOverrideCursor();

    switch (event->type())
    {
    case QEvent::MouseButtonRelease:
    {
        emit sig_deleteData(index);
    }
    default:
        break;
    }

    return bRepaint;
}