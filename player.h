#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QObject>
#include <QTimer>

#include "constants.h"
#include "functions.h"
#include "extptr.h"
#include "mobs.h"

enum PStatus //что делает персонаж
{
    waiting,
    turning,
    moving,
    moveandturn,
    fighting,
    casting,
    healing,
    looting
};

struct Memory{
    int count;

    float angleDif;
    float angle;
    float cAngleDif;
    char turnKey;

    QString statusStr;
    Point p;
    Mob *mob=nullptr;
    float dist;
    float cDist;

    int cTarHp;

    int castTime;
};

class Player: public QObject
{
    Q_OBJECT

public:
    Player(Target *target);
    ~Player();

    QString nick() const {return *m_nick;}
    int maxHp() const {return *m_maxHp;}
    int maxMp();
    int mp() const {return *m_mp;}
    float x() const {return *m_x;}
    float y() const {return *m_y;}
    float z() const {return *m_z;}
    int hp() const {return *m_hp;}
    float cos() const {return *m_cos;}
    float sin() const {return *m_sin;}
    float angle();
    PStatus status() const {return m_status;}

    float distTo(const Point p);//расстояние до точки
    float distTo(const Mob* mob);//расстояние до моба
    bool moveTo(const Point p, float dist=MOVE_TO_POINT_PRECISION);//движение к точке
    bool moveTo(Mob *mob, float dist=MOVE_TO_MOB_PRECISION);//движение к мобу
    void kill();//убить таргет
    void loot();//залутать таргет
    void heal();//похилиться
    void start();//старт персонажа
    void stop();//остановка персонажа

signals:
    void statusChanged(const QString &status);//при изменении статуса персонажа
    void sendStatus(const QString &status, int timeout=0);//сообщение в статус бар

public slots:
    void setStatus (PStatus status, const QString &statusMessage=0){//изменение статуса игрока
        if (status!=m_status){
            m_status=status;
            switch (m_status) {
            case PStatus::waiting:emit statusChanged("Player is waiting");break;
            case PStatus::turning:emit statusChanged("Player is turning");break;
            case PStatus::moving:emit statusChanged("Player is moving");break;
            case PStatus::moveandturn:emit statusChanged("Player is turn and move");break;
            case PStatus::fighting:emit statusChanged("Player is fighting");break;
            case PStatus::casting:emit statusChanged("Player is casting");break;
            case PStatus::healing:emit statusChanged("Player is healing");break;
            case PStatus::looting:emit statusChanged("Player is looting");break;
            default: emit statusChanged("unknown_status");
            }
        }
        if(statusMessage!=0)
            emit sendStatus(statusMessage);
    }

    void checking(){
        switch(status()){
        case PStatus::waiting:{//если игрок ничего не делает
            break;
        }
        case PStatus::turning:{//если игрок поворачивается
            onTurning();break;}
        case PStatus::moving:{//если игрок бежит
            onMoving();break;}
        case PStatus::moveandturn:{//если игрок бежит и поворачитвается
            onTurning();onMoving();break;}
        case PStatus::fighting:{//если игрок бьет моба
            onFighting();break;}
        case PStatus::casting:{//если игрок бьет моба
            onCasting();break;}
        case PStatus::looting:{//если игрок бьет моба
            onLooting();break;}
        default:{
            break;
        }
        }
        if(m_arcCD>0)
            m_arcCD-=TIMER_DELAY;
        if(m_drainCD>0)
            m_drainCD-=TIMER_DELAY;
    }

private:
    ExtPtr<QString> m_nick;
    ExtPtr<int> m_maxHp;
    ExtPtr<int> m_maxMp;
    ExtPtr<int> m_mp;
    ExtPtr<float> m_x;
    ExtPtr<float> m_y;
    ExtPtr<float> m_z;
    ExtPtr<int> m_hp;
    ExtPtr<float> m_cos;
    ExtPtr<float> m_sin;
    float m_angle;
    PStatus m_status=PStatus::waiting;
    int m_arcCD=0;
    int m_drainCD=0;
    Memory m_memory;
    Target *m_target;
    QTimer *check;

    float angleTo(const Point p);//угол до точки
    bool turnTo(const Point p, float angleDif=TURN_PRECISION);//поворот к точке

    void proceedCD(int t);
    void wait(int t=0);
    bool keyClick(char keyS);//клик на кнопку

    void onTurning();//во время поворота
    void onMoving();//во время бега
    void onFighting();//во время файта
    void onCasting();//во время каста
    void onLooting();//во время лута
};

#endif // PLAYER_H
