#include <cmath>

#include <QCoreApplication>

#include "player.h"

Player::Player()
    :m_nick("playerNick"), m_maxHp("playerMaxHP"), m_maxMp("playerMaxMP"), m_mp(&m_maxMp, "playerMP"),
     m_x("playerX"), m_y(&m_x, "playerY"), m_z(&m_x, "playerZ"), m_hp(&m_x, "playerHP"),
     m_cos("playerCos"), m_sin(&m_cos, "playerSin")
{
    m_angle=atan2(*m_sin, *m_cos);
    m_angle=(m_angle > 0 ? m_angle : (2*M_PI + m_angle)) * 180 / M_PI;
}

Player::~Player()
{

}

int Player::maxMp(){//пока указатели маны глючат ВРЕМЕННО!!!
    if (*m_maxMp==NULL){
        m_maxMp=ExtPtr<int>("playerMaxMP");
    }

    return *m_maxMp;
}


int Player::mp(){//пока указатели маны глючат ВРЕМЕННО!!!
    if (*m_mp==NULL){
        m_mp=ExtPtr<int>(&m_maxMp, "playerMP");
    }

    return *m_mp;
}


float Player::angle()
{
    m_angle=atan2(*m_sin, *m_cos);
    m_angle=(m_angle > 0 ? m_angle : (2*M_PI + m_angle)) * 180 / M_PI;
    return m_angle;
}



float Player::calcDist(float x, float y)//расчет расстояния до точки координат
{
    float dX = x - *m_x; //дельта по X
    float dY = y - *m_y; //дельта по Y
    float dist = sqrt(dX * dX + dY * dY); //расстояние
    return dist;
}

float Player::calcAngle(float x, float y)//расчет угла до точки координат
{
    float dX = x - *m_x; //дельта по X
    float dY = y - *m_y; //дельта по Y
    float dist = sqrt(dX * dX + dY * dY); //расстояние
    float cos = dX / dist;
    float sin = dY / dist;
    float angle = atan2(sin, cos); //угол
    angle=(angle > 0 ? angle : (2*M_PI + angle)) * 180 / M_PI;
    return angle;
}

float Player::calcAngleDif(float angle)//расчет разницы текущего и нужного угла
{
    float angleDif=angle-this->angle();//разница между текущим и нужным углом
    angleDif = angleDif >= 0 ? angleDif : angleDif + 360.0;//положителная разница улов
    return angleDif;
}


void Player::moveTo(float toX, float toY, float dist)//движение к точке координат
{
    float cDist = this->calcDist(toX, toY); //текущее расстояние до цели
    this->turnTo(toX, toY);//поворот до цели
    if(cDist>dist){
        QString statusStr="Moving to point ("+QString::number(toX)+";"+QString::number(toY)+")"+", distance=";
        setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
        keyDown('w');
        float angle, angleDif; //угол до цели и разница между углами
        while (cDist>dist){//ЦИКЛ ВАЙЛ!!!
            angle = this->calcAngle(toX, toY);//пересчет угла
            angleDif=this->calcAngleDif(angle);//разница между текущим и нужным углом 0-360
            angleDif=angleDif<180.0 ? angleDif : abs(angleDif-360.0);//асолютная дельта между углами 0-180
            if(angleDif>(TURN_PRECISION*2.0f)){//выравнивание на бегу
                this->turnTo(toX, toY);
            }
            Sleep(getRandomNumber(50,70));
            float newDist=this->calcDist(toX, toY);//пресчет расстояния
            if(cDist==newDist)//если не двигается
                keyDown('w');
            cDist=newDist;
            emit sendStatus(statusStr+QString::number(cDist));//в статус бар
            QCoreApplication::processEvents();
        }
        keyUp('w');
        setStatus(PStatus::waiting);
    }
}

