#include "autoage.h"
#include "ui_autoage.h"

#include <QDebug>

Autoage::Autoage(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Autoage),
      bot(new QTimer(this)), target(new Target), player(new Player(target)), mobs(new Mobs)
{
    ui->setupUi(this);
    setFixedSize(geometry().width(), geometry().height());//фиксирует размер окна
    statusBar()->showMessage("Ready");
    ui->pHpBar->setStyleSheet("QProgressBar::chunk{background-color: #6B9F18;}");//цвет хп бара персонажа
    ui->pMpBar->setStyleSheet("QProgressBar::chunk{background-color: #2B7ED5;}");//цвет мана бара персонажа
    ui->tHpBar->setStyleSheet("QProgressBar::chunk{background-color: #6B9F18;}");//цвет хп бара цели

    stPos={player->x(), player->y()};//инициализация стартовой позиции
    farmRange=ui->farmRange->value();//радуис фарма из ui
    timerId=startTimer(TIMER_DELAY);//таймер главного окна

    connect(player, &Player::statusChanged, ui->pStatus, &QLabel::setText);//статус персонажа в ui
    connect(player, &Player::sendStatus, statusBar(), &QStatusBar::showMessage);//сообщения в статус бар

    connect(ui->quit, &QAction::triggered, qApp, &QApplication::quit);
    connect(ui->radar, &QAction::triggered, this, &Autoage::radarSH);
    connect(ui->mobslist, &QAction::triggered, this, &Autoage::mobslistSH);
    connect(ui->start, &QPushButton::clicked, this, &Autoage::start);
    connect(ui->stop, &QPushButton::clicked, this, &Autoage::stop);
    connect(bot, &QTimer::timeout, this, &Autoage::botting);
}

Autoage::~Autoage()
{
    delete ui;
    if (mobslist!=nullptr)
        delete mobslist;
    if (radar!=nullptr)
        delete radar;
    delete bot;
    delete player;
    delete target;
    delete mobs;
}

void Autoage::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    mobs->refresh();
    mobs->filter();
    userPrint(); //вывод в ui по таймеру
}

void Autoage::closeEvent(QCloseEvent *e){
    Q_UNUSED(e);
    if (mobslist!=nullptr){
        mobslist->close();
    }
}

void Autoage::mobslistSH()//показать\скрыть окно списка мобов
{
    if (mobslist==nullptr){
        mobslist=new Mobslist(nullptr, mobs);
        connect(mobslist, &Mobslist::onClose, ui->mobslist, &QAction::setChecked);
    }
    if (ui->mobslist->isChecked())
        mobslist->show();
    else
        mobslist->hide();
}

void Autoage::radarSH()//показать\скрыть радар
{
    if (radar==nullptr){
        radar=new Radar(stPos, farmRange, this, player, target, &mobs->mobs());
        radar->move(0,geometry().height()-statusBar()->height());
    }
    if (ui->radar->isChecked()){
        radar->show();
        this->setFixedSize(geometry().width(), geometry().height()+radar->height());
    }else{
        this->setFixedSize(geometry().width(), geometry().height()-radar->height());
        delete radar;
        radar=nullptr;
    }
}


void Autoage::userPrint()//вывод данных в ui
{
    //Стартовая позиция
    ui->startPos->setText("Start at ("+QString::number(stPos.x)+", "+QString::number(stPos.y)+"), dist="+QString::number(player->distTo(stPos),'f',2));

    //Персонаж
    QString title=QString::number(player->x(),'f',1)+" "+QString::number(player->y(),'f',1)+" "+player->nick();//заголовок окна
    setWindowTitle(title);
    ui->pHpBar->setMaximum(player->maxHp());
    ui->pHpBar->setValue(player->hp());
    ui->pMpBar->setMaximum(player->maxMp());
    ui->pMpBar->setValue(player->mp());
    ui->pX->setText("X: "+QString::number(player->x()));
    ui->pY->setText("Y: "+QString::number(player->y()));
    ui->pZ->setText("Z: "+QString::number(player->z()));
    ui->pCos->setText("Cos: "+QString::number(player->cos()));
    ui->pSin->setText("Sin: "+QString::number(player->sin()));
    ui->pAngle->setText("Angle: "+QString::number(player->angle()));


    //Таргет
    ui->tName->setText(target->name());
    ui->tHpBar->setMaximum(target->maxHp());
    ui->tHpBar->setValue(target->hp());
    ui->tX->setText("X: "+QString::number(target->x()));
    ui->tY->setText("Y: "+QString::number(target->y()));
    ui->tZ->setText("Z: "+QString::number(target->z()));

    //Мобы
    ui->mobs->setText("Mobs around: "+QString::number(mobs->mobs().size()));
    if(mobslist!=nullptr)
        mobslist->userPrint();
    if(radar!=nullptr)
        radar->repaint();
}

void Autoage::start()
{
    ui->start->setDisabled(true);
    ui->stop->setEnabled(true);
    stPos={player->x(), player->y()};
    farmRange=ui->farmRange->value();//радуис фарма из ui
    player->start();
    bot->start(TIMER_DELAY);
}

void Autoage::stop()
{
    ui->stop->setDisabled(true);
    ui->start->setEnabled(true);
    bot->stop();
    player->stop();
}




