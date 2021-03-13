#ifndef MOB_H
#define MOB_H

#include <QString>

#include "extptr.h"

class Mob
{
public:
    Mob();
    Mob(const QString name, int num=NULL);//имя в файле оффсетов и номер если нужно

    QString name() const {return m_base.getName();}
    int addr() const {return &m_base;}
    int base() const {return *m_base;}
    QString type() const {return *m_type;}
    int x() const {return *m_x;}
    int y() const {return *m_y;}
    int z() const {return *m_z;}

    void refresh();

private:

    ExtPtr<int> m_base;
    ExtPtr<QString> m_type;
    ExtPtr<float> m_x;
    ExtPtr<float> m_y;
    ExtPtr<float> m_z;
};

#endif // MOB_H
