#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QObject>

#include "constants.h"
#include "extptr.h"
#include "target.h"

enum PStatus //что делает персонаж
{
    waiting,
    turning,
    moving,
    fighting,
    looting
};



class Player: public QObject
{
    Q_OBJECT

public:
    Player();
    ~Player();

    QString nick(){return *m_nick;}
    int hp(){return *m_hp;}
    int maxHp(){return *m_maxHp;}
    int mp();
    int maxMp();
    float x(){return *m_x;}
    float y(){return *m_y;}
    float z(){return *m_z;}
    float sin(){return *m_sin;}
    float cos(){return *m_cos;}
    float angle();
    PStatus status() const {return m_status;}


    void turnTo(float toX, float toY);//поворот к точке координат
    void turnTo(Target *tar);//поворот к таргету
    void moveTo(float toX, float toY, float dist=MOVE_TO_POINT_PRECISION);//движение к точке координат
    void moveTo(Target *tar, float dist=MOVE_TO_TARGET_PRECISION);//движение до таргета
    void kill(Target *tar);//убить таргет
    void drainHp(Target *tar); //высосать хп
    void loot(Target *tar);//залутать таргет

signals:
    void statusChanged(const QString &status);
    void sendStatus(const QString &status, int timeout=0);//сообщение в статус бар

public slots:
    void setStatus (PStatus status, const QString &statusMessage=0){
        if (status!=m_status){
            m_status=status;
            switch (m_status) {
            case PStatus::waiting:emit statusChanged("Player is waiting");break;
            case PStatus::turning:emit statusChanged("Player is turning");break;
            case PStatus::moving:emit statusChanged("Player is moving");break;
            case PStatus::fighting:emit statusChanged("Player is fighting");break;
            case PStatus::looting:emit statusChanged("Player is looting");break;
            default: emit statusChanged("Unknown status");
            }
        }
        if(statusMessage!=0)
            emit sendStatus(statusMessage);
    }

private:
    ExtPtr<QString> m_nick;
    ExtPtr<int> m_hp;
    ExtPtr<int> m_maxHp;
    ExtPtr<int> m_mp;
    ExtPtr<int> m_maxMp;
    ExtPtr<float> m_x;
    ExtPtr<float> m_y;
    ExtPtr<float> m_z;
    ExtPtr<float> m_sin;
    ExtPtr<float> m_cos;
    float m_angle;
    PStatus m_status=PStatus::waiting;

    float calcDist(float x, float y);
    float calcAngle(float x, float y);
    float calcAngleDif(float angle);
};

#endif // PLAYER_H
