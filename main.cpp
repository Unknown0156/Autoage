#include <iostream>

#include <QApplication>
#include <QSettings>

#include "constants.h"
#include "functions.h"
#include "autoage.h"

HANDLE hProc=NULL; //ГЛОБАЛЬНЫЙ хэндлер процесса (ExtPtr)
uintptr_t modBase=0; //ГЛОБАЛЬНЫЙ адрес модуля (ExtPtr)
QSettings *offsets=nullptr; //ГЛОБАЛЬНЫЙ файл оффсетов (ExtPtr)

HWND hWnd = NULL; //ГЛОБАЛЬНЫЙ хэндлер окна (functions)

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSettings *settings;
    settings = new QSettings(QCoreApplication::applicationDirPath()+"/"+F_NAME_SETTINGS,QSettings::IniFormat); //открывает файл настроек
    QString qprocName=settings->value("process").toString();
    const wchar_t *procName=reinterpret_cast<const wchar_t *>(qprocName.utf16());//имя процесса
    QString qmodName=settings->value("module").toString();
    const wchar_t *modName=reinterpret_cast<const wchar_t *>(qmodName.utf16()); //имя модуля
    delete settings;
    DWORD procId= getProcId(procName); //ид процесса
    hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId); //открывает хэгдлер процесса
    modBase = getModuleBaseAddress(procId, modName); //ищет адрес модуля в процессе
    getWindowFromProcessID(procId, hWnd); //открывает хэндлер окна
    offsets=new QSettings(QCoreApplication::applicationDirPath()+"/"+F_NAME_OFFSETS,QSettings::IniFormat); //открывает файл оффсетов
    Autoage window;
    window.show();
    return app.exec();
}
