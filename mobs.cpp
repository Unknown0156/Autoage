#include "mobs.h"


Mob::Mob(const QString name, int num): m_base(name+QString::number(num)),
    m_enemy(&m_base, name+"Enemy"), m_type(&m_base, name+"Type"), m_x(&m_base, name+"X"), m_y(&m_base, name+"Y"), m_z(&m_base, name+"Z"), m_hp(&m_base, name+"HP")
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

float Mob::distTo(float x, float y)//расстояние до
{
    float dX = x - *m_x; //дельта по X
    float dY = y - *m_y; //дельта по Y
    float dist = sqrt(dX * dX + dY * dY); //расстояние
    return dist;
}

Mob::~Mob()
{

}

Target::Target()
    :Mob("target"), m_name("targetName"), m_maxHp("targetMaxHP"), m_hp(&m_maxHp, "targetHP")
{
    //m_hp=ExtPtr<int>(&m_maxHp, "targetHP");//реинициализация родительского поля
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

Mob *Mobs::closestTo(int x, int y)//возврат ближайшего моба
{
    this->refresh();
    this->filter();
    Mob *closest=nullptr;
    foreach(Mob* mob, m_mobs){
        if(closest==nullptr){
            if(mob->enemy() && mob->hp()>0)
                closest=mob;
            else
                continue;
        }
        if(mob->enemy() && mob->hp()>0 && mob->distTo(x, y) < closest->distTo(x,y))
            closest=mob;
    }
    return closest;
}




















