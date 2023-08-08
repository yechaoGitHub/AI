#include "WSubtitle.h"
#include "WScrollBar.h"

#include <QPainter>
#include <QFontMetrics>
#include <QResizeEvent>
#include <QTimerEvent>
#include <cmath>


#pragma region WSubtitle
WSubtitle::WSubtitle(QWidget* parent) :
    QWidget{ parent }
{
}

WSubtitle::~WSubtitle()
{
}

void WSubtitle::SetTranslate(const QString& srcLanguage, const QString& destLanguage)
{
    _srcLanguage = srcLanguage;
    _destLanguage = destLanguage;
}

void WSubtitle::AddSubtitle(const QString& src, const QString& dest)
{
    auto w = this->rect().width();
    auto f = this->font();
    QFontMetrics fm{ f };

    if (_subtitle.empty())
    {
        _subtitle.push_back(NewTranslationText(src, dest, 0));
    }
    else
    {
        auto& b = _subtitle.back();
        _subtitle.push_back(NewTranslationText(src, dest, b.rect.bottom()));
    }

    emit subtitleAdd();
}

void WSubtitle::UpdateSubtitle(const QString& src, const QString& dest)
{
    auto& b = _subtitle.back();
    b = NewTranslationText(src, dest, b.rect.y());
    repaint();
    emit subtitleAdd();
}

QSize WSubtitle::sizeHint() const
{
    if (_subtitle.empty())
    {
        return QSize{ 1078, 176 };
    }
    else
    {
        auto& b = _subtitle.back();
        if (b.rect.bottom() > 176)
        {
            return QSize{ 1078, b.rect.bottom() };
        }
        else
        {
            return QSize{ 1078, 176 };
        }
    }
}

void WSubtitle::paintEvent(QPaintEvent* event)
{
    QPainter painter{ this };
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    QTextOption toption(Qt::AlignLeft | Qt::AlignTop);
    toption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    for (auto& subtitle : _subtitle)
    {
        subtitle.rect.x();
        subtitle.rect.right();
        painter.setPen(QColor{ 255, 255, 255, 127 });
        painter.drawLine(QLine{ subtitle.rect.topLeft(), subtitle.rect.topRight() });

        painter.setPen(QColor{ 255, 255, 255, 255 });
        auto text = _srcLanguage + ":" + subtitle.src.text;
        painter.drawText(subtitle.src.rect, text, toption);

        painter.setPen(QColor{ 0, 215, 244, 255 });
        text = _destLanguage + ":" + subtitle.dest.text;
        painter.drawText(subtitle.dest.rect, text, toption);
    }
}

void WSubtitle::resizeEvent(QResizeEvent* event)
{
    if (event->size().width() != event->size().width())
    {
        CalculateTranslationRect();
    }
}

void WSubtitle::CalculateTranslationRect()
{
    auto w = this->rect().width();
    auto f = this->font();
    QFontMetrics fm{ f };

    int gap = 16;
    int curPos{};
    int marginX = 0;
    for (auto& subtitle : _subtitle)
    {
        subtitle.rect.setX(marginX);
        subtitle.rect.setY(curPos);

        auto text = _srcLanguage + ":" + subtitle.src.text;
        curPos += gap;
        auto textSize = CalculateTextSize(fm, text, w);
        subtitle.src.rect.setX(marginX);
        subtitle.src.rect.setY(curPos);
        subtitle.src.rect.setSize(textSize);

        text = _destLanguage + ":" + subtitle.dest.text;
        curPos += gap + textSize.height();
        textSize = CalculateTextSize(fm, text, w);
        subtitle.dest.rect.setX(marginX);
        subtitle.dest.rect.setY(curPos);
        subtitle.dest.rect.setSize(textSize);

        curPos += gap + textSize.height();

        subtitle.rect.setWidth(w);
        subtitle.rect.setHeight(curPos - subtitle.rect.y());
    }
}

QSize WSubtitle::CalculateTextSize(QFontMetrics& fm, const QString& text, int w)
{
    auto textW = fm.width(text);
    auto textH = fm.lineSpacing();

    if (textW < w)
    {
        return { textW, textH };
    }
    else
    {
        int l = std::ceil((double)textW / (double)w);

        return { w, l * textH };
    }
}

WSubtitle::TranslationText WSubtitle::NewTranslationText(const QString& src, const QString& dest, int startPos)
{
    TranslationText translationText;

    auto w = this->rect().width();
    auto f = this->font();
    QFontMetrics fm{ f };

    int gap = 16;
    int marginX = 0;

    translationText.rect.setX(marginX);
    translationText.rect.setY(startPos);

    auto text = _srcLanguage + ":" + src;
    startPos += gap;
    auto textSize = CalculateTextSize(fm, text, w);
    translationText.src.rect.setX(marginX);
    translationText.src.rect.setY(startPos);
    translationText.src.rect.setSize(textSize);
    startPos += textSize.height();

    text = _destLanguage + ":" + dest;
    startPos += gap;
    textSize = CalculateTextSize(fm, text, w);
    translationText.dest.rect.setX(marginX);
    translationText.dest.rect.setY(startPos);
    translationText.dest.rect.setSize(textSize);
    startPos += textSize.height();

    startPos += gap;

    translationText.rect.setWidth(w);
    translationText.rect.setHeight(startPos - translationText.rect.y());

    translationText.src.text = src;
    translationText.dest.text = dest;

    return translationText;
}
#pragma endregion

#pragma region WSubtitleAutoScroll
WSubtitleAutoScroll::WSubtitleAutoScroll(QWidget* parent) :
    QWidget{ parent }
{
    _scrollArea = new QScrollArea{ this };
    _scrollArea->setFrameStyle(QFrame::NoFrame);
    _scrollArea->setStyleSheet("background:transparent");
    _scrollArea->setVerticalScrollBar(new WScrollBar{ });
    _scrollArea->setHorizontalScrollBar(new WScrollBar{ });
    _scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    _subtitle = new WSubtitle();
    _scrollArea->setWidget(_subtitle);

    _scrollArea->move(0, 0);
    _scrollArea->resize(this->size());

    connect(_subtitle, &WSubtitle::subtitleAdd, this, &WSubtitleAutoScroll::SubtitleAdded);
}

WSubtitleAutoScroll::~WSubtitleAutoScroll()
{

}

WSubtitle* WSubtitleAutoScroll::Subtitle()
{
    return _subtitle;
}

void WSubtitleAutoScroll::resizeEvent(QResizeEvent* event)
{
    _scrollArea->resize(this->size());
}

void WSubtitleAutoScroll::timerEvent(QTimerEvent* event)
{
    auto p = _scrollArea->verticalScrollBar()->sliderPosition();
    auto max = _scrollArea->verticalScrollBar()->maximum();

    if (p != max)
    {
        _scrollArea->verticalScrollBar()->setSliderPosition(p + 1);
    }
    else
    {
        killTimer(event->timerId());
    }
}

void WSubtitleAutoScroll::SubtitleAdded()
{
    _subtitle->resize(_subtitle->sizeHint());
    startTimer(16, Qt::PreciseTimer);
}

#pragma endregion