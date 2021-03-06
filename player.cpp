#include <cmath>

#include <QCoreApplication>

#include "player.h"
#include "functions.h"

Player::Player()
    :m_nick("playerNick"),m_hp("playerHP"),m_maxHp("playerMaxHP"),m_mp("playerMP"),m_maxMp("playerMaxMP"),m_x("playerX"),m_y("playerY"),m_z("playerZ")
    ,m_sin("playerSin"), m_cos("playerCos")
{
    m_angle=atan2(*m_sin, *m_cos);
    m_angle=(m_angle > 0 ? m_angle : (2*M_PI + m_angle)) * 180 / M_PI;
}

Player::~Player()
{

}

int Player::mp(){//пока указатели маны глючат ВРЕМЕННО!!!
    if (*m_mp==NULL)
        m_mp=ExtPtr<int>("playerMP");
    return *m_mp;
}

int Player::maxMp(){//пока указатели маны глючат ВРЕМЕННО!!!
    if (*m_maxMp==NULL)
        m_maxMp=ExtPtr<int>("playerMaxMP");
    return *m_maxMp;
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
            cDist=this->calcDist(toX, toY);//пресчет расстояния
            emit sendStatus(statusStr+QString::number(cDist));//в статус бар
            QCoreApplication::processEvents();
        }
        keyUp('w');
        setStatus(PStatus::waiting);
    }
}

void Player::moveTo(Target *tar, float dist)//движение к таргету
{
    float cDist = this->calcDist(tar->x(), tar->y()); //текущее расстояние до цели
    this->turnTo(tar);//поворот до цели
    if(cDist>dist){
        QString statusStr="Moving to point ("+QString::number(tar->x())+";"+QString::number(tar->y())+")"+", distance=";
        setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
        keyDown('w');
        float angle, angleDif; //угол до цели и разница между углами
        while (cDist>dist){//ЦИКЛ ВАЙЛ!!!
            angle = this->calcAngle(tar->x(), tar->y());//пересчет угла
            angleDif=this->calcAngleDif(angle);//разница между текущим и нужным углом 0-360
            angleDif=angleDif<180.0 ? angleDif : abs(angleDif-360.0);//асолютная дельта между углами 0-180
            if(angleDif>(TURN_PRECISION*2.0f)){//выравнивание на бегу
                this->turnTo(tar);
            }
            Sleep(getRandomNumber(50,70));
            cDist=this->calcDist(tar->x(), tar->y());//пресчет расстояния
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

void Player::turnTo(Target *tar)//поворот к таргету
{
    float angle = this->calcAngle(tar->x(), tar->y());//найти угол
    float angleDif=this->calcAngleDif(angle);//разница между текущим и нужным углом
    char turnKey=' ';//кнопка поворота
    turnKey = angleDif < 180.0f ? 'a' : 'd';//если разница углов меньше 180 жмет 'a', если нет 'd'
    angleDif=angleDif<180.0 ? angleDif : abs(angleDif-360.0);//асолютная дельта между углами 0-180
    if (angleDif>TURN_PRECISION){
        setStatus(PStatus::turning, "Turning to angle: "+QString::number(angle));//устанавливает статус
        keyDown(turnKey); //нажимает кпопку поворота
        while (angleDif>TURN_PRECISION){ //ЦИКЛ ВАЙЛ!!!
            Sleep(getRandomNumber(50,70));
            angle = this->calcAngle(tar->x(), tar->y());//пересчет угла
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
        this->moveTo(tar, MOVE_TO_TARGET_PRECISION+5.0f);//добежать до таргета
        setStatus(PStatus::fighting);
        QCoreApplication::processEvents();
    }
    setStatus(PStatus::waiting);
    keyUp('3');
}

void Player::drainHp(Target *tar)
{
    float hpProc=this->hp()/this->maxHp()*100;
    if(hpProc<70.0){

    }
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

