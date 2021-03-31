#include <iostream>

#include <QApplication>
#include <QSettings>

#include "functions.h"
#include "autoage.h"

HANDLE g_hProc=NULL; //хэндлер процесса (ExtPtr)
uintptr_t g_modBase=0; //адрес модуля (ExtPtr)
QSettings *g_offsets=nullptr; //файл оффсетов (ExtPtr)
HWND g_hWnd = NULL; //хэндлер окна (functions)

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSettings *settings=new QSettings(":/settings/settings.ini",QSettings::IniFormat); //открывает файл настроек
    const QString qprocName=settings->value("process").toString();
    const wchar_t *procName=reinterpret_cast<const wchar_t *>(qprocName.utf16());//имя процесса
    const QString qmodName=settings->value("module").toString();
    const wchar_t *modName=reinterpret_cast<const wchar_t *>(qmodName.utf16()); //имя модуля
    const QString title=settings->value("title").toString();//заголовок окна
    delete settings;

    DWORD procId= getProcId(procName); //ид процесса
    g_hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId); //открывает хэндлер процесса
    g_modBase = getModuleBaseAddress(procId, modName); //ищет адрес модуля в процессе
    g_offsets=new QSettings(":/settings/offsets.ini",QSettings::IniFormat); //открывает файл оффсетов
    g_hWnd=getWindowHandle(procId, title); //открывает хэндлер окна
    Autoage window;
    window.show();
    return app.exec();
}
