#pragma once
#include "WCommonWidget.h"
#include "qimage.h"
#include <QLineEdit>

class WCommonEdit : public WCommonWidget
{
    Q_OBJECT

public:
    WCommonEdit(QWidget* parent = nullptr);
    ~WCommonEdit();
    
    void SetImage(const QString& path);

    QLineEdit* textEdit;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void focusChanged(QWidget* old, QWidget* now);

    QImage      _icon;
    bool        _selected{ false };
};


