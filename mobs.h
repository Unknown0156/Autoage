#ifndef MOB_H
#define MOB_H
#include <cmath>

#include <QString>

#include "constants.h"
#include "extptr.h"

struct Point{
    float x,y;
};

class Mob
{
public:
    Mob();
    Mob(const QString name, int num=NULL);//имя в файле оффсетов и номер если нужно
    ~Mob();

    const QString &name() const {return m_base.name();}
    int addr() const {return &m_base;}
    int base() const {return *m_base;}
    bool enemy() const {if(*m_enemy==115) return false; else return true;}
    QString type() const {return *m_type;}
    float x() const {return *m_x;}
    float y() const {return *m_y;}
    float z() const {return *m_z;}
    int hp() const {return *m_hp;}

    bool operator==(const Mob &mob){return mob.addr()==this->addr();}
    void refresh();//пересчет указателй
    float distTo(float x, float y);

private:

    ExtPtr<int> m_base;//базовый указатель
    ExtPtr<int> m_enemy;//>0 враг
    ExtPtr<QString> m_type;//тип моба
    ExtPtr<float> m_x;
    ExtPtr<float> m_y;
    ExtPtr<float> m_z;
    ExtPtr<int> m_hp;
};

class Target:public Mob
{
public:
    Target();
    ~Target();

    QString name();
    int maxHp() const {return *m_maxHp;}
    int hp() const {return *m_hp;}

private:
    ExtPtr<QString> m_name;
    ExtPtr<int> m_maxHp;
    ExtPtr<int> m_hp;

    //добавить уровень
    //добавить расстояние

};

class Mobs
{
public:
    Mobs();
    ~Mobs();

    const QVector <Mob*> &allmobs() const {return m_allmobs;}//возвращает ссылку на массив всех мобов
    const QVector <Mob*> &mobs() const {return m_mobs;}//возвращает ссылку на массив рабочих мобов

    void refresh();//обновление и фильтрация мобов
    Mob *closestTo(int x, int y) const;//возврат ближайшего моба

private:
    QVector <Mob*> m_allmobs;
    QVector <Mob*> m_mobs;
};

#endif // MOB_H
