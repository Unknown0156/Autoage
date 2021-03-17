#ifndef RADAR_H
#define RADAR_H

#include <QWidget>
#include <QCloseEvent>
#include <QPainter>
#include <QImage>
#include <QRect>

#include "constants.h"
#include "player.h"
#include "mobs.h"

namespace Ui {
class Radar;
}

class Radar : public QWidget
{
    Q_OBJECT

public:
    explicit Radar(QWidget *parent = nullptr, Player *player=nullptr, Target *target=nullptr, const QVector<Mob*> *mobs=nullptr);
    ~Radar();

    void timerEvent(QTimerEvent *e);
    void paintEvent(QPaintEvent *e);
    void closeEvent (QCloseEvent *e);

signals:
    void onClose(bool check);

private:
    int timerId=0;
    Ui::Radar *ui;
    Player *m_player;
    Target *m_target;
    const QVector<Mob*> *m_mobs;
    QImage pImage;
    QRect pRect;
    QImage tImage;
    QRect tRect;
    QImage mImage;
    QImage fmImage;
    QImage dmImage;
    QRect mRect;
};

#endif // RADAR_H
