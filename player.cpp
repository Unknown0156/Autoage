#include "player.h"

Player::Player(Target *target)
    :m_nick("playerNick"), m_maxHp("playerMaxHP"), m_maxMp("playerMaxMP"), m_mp(&m_maxMp, "playerMP"),
     m_x("playerX"), m_y(&m_x, "playerY"), m_z(&m_x, "playerZ"), m_hp(&m_x, "playerHP"),
     m_cos("playerCos"), m_sin(&m_cos, "playerSin"),
     m_gx("playerGX"), m_gy(&m_gx, "playerGY"), m_gz(&m_gx, "playerGZ"),
     m_target(target), check(new QTimer)
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

void Player::proceedCD(int t)//кд
{
    if(m_arcCD>0)
        m_arcCD-=t;
    if(m_drainCD>0)
        m_drainCD-=t;
    if(m_healCD>0)
        m_healCD-=t;
}

void Player::wait(int t)//ожидание
{
    for(;t>=0;t-=20){
        Sleep(20);
        QCoreApplication::processEvents();
    }
    proceedCD(t);
}

bool Player::keyClick(char keyS)//клик на кнопку
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
    float dX, dY;
    if(m_gMove){//если двигаемся по глобальным координатам
        dX = p.x - *m_gx;
        dY = p.y - *m_gy;
    }else{//если по локальным
        dX = p.x - *m_x; //дельта по X
        dY = p.y - *m_y; //дельта по Y
    }
    float dist = sqrt(dX * dX + dY * dY); //расстояние
    float cos = dX / dist;
    float sin = dY / dist;
    float angle = atan2(sin, cos); //угол
    angle=(angle > 0 ? angle : (2*M_PI + angle)) * 180 / M_PI;
    return angle;
}
float Player::distTo(const Point p)//расчет расстояния до точки координат
{
    float dX, dY;
    if(m_gMove){//если двигаемся по глобальным координатам
        dX = p.x - *m_gx;
        dY = p.y - *m_gy;
    }else{//если по локальным
        dX = p.x - *m_x; //дельта по X
        dY = p.y - *m_y; //дельта по Y
    }
    float dist = sqrt(dX * dX + dY * dY); //расстояние
    return dist;
}
float Player::distTo(const Mob *mob)
{
    return distTo(Point{mob->x(), mob->y(),mob->z()});
}

Point Player::unstuckPoint(const Point p)//точка при застревании
{
    float x,y;
    if(m_gMove){
        x=*m_gx-(p.y-*m_gy)/distTo(p)*10.0f;
        y=*m_gy+(p.x-*m_gx)/distTo(p)*10.0f;
    }else{
        x=*m_x-(p.y-*m_y)/distTo(p)*10.0f;
        y=*m_y+(p.x-*m_x)/distTo(p)*10.0f;
    }
    return Point {x,y,0};
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
        m_mem.angleDif=angleDif;
        m_mem.angle=angle;
        m_mem.cAngleDif=cAngleDif;
        m_mem.turnKey=turnKey;
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
    float newAngleDif=m_mem.angle-this->angle();//пересчет разницы углов
    newAngleDif = newAngleDif >= 0 ? newAngleDif : newAngleDif + 360.0;//положителная разница улов
    newAngleDif=newAngleDif<180.0 ? newAngleDif : abs(newAngleDif-360.0);//асолютная дельта между углами 0-180
    if(newAngleDif<m_mem.angleDif){//если поврот завершен
        keyUp(m_mem.turnKey);
        if(m_status==PStatus::moveandturn){
            setStatus(PStatus::moving);return;
        }else{
            setStatus(PStatus::waiting);return;
        }
    }
    if(m_mem.cAngleDif==newAngleDif)//если не поворачивает
        keyDown(m_mem.turnKey);
    m_mem.cAngleDif=newAngleDif;
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
        m_mem.statusStr=statusStr;
        m_mem.dist=dist;
        m_mem.p=p;
        m_mem.cDist=cDist;
        m_mem.count=0;
        keyDown('w');
        setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
        return true;
    }
    return false;
}
bool Player::moveTo(Mob *mob, float dist)//движение к мобу
{
    Point p{mob->x(), mob->y(), mob->z()};//точка моба
    if(turnTo(p)){//поворот до моба
        while(m_status==PStatus::turning){
            wait(TIMER_DELAY);
            onTurning();
        }
    }
    float cDist = distTo(p); //текущее расстояние до моба
    if(cDist>dist){
        QString statusStr="Moving to mob ("+QString::number(p.x)+";"+QString::number(p.y)+")"+", distance=";
        m_mem.statusStr=statusStr;
        m_mem.dist=dist;
        m_mem.mob=mob;
        m_mem.cDist=cDist;
        m_mem.count=0;
        keyDown('w');
        setStatus(PStatus::moving, statusStr+QString::number(cDist));//устанавливает статус
        return true;
    }
    return false;
}
void Player::onMoving()
{
    m_mem.count++;
    if(m_mem.mob!=nullptr)
        m_mem.p=Point {m_mem.mob->x(), m_mem.mob->y(), m_mem.mob->z()};
    float newDist=distTo(m_mem.p);//пересчет расстояния
    emit sendStatus(m_mem.statusStr+QString::number(newDist));//в статус бар
    if(newDist<m_mem.dist){//если добежал
        keyUp('w');
        m_mem.count=0;
        m_mem.mob=nullptr;
        if(m_status==PStatus::moveandturn){
            setStatus(PStatus::turning);return;
        }else{
            setStatus(PStatus::waiting);return;
        }
    }
    if(m_status==PStatus::moving){
    if(m_mem.count%10==0){//раз в секунду
        if(abs(m_mem.cDist-newDist)<0.2f){//при застревании
            m_mem.mob=nullptr;
            if(moveTo(unstuckPoint(m_mem.p))){//бег к точке на 90 градусов
                keyClick(' ');//прыжок
                return;
            }
        }
        m_mem.cDist=newDist;
    }
    if(turnTo(m_mem.p,TURN_PRECISION*2.0f))//выравнивание на бегу
        return;
    }

}

