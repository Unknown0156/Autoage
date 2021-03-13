#ifndef AUTOAGE_H
#define AUTOAGE_H

#include <windows.h>

#include <QMainWindow>
#include <QVector>

#include "constants.h"
#include "functions.h"
#include "player.h"
#include "target.h"
#include "mob.h"
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

    void mobslistSH();
    void radarSH();

    void userPrint();
    void start();
    void stop();

    void moveTo();

private:
    Ui::Autoage *ui;
    Radar *radar=nullptr;
    Mobslist *mobslist=nullptr;
    int timerId=0; //таймер главного окна
    Player *player;
    Target *target;
    QVector <Mob*> mobs;
    bool botStarted=false;
};
#endif // AUTOAGE_H