void Player::moveTo(Mob *mob, float dist)//движение к мобу
{
    float cDist = this->calcDist(mob->x(), mob->y()); //текущее расстояние до моба
    this->turnTo(mob);//поворот до моба
    if(cDist>dist){
        QString statusStr="Moving to point ("+QString::number(mob->x())+";"+QString::number(mob->y())+")"+", distance=";
        setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
        keyDown('w');
        float angle, angleDif; //угол до моба и разница между углами
        while (cDist>dist){//ЦИКЛ ВАЙЛ!!!
            angle = this->calcAngle(mob->x(), mob->y());//пересчет угла
            angleDif=this->calcAngleDif(angle);//разница между текущим и нужным углом 0-360
            angleDif=angleDif<180.0 ? angleDif : abs(angleDif-360.0);//асолютная дельта между углами 0-180
            if(angleDif>(TURN_PRECISION*2.0f)){//выравнивание на бегу
                this->turnTo(mob);
            }
            Sleep(getRandomNumber(50,70));
            float newDist=this->calcDist(mob->x(), mob->y());//пресчет расстояния
            if(cDist==newDist)//если не двигается
                keyDown('w');
            cDist=newDist;
            emit sendStatus(statusStr+QString::number(cDist));//в статус бар
            QCoreApplication::processEvents();
        }
        keyUp('w');
        setStatus(PStatus::waiting);
    }

}

void Player::turnTo(float toX, float toY)//поворот к точке координат
{
    float angle = this->calcAngle(toX, toY);//найти угол
    float angleDif=this->calcAngleDif(angle);//разница между текущим и нужным углом
    char turnKey=' ';//кнопка поворота
    turnKey = angleDif < 180.0f ? 'a' : 'd';//если разница углов меньше 180 жмет 'a', если нет 'd'
    angleDif=angleDif<180.0 ? angleDif : abs(angleDif-360.0);//асолютная дельта между углами 0-180
    if (angleDif>TURN_PRECISION){
        setStatus(PStatus::turning, "Turning to angle: "+QString::number(angle));//устанавливает статус
        keyDown(turnKey); //нажимает кпопку поворота
        while (angleDif>TURN_PRECISION){ //ЦИКЛ ВАЙЛ!!!
            Sleep(getRandomNumber(50,70));
            angleDif=this->calcAngleDif(angle);//пересчет разницы углов
            QCoreApplication::processEvents();
        }
        keyUp(turnKey);
        setStatus(PStatus::waiting);
    }
}

void Player::turnTo(Mob *mob)//поворот к мобу
{
    float angle = this->calcAngle(mob->x(), mob->y());//найти угол
    float angleDif=this->calcAngleDif(angle);//разница между текущим и нужным углом
    char turnKey=' ';//кнопка поворота
    turnKey = angleDif < 180.0f ? 'a' : 'd';//если разница углов меньше 180 жмет 'a', если нет 'd'
    angleDif=angleDif<180.0 ? angleDif : abs(angleDif-360.0);//асолютная дельта между углами 0-180
    if (angleDif>TURN_PRECISION){
        setStatus(PStatus::turning, "Turning to angle: "+QString::number(angle));//устанавливает статус
        keyDown(turnKey); //нажимает кпопку поворота
        while (angleDif>TURN_PRECISION){ //ЦИКЛ ВАЙЛ!!!
            Sleep(getRandomNumber(50,70));
            angle = this->calcAngle(mob->x(), mob->y());//пересчет угла
            angleDif=this->calcAngleDif(angle);//пересчет разницы углов
            QCoreApplication::processEvents();
        }
        keyUp(turnKey);
        setStatus(PStatus::waiting);
    }
}
void Player::kill(Target *tar)
{
    this->moveTo(tar);//бег к таргету
    this->turnTo(tar);//поворот к таргету
    setStatus(PStatus::fighting, "Killing target: "+tar->name());
    keyDown('3');
    while (tar->hp()>0){//ЦИКЛ ВАЙЛ!!!
        Sleep(getRandomNumber(50,70));
        //this->moveTo(tar, MOVE_TO_TARGET_PRECISION+5.0f);//добежать до таргета
        //setStatus(PStatus::fighting);
        QCoreApplication::processEvents();
    }
    setStatus(PStatus::waiting);
    keyUp('3');
}

void Player::loot(Target *tar)
{
    if (tar->hp()==0){
        setStatus(PStatus::looting, "Looting target: "+tar->name());
        this->moveTo(tar,MOVE_TO_POINT_PRECISION);
        keyDown('f');
        Sleep(getRandomNumber(50,70));
        QCoreApplication::processEvents();
        keyUp('f');
        Sleep(getRandomNumber(50,70));
        QCoreApplication::processEvents();
        keyDown('f');
        Sleep(getRandomNumber(50,70));
        QCoreApplication::processEvents();
        keyUp('f');
        setStatus(PStatus::waiting);
    }
}

