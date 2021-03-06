#include "extptr.h"

template<>
const QString &ExtPtr<QString>::operator*()
{
    m_value.clear();
    uintptr_t tmpptr = m_ptr; //указатель для итерации по строке
    char valueCh=' '; //символ по указателю
    while (valueCh != 0) //чтение строки по-символьно из памяти
    {
        if (!ReadProcessMemory(hProc, (BYTE*)tmpptr, &valueCh, sizeof(valueCh), nullptr)) //чтение символа по указателю
        {
            m_value = "invalid_name"; //если не может прочитать
            break;
        }
        m_value.push_back(valueCh);
        tmpptr++;
    }
    return m_value;
}
