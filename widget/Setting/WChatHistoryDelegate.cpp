#include "WChatHistoryDelegate.h"
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>


WHistoryDelegate::WHistoryDelegate(QWidget*parent)
{
}

WHistoryDelegate::~WHistoryDelegate()
{}


void WHistoryDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    QStyledItemDelegate::paint(painter, viewOption, index);

    int nCount = 2;
    int w = option.rect.width() / nCount;
    if (w < 0) {
        return;
    }
    for (int i = 0; i < nCount; ++i)
    {
        QStyleOptionButton button;
        button.rect = option.rect.adjusted(4 + i * w, 4, -(w * (nCount - i - 1) + 4), -4);
        button.state |= QStyle::State_Enabled;

        button.state |= QStyle::State_Enabled;
        button.state |= QStyle::State_MouseOver;
        QPushButton pushBtn;
        if (i == 0) {
            pushBtn.setStyleSheet("QPushButton{border:none;background-position:center;background-repeat:no-repeat;background-image: url(:/QtTest/icon/Setting/chatbot/history.png);}");
        }
        else {
            pushBtn.setStyleSheet("QPushButton{border:none;background-position:center;background-repeat:no-repeat;background-image: url(:/QtTest/icon/Setting/del.png);}");
        }

        pushBtn.style()->drawControl(QStyle::CE_PushButton, &button, painter, &pushBtn);
    }
}


bool WHistoryDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    bool bRepaint = false;
    QMouseEvent* pEvent = static_cast<QMouseEvent*> (event);
    m_mousePoint = pEvent->pos();

    QApplication::restoreOverrideCursor();

    int nCount = 2;
    int w = option.rect.width() / nCount;
    if (w < 0) {
        return false;
    }

    for (int i = 0; i < nCount; ++i)
    {
        QStyleOptionButton button;
        button.rect = option.rect.adjusted(4 + i * w, 4, -(w * (nCount - i - 1) + 4), -4);
        // 鼠标位于按钮之上
        if (!button.rect.contains(m_mousePoint))
            continue;

        switch (event->type())
        {
            // 鼠标释放
        case QEvent::MouseButtonRelease:
        {
            emit sig_opeData(index,i);
        }
        default:
            break;
        }
    }

    return bRepaint;
}