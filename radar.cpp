#include <cmath>
#include "radar.h"
#include "ui_radar.h"

Radar::Radar(QWidget *parent, Player *player, Target *target, const QVector<Mob*> *mobs) :
    QWidget(parent), ui(new Ui::Radar),
    m_player(player), m_target(target), m_mobs(mobs)
{
    ui->setupUi(this);
    setFixedSize(this->height(), this->width());
    pImage.load(QCoreApplication::applicationDirPath()+"/"+"player.png");
    pRect=pImage.rect();
    tImage.load(QCoreApplication::applicationDirPath()+"/"+"target.png");
    tRect=tImage.rect();
    mImage.load(QCoreApplication::applicationDirPath()+"/"+"mob.png");
    fmImage.load(QCoreApplication::applicationDirPath()+"/"+"friendlymob.png");
    dmImage.load(QCoreApplication::applicationDirPath()+"/"+"deadmob.png");
    mRect=mImage.rect();
    timerId=startTimer(M_TIMER_DELAY);
}

Radar::~Radar()
{
    delete ui;
}

void Radar::timerEvent(QTimerEvent *e){
    Q_UNUSED(e);
    repaint();
}

void Radar::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);
    setWindowTitle("Radar: "+QString::number(m_mobs->size())+" mobs around");//заголовок окна

    QPainter painter(this);
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

void Radar::closeEvent(QCloseEvent *e)
{
    e->ignore();
    hide();
    emit onClose(false);

}
