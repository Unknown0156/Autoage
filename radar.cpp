#include "radar.h"

Radar::Radar(const Point &stPos, const float &farmRange, QWidget *parent, Player *player, Target *target, const QVector<Mob*> *mobs) :
    QWidget(parent), m_player(player), m_target(target), m_mobs(mobs), m_stPos(stPos), m_farmRange(farmRange)
{
    setFixedSize(350, 350);
    setStyleSheet("background-color:white;");
    ptImage.load(":/images/point.png");
    plImage.load(":/images/player.png");
    pRect=ptImage.rect();
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
    //отрисовка стартовой точки и радиуса
    painter.save();
    painter.translate(m_stPos.x-m_player->x(), -(m_stPos.y-m_player->y()));
    painter.drawEllipse(QPointF(0,0), m_farmRange, m_farmRange);
    painter.translate(pRect.height()/-2, pRect.width()/-2);
    painter.drawImage(pRect, ptImage);
    painter.restore();
    //отрисовка иконки таргета
    painter.save();
    painter.translate(m_target->x()-m_player->x(), -(m_target->y()-m_player->y()));
    painter.translate(tRect.height()/-2, tRect.width()/-2);
    painter.drawImage(tRect, tImage);
    painter.restore();
    //отрисовка мобов вокруг
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
    //отрисовка иконки игрока
    painter.save();
    painter.rotate(-m_player->angle());
    painter.translate(pRect.height()/-2, pRect.width()/-2);
    painter.drawImage(pRect, plImage);
    painter.restore();

}
