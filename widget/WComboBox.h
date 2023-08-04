#pragma once

#include "WDropDownList.h"

#include <QWidget>
#include <QImage>
#include <QListWidget>
#include <QString>

#include <vector>
#include <chrono>

class WComboBox : public QWidget
{
    Q_OBJECT
public:
    WComboBox(QWidget* parent = nullptr);
    ~WComboBox();

    void AddItem(const QString& text, void* data);
    int SelectItem();

protected:
    enum class Status { FOLD, EXPLAND };

    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void ItemClicked(int row, const QString& text);
    void DropDownHiden();

    int                                     _selectItem = -1;
    WDropDownList*                          _dropDownList;
    Status                                  _status;

    QImage                                  _downImg;
    QImage                                  _upImg;
    QString                                 _text;

    std::vector<void*>                      _datas;
};

