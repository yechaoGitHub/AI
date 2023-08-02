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

Q_SIGNALS:
    void hideLatter();

protected:
    enum class Status { FOLD, EXPLAND };

    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void ItemClicked(int row, const QString& text);
    void DropDownHiden();

    WDropDownList*                          _dropDownList;
    Status                                  _status;

    QImage                                  _downImg;
    QImage                                  _upImg;
    QString                                 _text;

    std::vector<void*>                      _datas;
};

