#include "functions.h"

extern HWND g_hWnd;//хэндлер окна

DWORD getProcId (const wchar_t* procName) //определение ид процесса по имени исполняемого файла
{
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //делает снимок всех процессов в системе
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry; //структура процесса из снимка
        procEntry.dwSize = sizeof(procEntry); //устанавливает размер

        if (Process32First(hSnap, &procEntry)) //первый процесс в снимке
        {
            do
            {
                if (!_wcsicmp(procEntry.szExeFile, procName)) //имя ехе файла процесса совпадает с искомым
                {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &procEntry)); //следующий процесс в снимке

        }
    }
    CloseHandle(hSnap); //закрывает снимок
    return procId; //возвращает ид процесса, либо 0 если процесс не найден
}

uintptr_t getModuleBaseAddress(DWORD procId, const wchar_t* modName) //определение базового адреса модуля в процессе
{
    uintptr_t modBase = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);//делает снимок всех модулей в процессе
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;//структура модуля из снимка
        modEntry.dwSize = sizeof(modEntry);//устанавливает размер
        if (Module32First(hSnap, &modEntry))//первый модуль в снимке
        {
            do
            {
                if (!_wcsicmp(modEntry.szModule, modName))//имя модуля совпадает с искомым
                {
                    modBase = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));//следующий модуль в снимке
        }
    }
    CloseHandle(hSnap);//закрывает снимок
    return modBase;//возвращает адресс модуля, либо 0 если модуль не найден
}

HWND getWindowHandle(DWORD procId, const QString &title) //определние хэндлера окна по ид процесса
{
    HWND hWnd=0;//нужный хэндлер
    HWND curHWnd = NULL;//текущий хэндлер окна
    do
    {
        curHWnd = FindWindowEx(NULL, curHWnd, NULL, NULL);//ищет следующее окно
        DWORD curProcId = 0;//текущий ид процесса
        GetWindowThreadProcessId(curHWnd, &curProcId);//получает ид процесса окна
        if (curProcId == procId)
        {
            char curTitle[256];
            GetWindowTextA(curHWnd, curTitle,sizeof(curTitle));//получает заголовок окна
            QString qcurTitle = QString::fromStdString(curTitle);
            if (qcurTitle.contains(title)){//если заголов сожержит нужный текст
                hWnd=curHWnd;
                break;
            }
        }
    }
    while (curHWnd != NULL);
    return hWnd;
}

bool keyDown(char keyS) //нажатие кнопки
{
    switch (keyS)
    {
    case 'w': return PostMessage(g_hWnd, WM_KEYDOWN, 0x57, MAKELPARAM(0x1, 0x11));
    case 'a': return PostMessage(g_hWnd, WM_KEYDOWN, 0x41, MAKELPARAM(0x1, 0x1E));
    case 's': return PostMessage(g_hWnd, WM_KEYDOWN, 0x53, MAKELPARAM(0x1, 0x1F));
    case 'd': return PostMessage(g_hWnd, WM_KEYDOWN, 0x44, MAKELPARAM(0x1, 0x20));
    case 'f': return PostMessage(g_hWnd, WM_KEYDOWN, 0x46, MAKELPARAM(0x1, 0x21));
    case '1': return PostMessage(g_hWnd, WM_KEYDOWN, 0x31, MAKELPARAM(0x1, 0x2));
    case '2': return PostMessage(g_hWnd, WM_KEYDOWN, 0x32, MAKELPARAM(0x1, 0x3));
    case '3': return PostMessage(g_hWnd, WM_KEYDOWN, 0x33, MAKELPARAM(0x1, 0x4));
    case '4': return PostMessage(g_hWnd, WM_KEYDOWN, 0x34, MAKELPARAM(0x1, 0x5));
    case '5': return PostMessage(g_hWnd, WM_KEYDOWN, 0x35, MAKELPARAM(0x1, 0x6));
    case '6': return PostMessage(g_hWnd, WM_KEYDOWN, 0x36, MAKELPARAM(0x1, 0x7));
    case '7': return PostMessage(g_hWnd, WM_KEYDOWN, 0x37, MAKELPARAM(0x1, 0x8));
    case '8': return PostMessage(g_hWnd, WM_KEYDOWN, 0x38, MAKELPARAM(0x1, 0x9));
    case '9': return PostMessage(g_hWnd, WM_KEYDOWN, 0x39, MAKELPARAM(0x1, 0xA));
    case '\t': return PostMessage(g_hWnd, WM_KEYDOWN, 0x9, MAKELPARAM(0x1, 0xF));
    case ' ': return PostMessage(g_hWnd, WM_KEYDOWN, 0x20, MAKELPARAM(0x1, 0x39));
    default: return 0;//нет такой кнопки
    }
}

