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
            if(player->distTo(m_stPos)>MAX_DIST_FROM_START && target->hp()==0)//если ушел на макс удаление от старта
                player->moveTo(m_stPos);//бежит на старт
            if (target->hp()>0){//если есть цель
                player->moveTo(target);//бег к таргету
                player->kill(target);
                player->loot(target);
            }else{//если цели нет
                if(((float)player->hp()/player->maxHp())<0.8f)//если персонаж продамажен
                    player->heal();
                Mob* closest=mobs->closestTo(Point {player->x(), player->y()});//находит ближайшего к персонажу моба
                player->moveTo(closest);//двигается к ближайшему мобу
                keyClick('\t');//табает моба
                if(target->hp()==0)//если не нашли цель ВРЕМЕННО
                    player->moveTo(m_stPos);
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
    Point m_stPos;
};
#endif // AUTOAGE_H
