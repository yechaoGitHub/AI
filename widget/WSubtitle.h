#pragma once
#include <QWidget>
#include <QScrollArea>
#include <vector>

#pragma region WSubtitle
class WSubtitle : public QWidget
{
    Q_OBJECT
public:
    WSubtitle(QWidget* parent = nullptr);
    ~WSubtitle();

    void SetTranslate(const QString& srcLanguage, const QString& destLanguage);
    void AddSubtitle(const QString& src, const QString& dest);
    void UpdateSubtitle(const QString& src, const QString& dest);

    QSize sizeHint() const override;

    //QSize PerferredSize();
Q_SIGNALS:
    void subtitleAdd();

protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);

private:
    struct TextLayout
    {
        QString text;
        QRect   rect;
    };

    struct TranslationText
    {
        TextLayout  src;
        TextLayout  dest;
        QRect       rect;
    };

    void CalculateTranslationRect();
    QSize CalculateTextSize(QFontMetrics& fm, const QString& text, int w);
    TranslationText NewTranslationText(const QString& src, const QString& dest, int startPos);

    QString                         _srcLanguage;
    QString                         _destLanguage;
    std::vector<TranslationText>    _subtitle;
};
#pragma endregion

#pragma region WSubtitleAutoScroll
class WSubtitleAutoScroll : public QWidget
{
    Q_OBJECT
public:
    WSubtitleAutoScroll(QWidget* parent = nullptr);
    ~WSubtitleAutoScroll();

    WSubtitle* Subtitle();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void timerEvent(QTimerEvent* event) override;

private:
    void SubtitleAdded();

    WSubtitle*      _subtitle;
    QScrollArea*    _scrollArea;
};
#pragma endregion