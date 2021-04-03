#include "player.h"

Player::Player(Target *target)
    :m_nick("playerNick"), m_maxHp("playerMaxHP"), m_maxMp("playerMaxMP"), m_mp(&m_maxMp, "playerMP"),
     m_x("playerX"), m_y(&m_x, "playerY"), m_z(&m_x, "playerZ"), m_hp(&m_x, "playerHP"),
     m_cos("playerCos"), m_sin(&m_cos, "playerSin"), m_target(target),
     check(new QTimer)
{
    m_angle=atan2(*m_sin, *m_cos);
    m_angle=(m_angle > 0 ? m_angle : (2*M_PI + m_angle)) * 180 / M_PI;
    connect(check, &QTimer::timeout, this, &Player::checking);
}

Player::~Player()
{
    delete check;
}

int Player::maxMp(){
    if(*m_maxMp<10000 || *m_maxMp>20000){
        m_maxMp=ExtPtr<int>("playerMaxMP");
        m_mp=ExtPtr<int>(&m_maxMp, "playerMP");
    }
    return *m_maxMp;
}

float Player::angle()
{
    m_angle=atan2(*m_sin, *m_cos);
    m_angle=(m_angle > 0 ? m_angle : (2*M_PI + m_angle)) * 180 / M_PI;
    return m_angle;
}

void Player::proceedCD(int t)
{
    if(m_arcCD>0)
        m_arcCD-=t;
    if(m_drainCD>0)
        m_drainCD-=t;
}

void Player::wait(int t)
{
    for(;t>=0;t-=20){
        Sleep(20);
        QCoreApplication::processEvents();
    }
    proceedCD(t);
}

bool Player::keyClick(char keyS)
{
    if(keyDown(keyS)){
        wait(getRandomNumber(50,70));
        keyUp(keyS);
        return true;
    }
    return false;
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
float Player::distTo(const Mob *mob)
{
    return distTo(Point{mob->x(), mob->y()});
}

bool Player::turnTo(const Point p, float angleDif)//поворот к точке координат
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
            setStatus(PStatus::moveandturn, statusStr+QString::number(angle));//устанавливает статус
        }else{
            setStatus(PStatus::turning, statusStr+QString::number(angle));//устанавливает статус
        }
        return true;
    }
    return false;
}

void Player::onTurning()
{
    float newAngleDif=m_memory.angle-this->angle();//пересчет разницы углов
    newAngleDif = newAngleDif >= 0 ? newAngleDif : newAngleDif + 360.0;//положителная разница улов
    newAngleDif=newAngleDif<180.0 ? newAngleDif : abs(newAngleDif-360.0);//асолютная дельта между углами 0-180
    if(newAngleDif<m_memory.angleDif){//если поврот завершен
        keyUp(m_memory.turnKey);
        if(m_status==PStatus::moveandturn){
            setStatus(PStatus::moving);return;
        }else{
            setStatus(PStatus::waiting);return;
        }
    }
    if(m_memory.cAngleDif==newAngleDif)//если не поворачивает
        keyDown(m_memory.turnKey);
    m_memory.cAngleDif=newAngleDif;

}

bool Player::moveTo(const Point p, float dist)//движение к точке координат
{
    if(turnTo(p)){//поворот до точки
        while(m_status==PStatus::turning){
            wait(TIMER_DELAY);
            onTurning();
        }
    }
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
        return true;
    }
    return false;
}
bool Player::moveTo(Mob *mob, float dist)//движение к мобу
{
    Point p{mob->x(), mob->y()};//точка моба
    if(turnTo(p)){//поворот до моба
        while(m_status==PStatus::turning){
            wait(TIMER_DELAY);
            onTurning();
        }
    }
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
        return true;
    }
    return false;
}
void Player::onMoving()
{
    m_memory.count++;
    if(m_memory.mob!=nullptr)
        m_memory.p=Point {m_memory.mob->x(), m_memory.mob->y()};
    float newDist=distTo(m_memory.p);//пересчет расстояния
    emit sendStatus(m_memory.statusStr+QString::number(newDist));//в статус бар
    if(newDist<m_memory.dist){//если добежал
        keyUp('w');
        m_memory.count=0;
        m_memory.mob=nullptr;
        if(m_status==PStatus::moveandturn){
            setStatus(PStatus::turning);return;
        }else{
            setStatus(PStatus::waiting);return;
        }
    }
    if(m_status==PStatus::moving){
    if(m_memory.count%10==0){//раз в секунду
        if(abs(m_memory.cDist-newDist)<0.2f){//при застревании
            keyDown('w');
            wait(getRandomNumber(50,70));
            float x=*m_x-(m_memory.p.y-*m_y)/distTo(m_memory.p)*10.0f;
            float y=*m_y+(m_memory.p.x-*m_x)/distTo(m_memory.p)*10.0f;
            Point p {x,y};
            m_memory.mob=nullptr;
            if(moveTo(p)){//разворот на 180 градусов
                keyClick(' ');//прыжок
                return;
            }
        } 
        m_memory.cDist=newDist;
    }
    turnTo(m_memory.p,TURN_PRECISION*2.0f);//выравнивание на бегу
    }

}

