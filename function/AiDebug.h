#pragma once
#include <QObject>
#include <QWidget>
#include <QPlainTextEdit>
#include <QScrollArea>

class WDebugWindow : public QWidget
{
public:
    WDebugWindow();
    ~WDebugWindow();

    void AddText(const QString& text);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QScrollArea* _scrollArea = nullptr;
    QString _debugText;
    QPlainTextEdit* _label = nullptr;
};

class AiDebug : public QObject
{
    Q_OBJECT
public:
    AiDebug();
    ~AiDebug();

    static void Output(const QString& text);
    static void ShowDebugWindow(bool show);

private:
signals:
    void addText(const QString& text);

private:
#ifdef _DEBUG
    static AiDebug INSTANCE;
    WDebugWindow* _debugWindow = nullptr;
#endif

};


