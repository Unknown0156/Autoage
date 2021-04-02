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
    void closeEvent(QCloseEvent *e);

    void mobslistSH();//показать\скрыть окно списка мобов
    void radarSH();//показать\скрыть окно радара

    void userPrint();//вывод данных в ui
    void start();
    void stop();

private slots:
    void botting(){
        if(player->status()==PStatus::waiting){//если игрок ничего не делает
            if(target->loot()){//если цель не облутана
                player->loot();return;}
            if (target->hp()>0 && target->distTo(stPos)<farmRange*1.1){//если есть цель и она не далеко
                if(!player->moveTo(target))//бег к таргету
                    player->kill();
            }else{//если цели нет или она далеко   
                if(((float)player->hp()/player->maxHp())<0.8f)//если персонаж продамажен
                    player->heal();
                Mob* closest=mobs->closestTo(Point {player->x(), player->y()}, stPos, farmRange);//находит ближайшего к персонажу моба
                if(closest){
                    if(!player->moveTo(closest))//двигается к ближайшему мобу
                        keyClick('\t');//табает моба
                }else
                    player->moveTo(stPos);
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
    float farmRange;
};
#endif // AUTOAGE_H
