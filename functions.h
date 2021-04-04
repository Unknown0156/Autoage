#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <random>

#include <Windows.h>
#include <TlHelp32.h>

#include <QString>
#include <QCoreApplication>

DWORD getProcId(const wchar_t* procName); //определение ид процесса по имени исполняемого файла
uintptr_t getModuleBaseAddress(DWORD procId, const wchar_t* modName); //определение базового адреса модуля в процессе
HWND getWindowHandle(DWORD procId, const QString &title);//определние хэндлера окна
bool keyDown(char keyS); //нажатие кнопки
bool keyUp(char keyS); //отжатие кнопки
int getRandomNumber(int min = INT16_MIN, int max = INT16_MAX); //генератор рандомных чисел (от, до)

#endif // FUNCTIONS_H