bool keyUp(char keyS) //отжатие кнопки
{
    switch (keyS)
    {
    case 'w': return PostMessage(g_hWnd, WM_KEYUP, 0x57, MAKELPARAM(0x1, 0xC011));
    case 'a': return PostMessage(g_hWnd, WM_KEYUP, 0x41, MAKELPARAM(0x1, 0xC01E));
    case 's': return PostMessage(g_hWnd, WM_KEYUP, 0x53, MAKELPARAM(0x1, 0xC01F));
    case 'd': return PostMessage(g_hWnd, WM_KEYUP, 0x44, MAKELPARAM(0x1, 0xC020));
    case 'f': return PostMessage(g_hWnd, WM_KEYUP, 0x46, MAKELPARAM(0x1, 0xC021));
    case '1': return PostMessage(g_hWnd, WM_KEYUP, 0x31, MAKELPARAM(0x1, 0xC002));
    case '2': return PostMessage(g_hWnd, WM_KEYUP, 0x32, MAKELPARAM(0x1, 0xC003));
    case '3': return PostMessage(g_hWnd, WM_KEYUP, 0x33, MAKELPARAM(0x1, 0xC004));
    case '4': return PostMessage(g_hWnd, WM_KEYUP, 0x34, MAKELPARAM(0x1, 0xC005));
    case '5': return PostMessage(g_hWnd, WM_KEYUP, 0x35, MAKELPARAM(0x1, 0xC006));
    case '6': return PostMessage(g_hWnd, WM_KEYUP, 0x36, MAKELPARAM(0x1, 0xC007));
    case '7': return PostMessage(g_hWnd, WM_KEYUP, 0x37, MAKELPARAM(0x1, 0xC008));
    case '8': return PostMessage(g_hWnd, WM_KEYUP, 0x38, MAKELPARAM(0x1, 0xC009));
    case '9': return PostMessage(g_hWnd, WM_KEYUP, 0x39, MAKELPARAM(0x1, 0xC00A));
    case '\t': return PostMessage(g_hWnd, WM_KEYUP, 0x9, MAKELPARAM(0x1, 0xC00F));
    case ' ':  return PostMessage(g_hWnd, WM_KEYUP, 0x20, MAKELPARAM(0x1, 0xC039));
    default: return 0;//нет такой кнопки
    }
}

bool keyClick(char keyS)//клик на кнопку
{
    if(keyDown(keyS)){
        wait(getRandomNumber(50,70));
        keyUp(keyS);
        return true;
    }
    return false;
}


void wait(int t)//ожидание
{
    for(;t>=0;t-=20){
        Sleep(20);
        QCoreApplication::processEvents();
    }
}

int getRandomNumber(int min, int max) //генератор рандомных чисел (от, до)
{
    std::random_device rd; //случайное системное число (зависит от ОС)
    std::mt19937 mersenne(rd()); // инициализирует Вихрь Мерсенна случайным системным числом
    std::uniform_int_distribution <int> dist(min, max); //задаем интервал целых чисел
    return dist(mersenne); //генерирует следующее целое число в интервале с сидом из вихря
}



