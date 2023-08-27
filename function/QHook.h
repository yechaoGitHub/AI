#ifndef HOOK_H
#define HOOK_H

#include <QObject>

class Hook :public QObject
{
    Q_OBJECT
public:
    Hook();
    void installHook();
    void unInstallHook();
    void sendSignal(int type);

signals:
    void sig_sendKeyType(int type);
};

#endif // HOOK_H