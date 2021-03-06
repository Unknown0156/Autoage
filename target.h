#ifndef TARGET_H
#define TARGET_H

#include <QString>

#include "extptr.h"

class Target
{
public:
    Target();
    ~Target();

    QString name();
    int hp(){return *m_hp;}
    int maxHp(){return *m_maxHp;}
    float x(){return *m_x;}
    float y(){return *m_y;}
    float z(){return *m_z;}

private:
    ExtPtr<QString> m_name;
    ExtPtr<int> m_hp;
    ExtPtr<int> m_maxHp;
    ExtPtr<float> m_x;
    ExtPtr<float> m_y;
    ExtPtr<float> m_z;

    //добавить уровень
    //добавить расстояние

};

#endif // TARGET_H
