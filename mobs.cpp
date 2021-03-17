#include "mob.h"

Mob::Mob()
{

}

Mob::Mob(const QString name, int num): m_base(name+QString::number(num)),
    m_type(&m_base, name+"Type"), m_x(&m_base, name+"X"), m_y(&m_base, name+"Y"), m_z(&m_base, name+"Z")
{

}

void Mob::refresh()
{
    m_base=ExtPtr<int>(m_base.getName());
    m_type=ExtPtr<QString>(&m_base, "mobType");
    m_x=ExtPtr<float>(&m_base, "mobX");
    m_y=ExtPtr<float>(&m_base, "mobY");
    m_z=ExtPtr<float>(&m_base, "mobZ");

}
