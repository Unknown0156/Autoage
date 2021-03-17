#include "extptr.h"

template<>
QString ExtPtr<QString>::operator*() const{
    QString value;
    uintptr_t tmpptr = m_ptr; //указатель для итерации по строке
    char valueCh=' '; //символ по указателю
    while (valueCh != 0) //чтение строки по-символьно из памяти
    {
        if (!ReadProcessMemory(hProc, (BYTE*)tmpptr, &valueCh, sizeof(valueCh), nullptr)) //чтение символа по указателю
        {
            value = "invalid_name"; //если не может прочитать
            break;
        }
        value.push_back(valueCh);
        tmpptr++;
    }
    return value;
}
