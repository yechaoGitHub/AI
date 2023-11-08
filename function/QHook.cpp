#include "QHook.h"
#include <QDebug>
#include <windows.h>


static HHOOK keyHook = nullptr;
static Hook* hook;
#define KEY_DOWN(vKey) ((GetAsyncKeyState(vKey) & 0x8000) ? 1:0)
LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*)lParam;
    if (wParam == WM_KEYDOWN) {
        if (pkbhs->vkCode == 0x47 && KEY_DOWN(VK_CONTROL) /*&& KEY_DOWN(VK_MENU)*/) {
            hook->sendSignal(1);
        }
        if (pkbhs->vkCode == 0x54 && KEY_DOWN(VK_CONTROL) && KEY_DOWN(VK_MENU)) {
            hook->sendSignal(2);
        }
    }
    return CallNextHookEx(keyHook, nCode, wParam, lParam);
}

Hook::Hook()
{

}

void Hook::installHook() {
    keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, nullptr, 0);
    hook = this;
}
void Hook::unInstallHook() {
    UnhookWindowsHookEx(keyHook);
    keyHook = nullptr;
}

void Hook::sendSignal(int type) {
    emit sig_sendKeyType(type);
}