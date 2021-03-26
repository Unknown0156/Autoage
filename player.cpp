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



float Player::distTo(const Point p)//расчет расстояния до точки координат
{
    float dX = p.x - *m_x; //дельта по X
    float dY = p.y - *m_y; //дельта по Y
    float dist = sqrt(dX * dX + dY * dY); //расстояние
    return dist;
}

float Player::angleTo(const Point p)//расчет угла до точки координат
{
    float dX = p.x - *m_x; //дельта по X
    float dY = p.y - *m_y; //дельта по Y
    float dist = sqrt(dX * dX + dY * dY); //расстояние
    float cos = dX / dist;
    float sin = dY / dist;
    float angle = atan2(sin, cos); //угол
    angle=(angle > 0 ? angle : (2*M_PI + angle)) * 180 / M_PI;
    return angle;
}

void Player::moveTo(const Point p, float dist)//движение к точке координат
{
    float cDist = distTo(p); //текущее расстояние до точки
    turnTo(p);//поворот до точки
    if(cDist>dist){
        QString statusStr="Moving to point ("+QString::number(p.x)+";"+QString::number(p.y)+")"+", distance=";
        setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
        keyDown('w');
        while (cDist>dist){//ЦИКЛ ВАЙЛ!!!
            wait(getRandomNumber(70,90));
            float newDist=distTo(p);//пересчет расстояния
            if(cDist==newDist){//если не двигается
                keyDown('w');
                wait(getRandomNumber(20,30));
                keyClick(' ');//прыжок
            }
            cDist=newDist;
            turnTo(p,TURN_PRECISION*2.0f);//выравнивание на бегу
            setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
        }
        keyUp('w');
        setStatus(PStatus::waiting);
    }
}

void Player::moveTo(Mob *mob, float dist)//движение к мобу
{
    Point p{mob->x(), mob->y()};//точка моба
    float cDist = distTo(p); //текущее расстояние до моба
    turnTo(p);//поворот до моба
    if(cDist>dist){
        QString statusStr="Moving to mob ("+QString::number(p.x)+";"+QString::number(p.y)+")"+", distance=";
        setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
        keyDown('w');
        while (cDist>dist){//ЦИКЛ ВАЙЛ!!!
             wait(getRandomNumber(70,90));
             p=Point{mob->x(), mob->y()};//пересчет точки моба
             float newDist=distTo(p);//пресчет расстояния
             if(cDist==newDist){//если не двигается
                 keyDown('w');
                 wait(getRandomNumber(20,30));
                 keyClick(' ');
             }
             cDist=newDist;
             turnTo(p,TURN_PRECISION*2.0f);//выравнивание на бегу
             setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
        }
        keyUp('w');
        setStatus(PStatus::waiting);
    }

}

void Player::turnTo(const Point p, float angleDif)//поворот к точке координат
{
    float angle = angleTo(p);//найти угол
    float cAngleDif=angle-this->angle();//разница между текущим и нужным углом
    cAngleDif = cAngleDif >= 0 ? cAngleDif : cAngleDif + 360.0;//положителная разница улов
    char turnKey = cAngleDif < 180.0f ? 'a' : 'd';//если разница углов меньше 180 жмет 'a', если нет 'd'
    cAngleDif=cAngleDif<180.0 ? cAngleDif : abs(cAngleDif-360.0);//асолютная дельта между углами 0-180
    if (cAngleDif>angleDif){
        QString statusStr="Turning to point ("+QString::number(p.x)+";"+QString::number(p.y)+")"+", angleDif=";
        setStatus(PStatus::turning, statusStr+QString::number(cAngleDif));//устанавливает статус
        keyDown(turnKey); //нажимает кпопку поворота
        while (cAngleDif>angleDif){ //ЦИКЛ ВАЙЛ!!!
            wait(getRandomNumber(50,70));
            float newAngleDif=angle-this->angle();//пересчет разницы углов
            newAngleDif = newAngleDif >= 0 ? newAngleDif : newAngleDif + 360.0;//положителная разница улов
            newAngleDif=newAngleDif<180.0 ? newAngleDif : abs(newAngleDif-360.0);//асолютная дельта между углами 0-180
            if(cAngleDif==newAngleDif){//если не поворачивает
                keyDown(turnKey);
                wait(getRandomNumber(20,30));
            }
            cAngleDif=newAngleDif;
            emit sendStatus(statusStr+QString::number(cAngleDif));//в статус бар
        }
        keyUp(turnKey);
        setStatus(PStatus::waiting);
    }
}

void Player::kill(Target *tar)//убить таргет
{
    moveTo(tar);//бег к таргету
    turnTo(Point{tar->x(), tar->y()});//поворот к таргету
    setStatus(PStatus::fighting, "Killing target: "+tar->name());
    keyDown('3');
    int cTarHp=tar->hp();
    int count=0;
    while (tar->hp()>0){//ЦИКЛ ВАЙЛ!!!
        wait(getRandomNumber(100,200));
        if(((float)this->hp()/this->maxHp())<0.8f){//если персонаж продамажен
            keyUp('3');
            wait(getRandomNumber(100,200));
            keyClick('2');
            wait(getRandomNumber(30,50));
            keyDown('3');
        }
        count++;
        if(count%10==0){//если хп цели не убывает
            if((cTarHp-tar->hp())<500)
                keyDown('3');
            cTarHp=tar->hp();
        }
    }
    setStatus(PStatus::waiting);
    keyUp('3');
}

void Player::loot(Target *tar)//залутать таргет
{
    if (tar->hp()==0){
        setStatus(PStatus::looting, "Looting target: "+tar->name());
        turnTo(Point{tar->x(), tar->y()});
        moveTo(tar,MOVE_TO_POINT_PRECISION*2.0f);
        wait(getRandomNumber(200,300));
        keyClick('f');
        setStatus(PStatus::waiting);
    }
}

void Player::heal()//похилиться
{
    setStatus(PStatus::fighting, "Healing...");
    wait(getRandomNumber(200,300));
    keyClick('4');
    wait(getRandomNumber(50,70));
    keyClick('1');
    setStatus(PStatus::waiting);
}

