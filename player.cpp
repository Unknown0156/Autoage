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
    m_start={*m_x, *m_y};
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



float Player::distTo(float x, float y)//расчет расстояния до точки координат
{
    float dX = x - *m_x; //дельта по X
    float dY = y - *m_y; //дельта по Y
    float dist = sqrt(dX * dX + dY * dY); //расстояние
    return dist;
}

float Player::angleTo(float x, float y)//расчет угла до точки координат
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

void Player::jump()
{
    keyDown(' ');
    wait(getRandomNumber(50,70));
    keyUp(' ');
}


void Player::moveTo(float toX, float toY, float dist)//движение к точке координат
{
    float cDist = this->distTo(toX, toY); //текущее расстояние до цели
    this->turnTo(toX, toY);//поворот до цели
    if(cDist>dist){
        QString statusStr="Moving to point ("+QString::number(toX)+";"+QString::number(toY)+")"+", distance=";
        setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
        keyDown('w');
        float angle, angleDif; //угол до цели и разница между углами
        while (cDist>dist){//ЦИКЛ ВАЙЛ!!!
            angle = this->angleTo(toX, toY);//пересчет угла
            angleDif=this->calcAngleDif(angle);//разница между текущим и нужным углом 0-360
            angleDif=angleDif<180.0 ? angleDif : abs(angleDif-360.0);//асолютная дельта между углами 0-180
            if(angleDif>(TURN_PRECISION*2.0f)){//выравнивание на бегу
                this->turnTo(toX, toY);
                setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
            }
            wait(getRandomNumber(50,70));
            float newDist=this->distTo(toX, toY);//пересчет расстояния
            if(cDist==newDist){//если не двигается
                //this->turnTo(0,0);
                //setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
                keyDown('w');
                wait(getRandomNumber(50,70));
                jump();
            }
            cDist=newDist;
            emit sendStatus(statusStr+QString::number(cDist));//в статус бар
        }
        keyUp('w');
        setStatus(PStatus::waiting);
    }
}

void Player::moveTo(Mob *mob, float dist)//движение к мобу
{
    float cDist = this->distTo(mob->x(), mob->y()); //текущее расстояние до моба
    this->turnTo(mob);//поворот до моба
    if(cDist>dist){
        QString statusStr="Moving to mob ("+QString::number(mob->x())+";"+QString::number(mob->y())+")"+", distance=";
        setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
        keyDown('w');
        float angle, angleDif; //угол до моба и разница между углами
        while (cDist>dist){//ЦИКЛ ВАЙЛ!!!
            angle = this->angleTo(mob->x(), mob->y());//пересчет угла
            angleDif=this->calcAngleDif(angle);//разница между текущим и нужным углом 0-360
            angleDif=angleDif<180.0 ? angleDif : abs(angleDif-360.0);//асолютная дельта между углами 0-180
            if(angleDif>(TURN_PRECISION*2.0f)){//выравнивание на бегу
                this->turnTo(mob);
                setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
            }
            wait(getRandomNumber(50,70));
            float newDist=this->distTo(mob->x(), mob->y());//пресчет расстояния
            if(cDist==newDist){//если не двигается
                //this->turnTo(0,0);
                //setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
                keyDown('w');
                wait(getRandomNumber(50,70));
                jump();
            }
            cDist=newDist;
            emit sendStatus(statusStr+QString::number(cDist));//в статус бар
        }
        keyUp('w');
        setStatus(PStatus::waiting);
    }

}

