#include <QtWidgets/QApplication>
#include "AiSound.h"
#include "login/WLoginUI.h"
#include "WRobotNavigation.h"
#include "base/GlobalSetting.h"
#include "AiDebug.h"
#include <QSharedMemory>

#include <Windows.h>
#include <DbgHelp.h>
#include <winnt.h>
#include <time.h>
#include <locale>
#include <codecvt>
#pragma comment(lib,"Dbghelp.lib")

LONG __stdcall ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
{
    char str_time[100];
    memset(str_time, 0, 100);
    struct tm* local_time = NULL;
    time_t utc_time;
    utc_time = time(NULL);
    local_time = localtime(&utc_time);
    strftime(str_time, 100, "%Y%m%d%H%M%S", local_time);

    std::string timeStr(str_time);
    std::string dumpFilePath = timeStr + ".dmp";
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(dumpFilePath);
    HANDLE hDumpFile = CreateFile(wide.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hDumpFile != INVALID_HANDLE_VALUE) {
        //Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //写入Dump文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
    }
    //这里弹出一个错误对话框并退出程序
    EXCEPTION_RECORD* record = pException->ExceptionRecord;
    fprintf(stderr, "record->ExceptionCode:%lu\n", record->ExceptionCode);
    return EXCEPTION_EXECUTE_HANDLER;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":QtTest/icon/icon.ico"));

    QSharedMemory shared("AiSound");
    if (shared.attach())
    {
        exit(0);
    }
    shared.create(1);

    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    if (!SETTING.init(QString(SETTING.getRootLocalPath() + "/system.ini"))) {
        return -1;
    }
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);

    AiDebug::ShowDebugWindow(true);

    AiSound::GetInstance().Initialize();
    AiSound::GetInstance().ShowLoginFrame();

    return a.exec();
    //return r;
}
