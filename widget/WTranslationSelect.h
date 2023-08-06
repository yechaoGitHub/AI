#pragma once
#include <QWidget>
#include "AiSound.h"
#include "ui_translateSelect.h"

class WTranslationSelect : public QWidget
{
    Q_OBJECT
public:
    WTranslationSelect(QWidget* parent = nullptr);
    ~WTranslationSelect();

    void SetFunctionType(FunctionType type);
    void SetSrcLanguage(const std::vector<TranslationLanguage>& vecSrc);
    void SetDestLanguage(const std::vector<TranslationLanguage>& vecDest);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    void StartClicked();
    Ui::Translation                     ui;

    FunctionType                        _type;
    std::vector<TranslationLanguage>    _vecSrc;
    std::vector<TranslationLanguage>    _vecDest;
    TranslationLanguage                 _selSrvLan;
    TranslationLanguage                 _selDestLan;
    QPoint                              _clickPos;
    bool                                _mouseHold;
};

