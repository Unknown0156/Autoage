#include <iostream>

#include <QApplication>
#include <QSettings>

#include "constants.h"
#include "functions.h"
#include "autoage.h"

HANDLE g_hProc=NULL; //ГЛОБАЛЬНЫЙ хэндлер процесса (ExtPtr)
uintptr_t g_modBase=0; //ГЛОБАЛЬНЫЙ адрес модуля (ExtPtr)
HWND g_hWnd = NULL; //ГЛОБАЛЬНЫЙ хэндлер окна (functions)
QSettings *g_offsets=nullptr; //ГЛОБАЛЬНЫЙ файл оффсетов (ExtPtr)

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSettings *settings;
    settings = new QSettings(":/settings/settings.ini",QSettings::IniFormat); //открывает файл настроек
    QString qprocName=settings->value("process").toString();
    const wchar_t *procName=reinterpret_cast<const wchar_t *>(qprocName.utf16());//имя процесса
    QString qmodName=settings->value("module").toString();
    const wchar_t *modName=reinterpret_cast<const wchar_t *>(qmodName.utf16()); //имя модуля
    delete settings;
    DWORD procId= getProcId(procName); //ид процесса
    g_hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId); //открывает хэндлер процесса
    g_modBase = getModuleBaseAddress(procId, modName); //ищет адрес модуля в процессе
    getWindowFromProcessID(procId, g_hWnd); //открывает хэндлер окна
    g_offsets=new QSettings(":/settings/offsets.ini",QSettings::IniFormat); //открывает файл оффсетов
    Autoage window;
    window.show();
    return app.exec();
}