void Player::kill()//убить таргет
{
    m_mem.cTarHp=m_target->hp();
    m_mem.count=0;
    if(m_target->hp()>0)
        setStatus(PStatus::fighting, "Killing target: "+m_target->name());
}
void Player::onFighting()
{ 
    m_mem.count++;
    if(m_target->hp()==0){//если таргет убит
        m_mem.count=0;
        m_target->loot()=true;
        setStatus(PStatus::waiting);return;
    }
    if(m_mem.count%2==0){//раз в 0.2 секунды
        if(((float)this->hp()/this->maxHp())>0.8f && m_mem.count==2 && m_arcCD<=0){//открывается с молнии
            keyClick('5');
            m_arcCD=8000;
            m_mem.castTime=2500;
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
    if(m_mem.count%50==0){//раз в 5 секунд
        if((m_mem.cTarHp-m_target->hp())<500){//если хп цели не убывает
            moveTo(m_target, MOVE_TO_POINT_PRECISION);
        }
        m_mem.cTarHp=m_target->hp();
    }  
}
void Player::onCasting()//во время каста
{
    m_mem.castTime-=TIMER_DELAY;
    if(m_mem.castTime<=0)
        setStatus(PStatus::fighting);

}

void Player::loot()//залутать таргет
{
    if(m_target->loot()){
        m_mem.p=Point{m_target->x(), m_target->y(), m_target->z()};
        setStatus(PStatus::looting, "Looting target: "+m_target->name());
    }
}
void Player::onLooting()
{
    if(moveTo(m_mem.p,MOVE_TO_POINT_PRECISION*2.0f)){//если далеко от трупа
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
    if(m_healCD<=0){
        setStatus(PStatus::healing, "Healing...");
        wait(getRandomNumber(200,300));
        keyClick('4');
        wait(getRandomNumber(100,200));
        keyClick('1');
        setStatus(PStatus::waiting);
        m_healCD=60000;
    }
}

void Player::nextTar()
{
    keyClick('\t');
}

void Player::start()
{
    setStatus(PStatus::waiting);
    check->start(TIMER_DELAY);
}
void Player::stop()
{
    check->stop();
    switch(status()){
    case PStatus::waiting:{//если игрок ничего не делает
        break;
    }
    case PStatus::turning:{//если игрок поворачивается
        keyUp(m_mem.turnKey);break;}
    case PStatus::moving:{//если игрок бежит
        keyUp('w');break;}
    case PStatus::moveandturn:{//если игрок бежит и поворачитвается
        keyUp(m_mem.turnKey);keyUp('w');break;}
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


