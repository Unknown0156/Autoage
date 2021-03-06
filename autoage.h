#ifndef AUTOAGE_H
#define AUTOAGE_H

#include <windows.h>

#include <QMainWindow>

#include "constants.h"
#include "functions.h"
#include "player.h"
#include "target.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Autoage; }
QT_END_NAMESPACE

class Autoage : public QMainWindow
{
    Q_OBJECT

public:
    Autoage(QWidget *parent = nullptr);
    ~Autoage();

    void timerEvent(QTimerEvent *);
    void paintEvent(QPaintEvent *);

    void userPrint();
    void start();
    void stop();

    void moveTo();


private:
    Ui::Autoage *ui;
    int timerId; //таймер главного окна
    Player *player;
    Target *target;
    bool botStarted=false;
};
#endif // AUTOAGE_H
