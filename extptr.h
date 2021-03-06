#ifndef EXTPTR_H
#define EXTPTR_H

#include <Windows.h>

#include <QString>
#include <QSettings>

//Класс внешнего указателя

extern HANDLE hProc;//ГЛОБАЛЬНЫЙ хэндлер процесса
extern uintptr_t modBase;//ГЛОБАЛЬНЫЙ адрес модуля
extern QSettings *offsets;//ГЛОБАЛЬНЫЙ ини файл оффсетов

template <class T>//тип данных по указателю
class ExtPtr
{
public:
    ExtPtr(const QString name); //имя указателя в ини файле offsets
    ~ExtPtr();

    uintptr_t getPtr() const {return this->m_ptr;} //возврат указателя
    QString getName() const {return this->m_name;} //возврат имени указателя

    const T &operator*()//чтение памяти по указателю
    {
        if (!ReadProcessMemory(hProc, (BYTE*)m_ptr, &m_value, sizeof(m_value), nullptr))
            m_value = NULL;
        return m_value;
    }

private:
    uintptr_t m_ptr=0; //указатель
    QString m_name=""; //имя указателя
    T m_value; //значение по указателю

};

template <class T>
ExtPtr<T>::ExtPtr(const QString name): m_name(name)
{
    m_ptr=modBase;
    QString offstr=offsets->value(name).toString();//читает строку оффсетов из ини файла
    QStringList offlist=offstr.split(" ",QString::SkipEmptyParts);//разделяет строку оффсетов на части
    foreach (QString offnum, offlist) {//итерация по оффсетам
        bool ok;
        m_ptr+=offnum.toUInt(&ok, 16);//прибавляет 16-тиричный оффсет к указателю
        if (offnum==offlist.last())//если оффсет последний - выходит
            break;
        if (!ReadProcessMemory(hProc, (BYTE*)m_ptr, &m_ptr, sizeof(m_ptr), 0)){//читает значение по указателю m_ptr в m_ptr
            m_ptr = 0;//если не может прочитать
            break; //выходит из цикла по оффсетам
        }
    }
    if (m_ptr)
        m_value = **this;//сохраняем значение в m_value
}

template<>
const QString &ExtPtr<QString>::operator*(); //шаблон для строки


template <typename T>
ExtPtr<T>::~ExtPtr()
{
}

#endif // EXTPTR_H
