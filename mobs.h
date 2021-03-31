#ifndef MOB_H
#define MOB_H
#include <cmath>

#include <QString>
#include <QDataStream>

#include "constants.h"
#include "extptr.h"

struct Point{
    float x,y,z=0;
};

QDataStream &operator>>(QDataStream &in, Point &p);//чтение точки из потока
QDataStream &operator<<(QDataStream &out, const Point &p);//запись точки в поток

class Mob
{
public:
    Mob(const QString name, int num=0);//имя в файле оффсетов и номер если нужно
    ~Mob();

    const QString &id() const {return m_base.name();}
    int addr() const {return &m_base;}
    int base() const {return *m_base;}
    virtual int enemy() const {return *m_enemy;}
    virtual QString type() const {return *m_type;}
    float x() const {return *m_x;}
    float y() const {return *m_y;}
    float z() const {return *m_z;}
    int hp() const {return *m_hp;}

    bool operator==(const Mob &mob){return mob.addr()==this->addr();}//сравнение по полю адреса
    void refresh();//пересчет указателей
    float distTo(const Point p);//расстояние до точки

private:
    ExtPtr<int> m_base;//базовый указатель
    ExtPtr<int> m_enemy;//друг\враг
    ExtPtr<QString> m_type;//тип моба
    ExtPtr<float> m_x;
    ExtPtr<float> m_y;
    ExtPtr<float> m_z;

protected:
    ExtPtr<int> m_hp;

};

class Target:public Mob
{
public:
    Target();
    ~Target();

    int enemy()=delete;
    QString type()=delete;
    QString name();
    int maxHp() const {return *m_maxHp;}
    bool &loot() {return m_loot;}

private:
    ExtPtr<QString> m_name;
    ExtPtr<int> m_maxHp;
    bool m_loot=false;
};

class Mobs
{
public:
    Mobs();
    ~Mobs();

    const QVector <Mob*> &allmobs() const {return m_allmobs;}//возвращает ссылку на массив всех мобов
    const QVector <Mob*> &mobs() const {return m_mobs;}//возвращает ссылку на массив рабочих мобов

    void refresh();//обновление мобов
    void filter();//фильтрация мобов
    Mob *closestTo (const Point p, const Point stPos, const int range=0);//возврат ближайшего к точке моба в радиусе фарма

private:
    QVector <Mob*> m_allmobs;
    QVector <Mob*> m_mobs;
    Mob *m_closest=nullptr;
};

#endif // MOB_H
