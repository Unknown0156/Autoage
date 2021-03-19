#ifndef AUTOAGE_H
#define AUTOAGE_H

#include <windows.h>

#include <QMainWindow>
#include <QHBoxLayout>
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

    void userPrint();//вывод данных в ui
    void start();
    void stop();

    void moveTo();

private slots:
    void botting(){
        if(player->status()==PStatus::waiting){
            mobs->refresh();
            if(player->distTo(player->start().x, player->start().y)>MAX_DIST_FROM_START && target->hp()==0)
                player->moveTo(player->start().x, player->start().y);
            if (target->hp()>0){
                player->kill(target);
                player->loot(target);
            }else{
                if(((float)player->hp()/player->maxHp())<0.8f)
                    player->heal();
                Mob* closest=mobs->closestTo(player->x(), player->y());
                player->moveTo(mobs->closestTo(player->x(), player->y()));
                keyDown('\t');
                Sleep(getRandomNumber(50,70));
                keyUp('\t');
                Sleep(getRandomNumber(50,70));
                if(target->hp()==0)
                    player->moveTo(player->start().x, player->start().y);
            }
        }
    }

private:
    Ui::Autoage *ui;
    Radar *radar=nullptr;
    Mobslist *mobslist=nullptr;
    int timerId=0; //таймер главного окна
    Player *player;
    Target *target;
    Mobs *mobs;
    bool botStarted=false;
    QTimer *bot;
};
#endif // AUTOAGE_H