void Player::kill()//убить таргет
{
    m_memory.cTarHp=m_target->hp();
    m_memory.count=0;
    if(m_target->hp()>0)
        setStatus(PStatus::fighting, "Killing target: "+m_target->name());
}
void Player::onFighting()
{ 
    m_memory.count++;
    if(m_target->hp()==0){//если таргет убит
        m_memory.count=0;
        m_target->loot()=true;
        setStatus(PStatus::waiting);return;
    }
    if(m_memory.count%2==0){//раз в 0.2 секунды
        if(((float)this->hp()/this->maxHp())>0.8f && m_memory.count==2 && m_arcCD<=0){//открывается с молнии
            keyClick('5');
            m_arcCD=8000;
            m_memory.castTime=2500;
            setStatus(PStatus::casting);return;
        }
        if(((float)this->hp()/this->maxHp())<0.85f && (float)m_target->hp()/m_target->maxHp()>0.3f && m_drainCD<=0){//если персонаж продамажен и дрэйн не на кд
            if(moveTo(m_target, MOVE_TO_MOB_PRECISION/2.0f)){
                return;
            }else{
                wait(getRandomNumber(300,400));
                keyClick('2');
                m_drainCD=20000;
            }
        }else
            keyClick('3');
    }
    if(m_memory.count%50==0){//раз в 5 секунд
        if((m_memory.cTarHp-m_target->hp())<500){//если хп цели не убывает
            moveTo(m_target, MOVE_TO_POINT_PRECISION);
        }
        m_memory.cTarHp=m_target->hp();
    }  
}
void Player::onCasting()//во время каста
{
    m_memory.castTime-=TIMER_DELAY;
    if(m_memory.castTime<=0)
        setStatus(PStatus::fighting);

}

void Player::loot()//залутать таргет
{
    if(m_target->loot()){
        m_memory.p=Point{m_target->x(), m_target->y()};
        setStatus(PStatus::looting, "Looting target: "+m_target->name());
    }
}
void Player::onLooting()
{
    if(moveTo(m_memory.p,MOVE_TO_POINT_PRECISION*2.0f)){//если далеко от трупа
        return;
    }else{
        wait(getRandomNumber(200,300));
        keyClick('f');
        m_target->loot()=false;
        setStatus(PStatus::waiting);
    }
}

void Player::heal()//похилиться
{
    setStatus(PStatus::healing, "Healing...");
    wait(getRandomNumber(200,300));
    keyClick('4');
    wait(getRandomNumber(100,200));
    keyClick('1');
    setStatus(PStatus::waiting);
}

void Player::start()
{
    check->start(TIMER_DELAY);
    setStatus(PStatus::waiting);
}
void Player::stop()
{
    check->stop();
    switch(status()){
    case PStatus::waiting:{//если игрок ничего не делает
        break;
    }
    case PStatus::turning:{//если игрок поворачивается
        keyUp(m_memory.turnKey);break;}
    case PStatus::moving:{//если игрок бежит
        keyUp('w');break;}
    case PStatus::moveandturn:{//если игрок бежит и поворачитвается
        keyUp(m_memory.turnKey);keyUp('w');break;}
    case PStatus::fighting:{//если игрок бьет моба
        break;}
    case PStatus::casting:{//если игрок бьет моба
        break;}
    case PStatus::looting:{//если игрок бьет моба
        break;}
    default:{
        break;
    }
    }
    setStatus(PStatus::waiting);
}


