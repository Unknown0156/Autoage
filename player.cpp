#include "player.h"

Player::Player(Target *target)
    :m_nick("playerNick"), m_maxHp("playerMaxHP"), m_maxMp("playerMaxMP"), m_mp(&m_maxMp, "playerMP"),
     m_x("playerX"), m_y(&m_x, "playerY"), m_z(&m_x, "playerZ"), m_hp(&m_x, "playerHP"),
     m_cos("playerCos"), m_sin(&m_cos, "playerSin"), m_target(target),
     check(new QTimer), loop(new QEventLoop)
{
    m_angle=atan2(*m_sin, *m_cos);
    m_angle=(m_angle > 0 ? m_angle : (2*M_PI + m_angle)) * 180 / M_PI;
    connect(this, &Player::exitLoop, loop, &QEventLoop::quit);
    connect(check, &QTimer::timeout, this, &Player::checking);
}

Player::~Player()
{
    delete check;
    delete loop;
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

void Player::looping()
{
    check->start(TIMER_DELAY);
    loop->exec();
    check->stop();
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

float Player::distTo(const Point p)//расчет расстояния до точки координат
{
    float dX = p.x - *m_x; //дельта по X
    float dY = p.y - *m_y; //дельта по Y
    float dist = sqrt(dX * dX + dY * dY); //расстояние
    return dist;
}

void Player::turnTo(const Point p, float angleDif)//поворот к точке координат
{
    float angle = angleTo(p);//найти угол
    float cAngleDif=angle-this->angle();//разница между текущим и нужным углом
    cAngleDif = cAngleDif >= 0 ? cAngleDif : cAngleDif + 360.0;//положителная разница улов
    char turnKey = cAngleDif < 180.0f ? 'a' : 'd';//если разница углов меньше 180 жмет 'a', если нет 'd'
    cAngleDif=cAngleDif<180.0 ? cAngleDif : abs(cAngleDif-360.0);//асолютная дельта между углами 0-180
    if (cAngleDif>angleDif){
        QString statusStr="Turning to point ("+QString::number(p.x)+";"+QString::number(p.y)+")"+", angle=";
        keyDown(turnKey); //нажимает кпопку поворота
        m_memory.angleDif=angleDif;
        m_memory.angle=angle;
        m_memory.cAngleDif=cAngleDif;
        m_memory.turnKey=turnKey;
        if(m_status==PStatus::moving){
            setStatus(PStatus::turnandmove, statusStr+QString::number(angle));//устанавливает статус
        }else{
            setStatus(PStatus::turning, statusStr+QString::number(angle));//устанавливает статус
            looping();
        }
    }
}

void Player::onTurning()
{
    float newAngleDif=m_memory.angle-this->angle();//пересчет разницы углов
    newAngleDif = newAngleDif >= 0 ? newAngleDif : newAngleDif + 360.0;//положителная разница улов
    newAngleDif=newAngleDif<180.0 ? newAngleDif : abs(newAngleDif-360.0);//асолютная дельта между углами 0-180
    if(m_memory.cAngleDif==newAngleDif)//если не поворачивает
        keyDown(m_memory.turnKey);
    m_memory.cAngleDif=newAngleDif;
    if(newAngleDif<m_memory.angleDif){
        if(m_status==PStatus::turnandmove){
            keyUp(m_memory.turnKey);
            setStatus(PStatus::moving);
        }else{
            keyUp(m_memory.turnKey);
            setStatus(PStatus::waiting);
            loop->exit();
        }
    }
}
void Player::moveTo(const Point p, float dist)//движение к точке координат
{
    turnTo(p);//поворот до точки
    float cDist = distTo(p); //текущее расстояние до точки
    if(cDist>dist){
        QString statusStr="Moving to point ("+QString::number(p.x)+";"+QString::number(p.y)+")"+", distance=";
        m_memory.statusStr=statusStr;
        m_memory.dist=dist;
        m_memory.p=p;
        m_memory.cDist=cDist;
        m_memory.count=0;
        keyDown('w');
        setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
        looping();
    }
}

void Player::moveTo(Mob *mob, float dist)//движение к мобу
{
    Point p{mob->x(), mob->y()};//точка моба
    turnTo(p);//поворот до моба
    float cDist = distTo(p); //текущее расстояние до моба
    if(cDist>dist){
        QString statusStr="Moving to mob ("+QString::number(p.x)+";"+QString::number(p.y)+")"+", distance=";
        m_memory.statusStr=statusStr;
        m_memory.dist=dist;
        m_memory.mob=mob;
        m_memory.cDist=cDist;
        m_memory.count=0;
        keyDown('w');
        setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
        looping();
    }
}

void Player::onMoving()
{
    m_memory.count++;
    if(m_memory.mob!=nullptr)
        m_memory.p=Point {m_memory.mob->x(), m_memory.mob->y()};
    float newDist=distTo(m_memory.p);//пересчет расстояния
    if(m_memory.count%5==0){//раз в 5 таймеров
        if(abs(m_memory.cDist-newDist)<0.2f){//прошел меньше ...
            keyDown('w');
            wait(getRandomNumber(20,30));
            keyClick(' ');//прыжок
        } 
        m_memory.cDist=newDist;
    }
    emit sendStatus(m_memory.statusStr+QString::number(newDist));//в статус бар
    if(m_status==PStatus::moving)
        turnTo(m_memory.p,TURN_PRECISION*2.0f);//выравнивание на бегу
    if(newDist<m_memory.dist){
        if(m_status==PStatus::turnandmove){
            keyUp('w');
            setStatus(PStatus::turning);
        }else{
            m_memory.count=0;
            m_memory.mob=nullptr;
            keyUp('w');
            setStatus(PStatus::waiting);
            loop->exit();
        }
    }
}


void Player::kill(Target *tar)//убить таргет
{
    m_memory.cTarHp=tar->hp();
    m_memory.count=0;
    m_memory.drainCD=false;
    setStatus(PStatus::fighting, "Killing target: "+tar->name());
    while(tar->hp()>0){
        //loop->exec();
        looping();
        if(((float)this->hp()/this->maxHp())<0.95f && !m_memory.drainCD){
            moveTo(m_target, MOVE_TO_MOB_PRECISION/2.0f);
            setStatus(PStatus::fighting, "Killing target: "+m_target->name());
            wait(getRandomNumber(200,300));
            keyClick('2');
            m_memory.drainCD=true;
        }
    }
    setStatus(PStatus::waiting);
}

void Player::onFighting()
{
    if(loop->isRunning()){
    if(((float)this->hp()/this->maxHp())>0.8f && m_memory.count==0)
        keyClick('5');
    if(m_memory.count%2==0){
        if(((float)this->hp()/this->maxHp())<0.95f && !m_memory.drainCD)//если персонаж продамажен и дрэйн не на кд
            loop->exit();
        else
            keyClick('3');
    }
    if(m_memory.count%15==0){//если хп цели не убывает
        //if((m_memory.cTarHp-m_memory.tar->hp())<500)
            //keyDown('3');
        //m_memory.cTarHp=m_memory.tar->hp();
    }
    if(m_target->hp()==0)
        loop->exit();
    m_memory.count++;
    }
}

void Player::loot(Target *tar)//залутать таргет
{
    setStatus(PStatus::looting, "Looting target: "+tar->name());
    moveTo(tar,MOVE_TO_POINT_PRECISION*2.0f);
    wait(getRandomNumber(200,300));
    keyClick('f');
    setStatus(PStatus::waiting);
}

void Player::heal()//похилиться
{
    setStatus(PStatus::healing, "Healing...");
    wait(getRandomNumber(200,300));
    keyClick('4');
    wait(getRandomNumber(50,70));
    keyClick('1');
    setStatus(PStatus::waiting);
}


