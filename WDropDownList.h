#pragma once
#include <QWidget>
#include <QListWidget>
#include <QScrollBar>

class WDropDownList : public QWidget
{
    Q_OBJECT

public:
    WDropDownList(QWidget* parent = nullptr);
    ~WDropDownList();

    int RowHeight();

    int AddItem(const QString& label);

    QListWidget* listWidget;

Q_SIGNALS:
    void itemClicked(int row, const QString& text);
    void windowHiden();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;

private:
    void ItemClicked(QListWidgetItem* item);
};

class WDropDownListItemWidget : public QWidget
{
    Q_OBJECT
public:
    WDropDownListItemWidget(QWidget* parent = nullptr);
    ~WDropDownListItemWidget();

    QString     text;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    bool        _mouseIn;
};

