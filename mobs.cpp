#include "mobs.h"

QDataStream &operator>>(QDataStream &in, Point &p){//чтение точки из потока
    in >> p.x >> p.y >>p.z;
    return in;
}

QDataStream &operator<<(QDataStream &out, const Point &p){//запись точки в поток
    out << p.x << p.y <<p.z;
    return out;
}

Mob::Mob(const QString id, int num): m_base(id+QString::number(num)),
    m_enemy(&m_base, id+"Enemy"), m_type(&m_base, id+"Type"), m_x(&m_base, id+"X"), m_y(&m_base, id+"Y"), m_z(&m_base, id+"Z"), m_hp(&m_base, id+"HP")
{

}

void Mob::refresh()//пересчет указателей
{
    QString name=m_base.name();
    m_base=ExtPtr<int>(name);
    name.remove(QRegExp("[0-9]"));//удаляет номер моба
    m_enemy=ExtPtr<int>(&m_base, name+"Enemy");
    m_type=ExtPtr<QString>(&m_base, name+"Type");
    m_x=ExtPtr<float>(&m_base, name+"X");
    m_y=ExtPtr<float>(&m_base, name+"Y");
    m_z=ExtPtr<float>(&m_base, name+"Z");
    m_hp=ExtPtr<int>(&m_base, name+"HP");
}

float Mob::distTo(const Point p)//расстояние до точки
{
    float dX = p.x - *m_x; //дельта по X
    float dY = p.y - *m_y; //дельта по Y
    float dist = sqrt(dX * dX + dY * dY); //расстояние
    return dist;
}

Mob::~Mob()
{
}

Target::Target()
    :Mob("target"), m_name("targetName"), m_maxHp("targetMaxHP")
{
    Mob::m_hp=ExtPtr<int>(&m_maxHp, "targetHP");
}

Target::~Target()
{
}

QString Target::name()
{
    if (*m_name == "invalid_name")
        m_name = ExtPtr <QString>("targetName");
    return *m_name;
}


Mobs::Mobs()
{
    for (int i=0;i<NUMBER_OF_MOBS ;i++ ) {
        m_allmobs.push_back(new Mob("mob", i));
    }
    this->filter();
}

Mobs::~Mobs()
{
    foreach(Mob* mob, m_allmobs)
        delete mob;
    m_allmobs.clear();
    m_mobs.clear();
}

void Mobs::refresh()//обновлние мобов
{ 
    foreach (Mob* mob, m_allmobs) {
        mob->refresh();
    }
}

void Mobs::filter()//фильтрация мобов
{
    m_mobs.clear();//очищает массив мобов
    m_mobs.push_back(new Mob("mob",0));//добавляем игрока в массив
    foreach(Mob* mobptr, m_allmobs){//цикл по массиву всех мобов
        if(mobptr->base()==MOB_BASE)//если vtable правлильный
            foreach(Mob* mob, m_mobs){//цикл по массиву рабочих мобов
                if(mobptr->addr()==mob->addr())//если моб уже есть в массиве
                    break;
                if(mob==m_mobs.last())//если дошел до конца и моба нет
                    m_mobs.push_back(mobptr);//добавляем моба в массив рабочих мобов
            }
    }
    m_mobs.erase(m_mobs.begin());//удаляем игрока
}

Mob *Mobs::closestTo(const Point p, const Point stPos, const int range)//возврат ближайшего к точке моба в радиусе фарма
{
    if(m_closest){
        delete m_closest;
        m_closest=nullptr;
    }
    foreach(Mob* mob, m_mobs){//итерация по мобам
        if(m_closest==nullptr){//если первый моб еще не найден
            if(mob->enemy() && mob->hp()>0 && mob->distTo(stPos)<range){//если моб враг и его хп больше 0 и он в рендже от старта
                m_closest=new Mob("mob");
                *m_closest=*mob;
            }
        }else{
            if(mob->enemy() && mob->hp()>0 && mob->distTo(stPos)<range && mob->distTo(p) < m_closest->distTo(p))//если новый моб ближе текущего
                *m_closest=*mob;
        }
    }
    return m_closest;
}












