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

    int nCount = 1;
    int w = option.rect.width() / nCount;
    w = 20;
    if (w < 0) {
        return;
    }
    for (int i = 0; i < nCount; ++i)
    {
        QStyleOptionButton button;
        /*auto it = option.rect;
        button.rect = option.rect.adjusted(4 + i * w, 4, -(w * (nCount - i - 1) + 4), -4);
        auto ss = button.rect;*/

        if (i == 0) {
            button.rect = option.rect.adjusted(2 + i * w, 4, -60, -4);
        }
        else {
            button.rect = option.rect.adjusted(4 + i * w, 4, -(w * (nCount - i - 1) + 4), -4);
           /* auto it = option.rect;
            button.rect = option.rect.adjusted(2 + i * w, 4, -60, -4);
            auto ss = button.rect;
            int i = 1;*/
        }

        button.state |= QStyle::State_Enabled;

        button.state |= QStyle::State_Enabled;
        button.state |= QStyle::State_MouseOver;
        QPushButton pushBtn;
        if (nCount == 2) {
            if (i == 0) {
                pushBtn.setStyleSheet("QPushButton{border:none;background-position:center;background-repeat:no-repeat;background-image: url(:/QtTest/icon/Setting/chatbot/history.png);}");
            }
            else {
                pushBtn.setStyleSheet("QPushButton{border:none;background-position:center;background-repeat:no-repeat;background-image: url(:/QtTest/icon/Setting/del.png);}");
            }
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

    int nCount = 1;
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