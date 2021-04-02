#ifndef RADAR_H
#define RADAR_H

#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QImage>
#include <QRect>

#include "constants.h"
#include "player.h"
#include "mobs.h"

class Radar : public QWidget
{
    Q_OBJECT

public:
    explicit Radar(const Point &stPos, const float &farmRange, QWidget *parent = nullptr, Player *player=nullptr, Target *target=nullptr, const QVector<Mob*> *mobs=nullptr);
    ~Radar();

    void paintEvent(QPaintEvent *e);

private:
    Player *m_player;
    Target *m_target;
    const QVector<Mob*> *m_mobs;
    const Point &m_stPos;
    const float &m_farmRange;
    QImage ptImage;
    QImage plImage;
    QRect pRect;
    QImage tImage;
    QRect tRect;
    QImage mImage;
    QImage fmImage;
    QImage dmImage;
    QRect mRect;
};

#endif // RADAR_H
