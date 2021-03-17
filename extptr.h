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
    ExtPtr();
    ExtPtr(const QString name); //инициализация по имени указателя в ини файле offsets
    ExtPtr(uintptr_t base, const QString name);//иницализация по указателю+оффсет из ини файла
    ~ExtPtr();

    const QString &name() const {return m_name;} //возврат имени указателя
    uintptr_t operator&() const {return m_ptr;}//возврат указателя
    T operator*() const{//чтение памяти по указателю
        T value;
        if (!ReadProcessMemory(hProc, (BYTE*)m_ptr, &value, sizeof(value), nullptr))//если не может прочитать
            value = NULL;
        return value;
    }
    ExtPtr& operator=(const ExtPtr &extptr){//присваивание
        m_name=extptr.m_name;
        m_ptr=extptr.m_ptr;
        return *this;
    }

private:
    QString m_name="";//имя указателя
    uintptr_t m_ptr=0;//указатель
};

template<class T>
ExtPtr<T>::ExtPtr()
{

}

template<class T>
ExtPtr<T>::ExtPtr(const QString name):m_name(name)//инициализация по имени указателя в ини файле offsets
{
    //m_name=new QString (name);
    QString offstr=offsets->value(m_name).toString();//читает строку оффсетов из ини файла
    QStringList offlist=offstr.split(" ");//разделяет строку оффсетов на части
    bool ok;
    m_ptr=modBase+offlist.at(0).toUInt(&ok,16);//адрес модуля + первый оффсет
    for(int i=1; i<offlist.size();i++) {//итерация по оффсетам
        if (!ReadProcessMemory(hProc, (BYTE*)m_ptr, &m_ptr, sizeof(m_ptr), 0)){//читает значение по указателю m_ptr в m_ptr
            m_ptr = 0;//если не может прочитать
            break; //выходит из цикла по оффсетам
        }
        m_ptr+=offlist.at(i).toUInt(&ok, 16);//прибавляет 16-тиричный оффсет к указателю
    }
}

template<class T>
ExtPtr<T>::ExtPtr(uintptr_t base, const QString name):m_name(name)//иницализация по указателю+оффсет из ини файла
{
    //m_name=new QString (name);
    QString offstr=offsets->value(m_name).toString();//читает строку оффсетов из ини файла
    bool ok;
    m_ptr=base+offstr.toUInt(&ok, 16);//базовый адрес+оффсет
}

template<>
QString ExtPtr<QString>::operator*() const; //чтение памяти по указателю для строки


template <typename T>
ExtPtr<T>::~ExtPtr()
{
    //if (m_name!=nullptr)
    //delete m_name;
}



#endif // EXTPTR_H
