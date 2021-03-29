#ifndef AUTOAGE_H
#define AUTOAGE_H

#include <windows.h>

#include <QMainWindow>
#include <QTimer>

#include "constants.h"
#include "functions.h"
#include "player.h"
#include "mobs.h"
#include "radar.h"
#include "mobslist.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Autoage; }
QT_END_NAMESPACE

class Autoage : public QMainWindow
{
    Q_OBJECT

public:
    Autoage(QWidget *parent = nullptr);
    ~Autoage();

    void timerEvent(QTimerEvent *e);
    void paintEvent(QPaintEvent *e);
    void closeEvent(QCloseEvent *e);

    void mobslistSH();//показать\скрыть окно списка мобов
    void radarSH();//показать\скрыть окно радара

    void userPrint();//вывод данных в ui
    void start();
    void stop();

    void moveTo();//ВРЕМЕННО

private slots:
    void botting(){
        if(player->status()==PStatus::waiting){//если игрок ничего не делает
            if(player->distTo(stPos)>MAX_DIST_FROM_START && target->hp()==0 && !target->loot()){//если ушел на макс удаление от старта
                player->moveTo(stPos);return;}//бежит на старт
            if (target->hp()>0 && player->distTo(target)<MAX_DIST_FROM_START){//если есть цель и она не далеко
                player->moveTo(target);//бег к таргету
                if(player->status()==PStatus::waiting)//если уже у таргета
                    player->kill();
            }else{//если цели нет или она далеко
                if(target->loot()){//если цель не облутана
                    player->loot();return;}
                if(((float)player->hp()/player->maxHp())<0.8f)//если персонаж продамажен
                    player->heal();
                Mob* closest=mobs->closestTo(Point {player->x(), player->y()});//находит ближайшего к персонажу моба
                player->moveTo(closest);//двигается к ближайшему мобу
                if(player->status()==PStatus::waiting)//если уже у ближайшего моба
                    keyClick('\t');//табает моба
            }
        }
    }

private:
    Ui::Autoage *ui;
    Radar *radar=nullptr;
    Mobslist *mobslist=nullptr;
    int timerId=0; //таймер главного окна
    QTimer *bot;
    Target *target;
    Player *player;
    Mobs *mobs;
    Point stPos;
};
#endif // AUTOAGE_H