void Player::turnTo(float toX, float toY)//поворот к точке координат
{
    float angle = this->angleTo(toX, toY);//найти угол
    float angleDif=this->calcAngleDif(angle);//разница между текущим и нужным углом
    char turnKey=' ';//кнопка поворота
    turnKey = angleDif < 180.0f ? 'a' : 'd';//если разница углов меньше 180 жмет 'a', если нет 'd'
    angleDif=angleDif<180.0 ? angleDif : abs(angleDif-360.0);//асолютная дельта между углами 0-180
    if (angleDif>TURN_PRECISION){
        QString statusStr="Turning to point ("+QString::number(toX)+";"+QString::number(toY)+")"+", angle=";
        setStatus(PStatus::turning, statusStr+QString::number(angle));//устанавливает статус
        keyDown(turnKey); //нажимает кпопку поворота
        while (angleDif>TURN_PRECISION){ //ЦИКЛ ВАЙЛ!!!
            wait(getRandomNumber(50,70));
            //angle=this->angleTo(toX, toY);//пересчет угла
            angleDif=this->calcAngleDif(angle);//пересчет разницы углов
            angleDif=angleDif<180.0 ? angleDif : abs(angleDif-360.0);//асолютная дельта между углами 0-180
            emit sendStatus(statusStr+QString::number(angleDif));//в статус бар
        }
        keyUp(turnKey);
        setStatus(PStatus::waiting);
    }
}

void Player::turnTo(Mob *mob)//поворот к мобу
{
    float angle = this->angleTo(mob->x(), mob->y());//найти угол
    float angleDif=this->calcAngleDif(angle);//разница между текущим и нужным углом
    char turnKey=' ';//кнопка поворота
    turnKey = angleDif < 180.0f ? 'a' : 'd';//если разница углов меньше 180 жмет 'a', если нет 'd'
    angleDif=angleDif<180.0 ? angleDif : abs(angleDif-360.0);//асолютная дельта между углами 0-180
    if (angleDif>TURN_PRECISION){
        QString statusStr="Turning to point ("+QString::number(mob->x())+";"+QString::number(mob->y())+")"+", angle=";
        setStatus(PStatus::turning, statusStr+QString::number(angle));//устанавливает статус
        keyDown(turnKey); //нажимает кпопку поворота
        while (angleDif>TURN_PRECISION){ //ЦИКЛ ВАЙЛ!!!
            wait(getRandomNumber(50,70));
            angle = this->angleTo(mob->x(), mob->y());//пересчет угла
            angleDif=this->calcAngleDif(angle);//пересчет разницы углов
            angleDif=angleDif<180.0 ? angleDif : abs(angleDif-360.0);//асолютная дельта между углами 0-180
            emit sendStatus(statusStr+QString::number(angleDif));//в статус бар
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
    int tarCurHp=tar->hp();
    int count=0;
    while (tar->hp()>0){//ЦИКЛ ВАЙЛ!!!
        wait(getRandomNumber(100,200));
        if(((float)this->hp()/this->maxHp())<0.8f){//если персонаж продамажен
            keyUp('3');
            wait(getRandomNumber(100,200));
            keyDown('2');
            wait(getRandomNumber(50,70));
            keyUp('2');
            wait(getRandomNumber(50,70));
            keyDown('3');
        }
        count++;
        if(count>10){//если хп цели не убывает
            count=0;
            if((tarCurHp-tar->hp())<1000)
                keyDown('3');
            tarCurHp=tar->hp();
        }
    }
    setStatus(PStatus::waiting);
    keyUp('3');
}

void Player::loot(Target *tar)
{
    if (tar->hp()==0){
        setStatus(PStatus::looting, "Looting target: "+tar->name());
        //this->turnTo(tar);
        //this->moveTo(tar,MOVE_TO_POINT_PRECISION*2.0f);
        wait(getRandomNumber(200,300));
        keyDown('f');
        wait(getRandomNumber(50,70));
        keyUp('f');
        setStatus(PStatus::waiting);
    }
}

void Player::heal()
{
    setStatus(PStatus::fighting, "Healing...");
    wait(getRandomNumber(200,300));
    keyDown('4');
    wait(getRandomNumber(50,70));
    keyUp('4');
    wait(getRandomNumber(50,70));
    keyDown('1');
    wait(getRandomNumber(50,70));
    keyUp('1');
    setStatus(PStatus::waiting);
}

