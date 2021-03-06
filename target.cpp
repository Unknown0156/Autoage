#include "target.h"

Target::Target()
    :m_name("targetName"), m_hp("targetHP"), m_maxHp("targetMaxHP"), m_x("targetX"), m_y("targetY"), m_z("targetZ")
{

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
