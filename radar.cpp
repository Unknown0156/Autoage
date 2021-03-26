#include <cmath>
#include "radar.h"

Radar::Radar(QWidget *parent, Player *player, Target *target, const QVector<Mob*> *mobs) :
    QWidget(parent), m_player(player), m_target(target), m_mobs(mobs)
{
    setFixedSize(350, 350);
    setStyleSheet("background-color:white;");
    pImage.load(":/images/player.png");
    pRect=pImage.rect();
    tImage.load(":/images/target.png");
    tRect=tImage.rect();
    mImage.load(":/images/mob.png");
    fmImage.load(":/images/friendlymob.png");
    dmImage.load(":/images/deadmob.png");
    mRect=mImage.rect();
}

Radar::~Radar()
{

}


void Radar::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    painter.drawLine(0,0,349,0);
    painter.translate(this->width()/2,this->height()/2);//на центр окна
    //отрисовка иконки игрока
    painter.save();
    painter.rotate(-m_player->angle());
    painter.translate(pRect.height()/-2, pRect.width()/-2);
    painter.drawImage(pRect, pImage);
    painter.restore();
    //отрисовка иконки таргета
    painter.save();
    painter.translate(m_target->x()-m_player->x(), -(m_target->y()-m_player->y()));
    painter.translate(tRect.height()/-2, tRect.width()/-2);
    painter.drawImage(tRect, tImage);
    painter.restore();
    //отрисовка мобов овкруг
    foreach(Mob *mob, *m_mobs){
        painter.save();
        painter.translate(mob->x()-m_player->x(), -(mob->y()-m_player->y()));
        painter.translate(mRect.height()/-2, mRect.width()/-2);
        if(mob->hp())
            if(mob->enemy())
                painter.drawImage(mRect, mImage);
            else
                painter.drawImage(mRect, fmImage);
        else
            painter.drawImage(mRect, dmImage);
        painter.restore();
    }

}
