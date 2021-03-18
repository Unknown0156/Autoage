#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <random>

#include <Windows.h>
#include <TlHelp32.h>

#include <QString>
#include <QCoreApplication>

#include "constants.h"

DWORD getProcId(const wchar_t* procName); //определение ид процесса по имени исполняемого файла
uintptr_t getModuleBaseAddress(DWORD procId, const wchar_t* modName); //определение базового адреса модуля в процессе
void getWindowFromProcessID(DWORD ProcessID,HWND &hWnd);//определние хэндлера окна по ид процессу
bool keyDown(char keyS); //нажатие кнопки
bool keyUp(char keyS); //отжатие кнопки
void wait(int t=0);//ожидание
int getRandomNumber(int min = INT16_MIN, int max = INT16_MAX); //генератор рандомных чисел (от, до)

#endif // FUNCTIONS_H
