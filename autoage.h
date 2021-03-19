#ifndef AUTOAGE_H
#define AUTOAGE_H

#include <windows.h>

#include <QMainWindow>
#include <QTimer>
#include <QVector>

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

    Point &stPos() {return m_stPos;}
    void userPrint();//вывод данных в ui
    void start();
    void stop();

    void moveTo();

private slots:
    void botting(){
        if(player->status()==PStatus::waiting){//если игрок ничего не делает
            if(player->distTo(m_stPos.x, m_stPos.y)>MAX_DIST_FROM_START && target->hp()==0)//если ушел на макс удаление от старта
                player->moveTo(m_stPos.x, m_stPos.y);//бежит на старт
            if (target->hp()>0){//если есть цель
                player->kill(target);
                player->loot(target);
            }else{//если цели нет
                if(((float)player->hp()/player->maxHp())<0.8f)//если персонаж продамажен
                    player->heal();
                Mob* closest=mobs->closestTo(player->x(), player->y());//находит ближайшего моба
                player->moveTo(closest);//двигается к ближайшему мобу
                keyDown('\t');//таб ВРЕМЕННО
                Sleep(getRandomNumber(50,70));
                keyUp('\t');
                Sleep(getRandomNumber(50,70));
                if(target->hp()==0)//если не нашли цель ВРЕМЕННО
                    player->moveTo(m_stPos.x, m_stPos.y);
            }
        }
    }

private:
    Ui::Autoage *ui;
    Radar *radar=nullptr;
    Mobslist *mobslist=nullptr;
    int timerId=0; //таймер главного окна
    QTimer *bot;
    Player *player;
    Target *target;
    Mobs *mobs;
    Point m_stPos;
};
#endif // AUTOAGE_H
